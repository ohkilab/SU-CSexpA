# Simple HTTPサーバの実装
## はじめに

最も単純なWebサーバとして，まずは以下に示す仕様の Simple HTTPサーバを実装します．

-   GET に応答できること
-   Keep-Alive はサポートしない
-   Content-Typeは，HTMLとJPGのみサポートすること
-   1回に1セッションのみ応答できればよい

グループワークの課題に直結しますので，必ず各個人で動作させられるようにしてください．

これまでと同様に，「exp1\_」という接頭語のついている関数は本実験用に作成するものです．

「exp1\_」という接頭語の付いていない関数は，C言語の標準ライブラリ関数となりますので，manコマンドやGoogle先生で調べてみてください．

以下に示すソースコードを実装し，各自でSimple HTTPサーバを動作させてみてください．

ソースコードの簡単な解説も載せましたが，よく分からない部分はグループ内で相談し十分理解し合うようにしてください．

## ソースコード解説

第一部で使用したソースコードと，以下の関数を組み合わせて Simple HTTPサーバを実装します．

### main()

まずはmain()関数から概説します．

各自で動作を確認してほしいので，xxxxx という部分は各自の学籍番号の下5桁を記述してください．

クライアントから接続要求が届いたら，以下を while(1) で無限に繰り返すという単純なプログラムです．

-   exp1\_http\_session() というセッション処理のための実験A用関数に渡す
-   exp1\_http\_session() の処理が終わったらアクセプトしたディスクリプタを正常終了し次の接続処理へ

複数クライアントからの接続要求に対応できるようになっていませんし，特に処理内容の高速化も意識していない単純なものとなります．

```c
int main(int argc, char **argv)
{
  int sock_listen;

  sock_listen = exp1_tcp_listen(xxxxx);

  while(1){
    struct sockaddr addr;
    int sock_client;
    int len;

    sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
    exp1_http_session(sock_client);

    shutdown(sock_client, SHUT_RDWR);
    close(sock_client);
  }
}
```

```{caution}
sock\_listen はクライアントからの接続要求待ち受け用のディスクリプタ，sock\_clientはクライアントとのHTTPデータ通信用のディスクリプタですので，無限ループ内で閉じているディスクリプタがどちらか注意してください．
```

### exp1\_http\_session()

main()で実行する[HTTP](https://ja.wikipedia.org/wiki/Hypertext_Transfer_Protocol)セッションに関する中心的な処理を記述している関数になります．

具体的には以下のような処理を行っています．

-   クライアントからデータを受信（ここではまず最初にHTTPリクエストを受信することになります）
-   クライアントから受信したデータにおけるヘッダ部分を分解（exp1\_parse\_header()にて）し，抽出された情報をexp1\_info\_type構造体で宣言された変数 **info** へ格納
-   抽出された情報infoを元にクライアントへ返すHTTPリプライを生成し返送（exp1\_http\_reply()にて）

```c
int exp1_http_session(int sock)
{
  char buf[2048];
  int recv_size = 0;
  exp1_info_type info;
  int ret = 0;

  while(ret == 0){
    int size = recv(sock, buf + recv_size, 2048, 0);

    if(size == -1){
      return -1;
    }

    recv_size += size;
    ret = exp1_parse_header(buf, recv_size, &info);
  }

  exp1_http_reply(sock, &info);

  return 0;
}
```

ここで，**exp1\_info\_type構造体** は以下のように宣言されています．以下では，最低限の情報のみしか管理してませんので，今後，Simple HTTPサーバを機能拡張し，様々な[HTTPメッセージ](https://ja.wikipedia.org/wiki/Hypertext_Transfer_Protocol#HTTP.E3.83.A1.E3.83.83.E3.82.BB.E3.83.BC.E3.82.B8)を返信できるようにしていく際には，この構造体を拡張していくと楽と思います．

-   cmd (HTTPリクエスト内の[HTTPメソッド](https://ja.wikipedia.org/wiki/Hypertext_Transfer_Protocol#.E3.83.A1.E3.82.BD.E3.83.83.E3.83.89)用：GETやPOSTなど)
-   path (HTTPリクエスト内のURI情報用)
-   real\_path (リクエスト対象の実体へのパス情報用)
-   type (HTTPリプライにおけるContent-Typeフィールド用：test/htmlやimage/jpegなど)
-   code (HTTPリプライにおける[HTTPステータスコード](https://ja.wikipedia.org/wiki/HTTP%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)用)
-   size（HTTPリプライにおけるContent-Lengthフィールド用）

```c
typedef struct
{
  char cmd[64];
  char path[256];
  char real_path[256];
  char type[64];
  int code;
  int size;
} exp1_info_type;
```

### exp1\_parse\_header()

クライアントからの[HTTPリクエスト](https://ja.wikipedia.org/wiki/Hypertext_Transfer_Protocol#HTTP.E3.83.A1.E3.83.83.E3.82.BB.E3.83.BC.E3.82.B8)（例：GET / HTTP/1.0）をパース（構文解析）する関数です．

現時点では，HTTPリクエストの1行目のみを抽出するステートマシン（状態機械）になっています．

1行目を抽出（PARSE\_STATUSかつ'\\r'（復帰）でない間）したら，exp1\_parse\_status()へその行の情報（status（例：GET / HTTP/1.0））を渡します．

exp1\_parse\_status()では，HTTPリクエストの1行目に含まれるHTTPメソッドのcmd（例：GET）やURI情報のpath（例：/）といった基本的な情報を **info** へ格納します．

格納されたinfoをexp1\_check\_file()へ渡して，リクエスト対象の実体へのパス情報やHTTPリプライで返す具体的な情報を **info** へ追加格納します．

HTTPリクエストをより詳細に解析させたい場合は，この関数を拡張していくと楽と思います．

```c
int exp1_parse_header(char* buf, int size, exp1_info_type* info)
{
  char status[1024];
  int i, j;

  enum state_type
  {
    PARSE_STATUS,
    PARSE_END
  }state;

  state = PARSE_STATUS;
  j = 0;
  for(i = 0; i < size; i++){
    switch(state){
    case PARSE_STATUS:
      if(buf[i] == '\r'){
        status[j] = '\0';
        j = 0;
        state = PARSE_END;
        exp1_parse_status(status, info);
        exp1_check_file(info);
      }else{
        status[j] = buf[i];
        j++;
      }
      break;
    }

    if(state == PARSE_END){
      return 1;
    }
  }

  return 0;
}
```

### exp1\_parse\_status()

HTTPリクエスト1行目（例：GET / HTTP/1.0）を解析する関数です．

HTTPリクエストの1行目に含まれるHTTPメソッドをcmd (例：GET)へ，URI情報をpath (例：/)へ，ステートマシンで抽出し **info** へ格納します．

```c
void exp1_parse_status(char* status, exp1_info_type *pinfo)
{
  char cmd[1024];
  char path[1024];
  char* pext;
  int i, j;

  enum state_type
  {
    SEARCH_CMD,
    SEARCH_PATH,
    SEARCH_END
  }state;

  state = SEARCH_CMD;
  j = 0;
  for(i = 0; i < strlen(status); i++){
    switch(state){
    case SEARCH_CMD:
      if(status[i] == ' '){
        cmd[j] = '\0';
        j = 0;
        state = SEARCH_PATH;
      }else{
        cmd[j] = status[i];
        j++;
      }
      break;
    case SEARCH_PATH:
      if(status[i] == ' '){
        path[j] = '\0';
        j = 0;
        state = SEARCH_END;
      }else{
        path[j] = status[i];
        j++;
      }
      break;
    }
  }

  strcpy(pinfo->cmd, cmd);
  strcpy(pinfo->path, path);
}
```

### exp1\_check\_file()

exp1\_parse\_status()で得られたinfo->pathから，リクエスト対象の実体へのパス情報へ変換する関数です．

ファイルやディレクトリ情報を取得可能なシステムコール [stat()](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/stat.2.html) を用いて，ファイルが存在するか，指定されたURI情報がディレクトリ名なのか調査し，HTTPリプライで返すべく適切な[HTTPステータスコード](https://ja.wikipedia.org/wiki/HTTP%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)を **info->code** へ格納しています．

また，リクエスト対象のファイルサイズを取得し，HTTPリプライで返すContent-Length用に **info->size** へ格納しておきます．

最後に，info->pathの拡張子を見て，HTTPリプライで返すContent-Type用に **info->type** へ適切な文字列を格納しておきます．

```c
void exp1_check_file(exp1_info_type *info)
{
  struct stat s;
  int ret;
  char* pext;

  sprintf(info->real_path, "html%s", info->path);
  ret = stat(info->real_path, &s);

  if((s.st_mode & S_IFMT) == S_IFDIR){
    sprintf(info->real_path, "%s/index.html", info->real_path);
  }

  ret = stat(info->real_path, &s);

  if(ret == -1){
    info->code = 404;
  }else{
    info->code = 200;
    info->size = (int) s.st_size;
  }

  pext = strstr(info->path, ".");
  if(pext != NULL && strcmp(pext, ".html") == 0){
    strcpy(info->type, "text/html");
  }else if(pext != NULL && strcmp(pext, ".jpg") == 0){
    strcpy(info->type, "image/jpeg");
  }
}
```

### exp1\_http\_reply()

exp1\_parse\_header()で抽出された **info** を元に，クライアントへ返すべく適切なHTTPリプライを作成し返します．

```c
void exp1_http_reply(int sock, exp1_info_type *info)
{
  char buf[16384];
  int len;
  int ret;

  if(info->code == 404){
    exp1_send_404(sock);
    printf("404 not found %s\n", info->path);
    return;
  }

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
  len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  exp1_send_file(sock, info->real_path);
}
```

### exp1\_send\_404()

info->pathで指定されたファイルがhtmlディレクトリに存在しなかった場合（**info->code** へエラーコード404が入っている場合），「404 Not Found」をクライアントへ返します．

このサンプルソースコードでは，ステータスコード404はクライアントへ返していますが，404ページ自体は返信してませんのでご注意ください（4XX系を実装する人は適宜作成してください）．

```c
void exp1_send_404(int sock)
{
  char buf[16384];
  int ret;

  sprintf(buf, "HTTP/1.0 404 Not Found\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}
```

### exp1\_send\_file()

info->real\_pathで指定されたファイルがhtmlディレクトリに存在した場合は，そのファイルをfopen()で開き，fread()で読み込める間，読み込めたデータをsend()でクライアントへ分割して返信し，読み込めなくなったらファイルをfclose()します．

```c
void exp1_send_file(int sock, char* filename)
{
  FILE *fp;
  int len;
  char buf[16384];

  fp = fopen(filename, "r");
  if(fp == NULL){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  len = fread(buf, sizeof(char), 16384, fp);
  while(len > 0){
    int ret = send(sock, buf, len, 0);
    if(ret < 0){
      shutdown(sock, SHUT_RDWR);
      close(sock);
      break;
    }
    len = fread(buf, sizeof(char), 1460, fp);
  }

  fclose(fp);
}
```

### exp1.h

基本的にこれまで提示したexp1.hと同じと思いますが，念のため再掲しておきます．

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <float.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
```

## 実行方法

-   各自のアカウントでログインし，作成したSimple HTTPサーバのソースコードのあるディレクトリへ移動しコンパイルしておいてください．
-   以下のコマンドを入力して，こちらで用意しておいたサンプルページをダウンロードし解凍しておきます．

```
 $ wget http://exp1.inf.shizuoka.ac.jp/images/3/31/day6.zip 
```

```
 $ unzip day6.zip 
```

-   先ほどコンパイルした各自のSimple HTTPサーバのプログラムを実行します．
-   Webブラウザで`http://192.168.1.101:xxxxx`(xxxxxは学籍番号の下5桁)と入力してEnterしてみてください．「It Works!」とWebブラウザ上に表示されたら成功です．
-   皆さんの作成したSimple HTTPサーバが正常に動作し，サンプルページとして用意した `index.html`を返してくれたことになります（`index.html`に何が書いてあるか実際に開いて確認してみてください）．
-   test.htmlというサンプルページも作成しておきましたので`http://192.168.1.101:xxxxx/test.html`も試してみてください．)

## おわりに

単純なHTTPのみを処理できるSimple HTTPサーバは，正常に動作しましたか？

次は，このSimple HTTPサーバの性能を評価するベンチマークプログラムを作成してみましょう．