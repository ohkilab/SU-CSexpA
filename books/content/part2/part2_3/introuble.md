# 第二部の実装で詰まった時の対処法
## はじめに

第二部の実装は，様々な難度のものがありますので以下も参考にしてみてください．

## ApacheやJettyの挙動と比べる

HTTPサーバ機能拡張の実装について，具体的にどのようになればよいのか世の中で使用されているWebサーバの動作を参考にするという方法があります．

例えば，[Apache](http://httpd.apache.org/)は商用でも利用される一般的なWebサーバーです．一方で，最先端のWeb技術がすぐに実装されるというわけではありません．

最先端のWeb技術が比較的早く実装されるWebサーバとして，[Jetty](http://www.eclipse.org/jetty/)が挙げられます．

プロトコル解析技術を用いて，正しい動作をApacheやJettyで確認してから，その通りに動作するプログラムを実装すると楽かもしれません．

ここで，Webサーバの挙動を分かりやすく解析するためには，接続するWebブラウザの設定における「同時接続数」を1にして分析した方が分かりやすいと思います（Webブラウザによって異なります）．

[http://setting-tool.net/browser-max-connections-per-server](http://setting-tool.net/browser-max-connections-per-server)

## 標準化文書にあたる

HTTPに関しては，IETFで全て標準化の文書が[RFC](https://www.ietf.org/rfc.html)として公開されています（一部は日本語版もあり）．

HTTP/1.1の場合，[RFC2616](https://tex2e.github.io/rfc-translater/html/rfc2616.html)として公開されています．

また，HTTP/2も，[RFC7540](https://tex2e.github.io/rfc-translater/html/rfc7540.html)として公開されています．

## 必要最低限のコードを書いて試してみる

例えば，POST実装で躓（つまづ）いたとします．まず，POSTが動作するフォームをtest.htmlとして作ってみます（POSTの部分をGETに変えるとGETで動作しますが，ファイルのアップロードは動作しません）．

```html
<form enctype="multipart/form-data" action="upload.php" method="POST">
  upload file: <input name="userfile" type="file" /><br>
  <input type="text" name="text1" value="value1" /><br>
  <input type="submit" value="send" />
</form>
```
次に，Webサーバー側へ`upload.php`としてPOSTを受け取るCGIを置きます．

```php
<?php

echo 'dumping $_POST<br>';
var_dump($_POST);
echo "<br>";
$uploaddir = './';
echo 'dumping $FILES<br>';
var_dump($_FILES);
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile);

?>
```

この時の動作をWiresharkやFireBugで確認して，POSTがどのような動作をしているか把握します．このように必要最低限のコードを実装して試してみるという方法もあります．

## POST実装について

例えば，以下のような`post.txt`があったとします．これを解析するプログラムを作る方法です．

### post.txt
```
POST /test/upload.php HTTP/1.1
Host: sarulab.inf.shizuoka.ac.jp
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: ja,en-US;q=0.7,en;q=0.3
Accept-Encoding: gzip, deflate
Referer: http://sarulab.inf.shizuoka.ac.jp/user/saru/test/test.html
Connection: keep-alive
Content-Type: multipart/form-data; boundary=---------------------------20124110774221
Content-Length: 306

-----------------------------20124110774221
Content-Disposition: form-data; name="userfile"; filename="sample.txt"
Content-Type: text/plain

hello world
-----------------------------20124110774221
Content-Disposition: form-data; name="text1"

value1
-----------------------------20124110774221--
```


HTTP/1.1の[RFC2616](https://triple-underscore.github.io/RFC2616-ja.html)を見ると，以下のようにheaderとbodyはCrLf×2で分離されていることが分かります．

```
5 リクエスト

   クライアントからサーバへのリクエストメッセージには，リソースに適用されるメソッド，リソースの識別子，使用するプロトコルのバージョンが最初の行に含まれる．

        Request       = Request-Line              ; Section 5.1
                        *(( general-header        ; Section 4.5
                         | request-header         ; Section 5.3
                         | entity-header ) CRLF)  ; Section 7.1
                        CRLF
                        [ message-body ]          ; Section 4.3

```

headerとbodyを分けるオートマトンは，以下のように描けます．

![day8_chart.jpg](https://www-int.ist.osaka-u.ac.jp/sarulab/user/saru/exp1/day8_chart.jpg)

このオートマトンの実装例を以下の`parse_header_body.c`に示します．

### parse\_header\_body.c

```c
#include "exp1.h"

int main()
{
  char buf[16384];
  char header[16384];
  char body[16384];
  FILE* fp = fopen("post.txt", "r");
  int size = 0;
  int ret;
  int i, j;

  enum {
    E_FIND_CR,
    E_FIND_CRLF,
    E_FIND_CRLFCR,
    E_FIND_CRLFCRLF,
    E_FIND_NULL
  } parse_status;

  ret = fread(buf, 1, sizeof(buf), fp);
  while(ret > 0){
    size += ret;
    ret = fread(buf + size, 1, sizeof(buf) - size, fp);
  }
  buf[size] = 0;
  fclose(fp);

  i = 0;
  j = 0;
  bzero(header, sizeof(header));
  bzero(body, sizeof(body));
  parse_status = E_FIND_CR;

  while(buf[i] != 0){
    switch(parse_status){
    case E_FIND_CR:
      header[j] = buf[i];
      j++;

      if(buf[i] == '\r'){
        parse_status = E_FIND_CRLF;
      }else{
        parse_status = E_FIND_CR;
      }
      break;
    case E_FIND_CRLF:
      header[j] = buf[i];
      j++;

      if(buf[i] == '\n'){
        parse_status = E_FIND_CRLFCR;
      }else{
        parse_status = E_FIND_CR;
      }
      break;
    case E_FIND_CRLFCR:
      header[j] = buf[i];
      j++;

      if(buf[i] == '\r'){
        parse_status = E_FIND_CRLFCRLF;
      }else{
        parse_status = E_FIND_CR;
      }
      break;
    case E_FIND_CRLFCRLF:
      header[j] = buf[i];
      j++;

      if(buf[i] == '\n'){
        parse_status = E_FIND_NULL;
        j = 0;
      }else{
        parse_status = E_FIND_CR;
      }

      break;
    case E_FIND_NULL:
      body[j] = buf[i];
      j++;
      break;
    default:
      printf("impossible\n");
      exit(-1);
    }
    i++;
  }

  printf("################ header is ##############\n");
  printf("%s", header);

  printf("################ body is ################\n");
  printf("%s", body);
}
```
次に，bodyの中の各変数を展開する必要があります．

headerで指定されている「boundary=---------------------------20124110774221」で切れば良さそうですね．

## C言語からPHPなどを呼び出すことはできるか？

[system()](http://linuxjm.osdn.jp/html/LDP_man-pages/man3/system.3.html)を用いることで，任意のプログラムやコマンドを呼び出すことができます．

例えば，以下の`system_sample.c`と`test.php`を作成して，`system_sample.c`をコンパイル後実行してみてください．

### system\_sample.c

```c
#include "exp1.h"

int main()
{
  system("php test.php");
}
```
### test.php
```php
<?php
echo "hello world\n";
?>
```

標準出力に「hello world」が出力されたと思います．次の課題は，この結果をどのようにネットワーク越しのリモート端末へ返送するかでしょうか．

system()で `php test.php > tmp.html` を実行させてから，`tmp.html`を読み込んでソケットで返すなど様々な実装方法がありそうですが，[dup2()](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/dup.2.html)を用いると楽と思います．以下に示す`dup2_sample.c`をコンパイルして実行してみてください．

### dup2\_sample.c

```c
#include "exp1.h"

int main()
{
  int fd = open("output.txt", O_WRONLY | O_CREAT);
  dup2(fd, 1);
  printf("hello world");
  close(fd);
}
```
printf()の結果が，output.txtへ出力されたはずです．つまり，dup2()によって標準出力をoutput.txtのディスクリプタへ割り当てたことになります．

ソケットに割り当てられたディスクリプタへ標準出力を割り当てれば実現できると思います（もちろん，一旦ファイルへ出力する方法でも構いません）．

## 参考書籍

最近の要点をしっかりまとめてある良書と思います

-   [Linuxネットワークプログラミングバイブル（今は廃版でKindle版しかないようですが）](https://www.amazon.co.jp/dp/B00O8GIL62)
-   [ハイパフォーマンス ブラウザネットワーキング ―ネットワークアプリケーションのためのパフォーマンス最適化](https://www.amazon.co.jp/dp/4873116767)

以下は古くなりましたがご参考まで（当時よく読んでいた良書）

-   [基礎からわかるTCP/IP ネットワーク実験プログラミング―Linux/FreeBSD対応](https://www.amazon.co.jp/dp/4274065847)
-   [基礎からわかるTCP/IP アナライザ作成とパケット解析―Linux/FreeBSD対応](http://www.amazon.co.jp/dp/4274065820)
-   [UNIXネットワークプログラミング〈Vol.1〉ネットワークAPI:ソケットとXTI](http://www.amazon.co.jp/dp/4894712059)
-   [HTTP詳説―作ってわかるHTTPプロトコルのすべて](http://www.amazon.co.jp/dp/4894710412)
-   [HTTPの教科書](http://www.amazon.co.jp/dp/479812625X)

