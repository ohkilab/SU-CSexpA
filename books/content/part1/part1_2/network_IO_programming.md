# ネットワーク入出力プログラミング

## はじめに

この課題の目的はファイル入出力プログラミングの知識と経験を前提に，ネットワークプログラミングの理解を深めることです．
Raspberry Piと各自のノートPC上に構築したLinux環境を利用してサンプルプログラムをビルドして通信を行います．

ネットワークを利用したプログラムは，ネットワークを対象に入出力を行います．
サーバプログラムとクライアントプログラム間で通信を行いますが，互いに通信を行う前にそれぞれのプログラムを個別に動作確認することが大事です．
ファイル入出力においてファイルの内容を確認して入出力の結果を確認したように，ネットワークを対象に入出力の結果を確認してみましょう．
その際，パケットキャプチャのツール([Wireshark](https://www.wireshark.org/))が役に立ちます．

### 参考情報

-   「Wiresharkの使い方」: みやたひろし. "パケットキャプチャの教科書", SBクリエイティブ, p.19, 2017.
-   「パケットキャプチャデータのフィルタに関わる機能」: みやたひろし. "パケットキャプチャの教科書", SBクリエイティブ, p.27, 2017.
-   「TCP(Transmission Controcl Protocol)」: みやたひろし. "パケットキャプチャの教科書", SBクリエイティブ, p.185, 2017.

## 共有ライブラリ

TCP接続を行うサーバ側・クライアント側で，共通の処理を関数化したソースコードを以下に示します．
これらの関数は共有ライブラリファイル`exp1lib.h`, `exp1lib.c`内に記述し，各サンプルコードから利用するようにしています．

### exp1\_tcp\_listen

TCP接続を行うためのサーバ側ライブラリです（情報科学実験A用の共有ライブラリ）．

プログラムの各行が何を行っているのか，コードリーディングして確認してください．

```c
int exp1_tcp_listen(int port) {
	int sock;
	struct sockaddr_in addr;
	int yes = 1;
	int ret;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	ret = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) {
		perror("bind");
		exit(1);
	}

	ret = listen(sock, 5);
	if (ret < 0 {
		perror("reader: listen");
		close(sock);
		exit(-1);
	}

	return sock;
}
```

### exp1\_tcp\_connect

TCP接続を行うためのクライアント側ライブラリです（情報科学実験A用の共有ライブラリ）．

プログラムの各行が何を行っているのか，コードリーディングして確認してください．

```c
int exp1_tcp_connect(const char *hostname, int port) {
	int sock;
	int ret;
	struct sockaddr_in addr;
	struct hostent *host;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	host = gethostbyname(hostname);
	addr.sin_addr = *(struct in_addr *) (host->h_addr_list[0]);
	addr.sin_port = htons(port);

	ret = connect(sock, (struct sockaddr *) &addr, sizeof addr);
	if (ret < 0) {
		return -1;
	} else {
		return sock;
	}
}
```

## 共有ライブラリのビルド

Makefileの内容はサンプルコードによって異なりますが，典型的には以下のような記述になっています．

```makefile
 CC = gcc
 C++ = g++
 LD = g++
 CFLAGS = -c -Wall -pedantic-errors -O0 -g3 -std=gnu11 -I../
 LDFLAGS =
 OBJECTS = 010server.o ../exp1lib.o
 EXECUTABLE = 010server

 all: $(EXECUTABLE)

 $(EXECUTABLE): $(OBJECTS)
         $(LD) $(LDFLAGS) $(OBJECTS) -o $@

 .cpp.o:
         $(C++) $(CFLAGS) $< -o $@

 .c.o:
         $(CC) $(CFLAGS) $< -o $@

 clean:
         -rm -f ${EXECUTABLE} *.o core tmp.txt
```

共有ライブラリを各サンプルコードで共有するために別ディレクトリ（各サンプルコードの1つ上のディレクトリ）に配置しています．
そこで，以下のようにビルド対象に指定し，実行ファイルを作成する際にリンクするようにしています．

```makefile
 OBJECTS = 010server.o ../exp1lib.o
```

## \[必須課題7\] read，writeを用いて入力文字列を送信

### サーバ側ソースコードの一部: 007server.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock_listen;
  int sock_client;
  struct sockaddr addr;
  int len = 0;
  int ret = 0;
  char buf[1024];

  sock_listen = exp1_tcp_listen(11111);
  sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
  ret = read(sock_client, buf, 1024);
  write(1, buf, ret);
  close(sock_client);
  close(sock_listen);

  return 0;
}
```

### クライアント側ソースコードの一部: 007client.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock;
  char* p;
  char buf[1024];

  if(argc != 2) {
    printf("usage: %s [ip address]\n", argv[0]);
    exit(-1);
  }

  sock = exp1_tcp_connect(argv[1], 11111);
  p = fgets(buf, 1024, stdin);
  write(sock, p, strlen(p));
  close(sock);

  return 0;
}
```

### サーバ側実行方法

以下のように実行するだけです．

```shell
$ ./007server
```

クライアントから文字列を受け取ると標準出力へ受信した文字列を出力して終了します．

### クライアント側実行方法

サーバのIPアドレスを指定して実行します．

```{hint}
サーバー側のIPアドレスを確認する方法:`ip a`
```

以下のように入力します．

1行目を入力すると入力待ち受け状態になるので，文字列を入力してEnterを押すとサーバ側に文字列が表示されます．

下の例では「hello world」と入力しています．

```shell
$ ./007client {あなたのサーバのIPアドレス}
hello world
```

## \[必須課題8\] 一般的な関数を用いて入力文字列を送信

### サーバ側ソースコードの一部: 008server.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock_listen;
  int sock_client;
  struct sockaddr addr;
  int len = 0;
  int ret = 0;
  char buf[1024];

  sock_listen = exp1_tcp_listen(11111);
  sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
  ret = recv(sock_client, buf, 1024, 0);
  write(1, buf, ret);
  close(sock_client);
  close(sock_listen);

  return 0;
}
```

必須課題7とほぼ同じソースコードで実現できます．

### クライアント側ソースコードの一部: 008client.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock;
  char* p;
  char buf[1024];

  if(argc != 2) {
    printf("usage: %s [ip address]\n", argv[0]);
    exit(-1);
  }

  sock = exp1_tcp_connect(argv[1], 11111);
  p = fgets(buf, 1024, stdin);
  send(sock, p, strlen(p), 0);
  close(sock);

  return 0;
}
```

必須課題7とほぼ同じソースコードで実現できます．

### サーバ側実行方法

```shell
$ ./008server
```

クライアントから文字列を受け取ると標準出力に出力して終了します．

### クライアント側実行方法

サーバのIPアドレスを指定して実行します．

以下のように入力します．

1行目を入力すると入力待ち受け状態になるので，文字列を入力してEnterを押すとサーバ側に文字列が表示されます．

下の例では「hello world」と入力しています．

```shell
$ ./008client {あなたのサーバのIPアドレス}
hello world
```

## \[必須課題9\] ファイル送受信

### サーバ側ソースコードの一部: 009server.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock_listen;
  int sock_client;
  struct sockaddr addr;
  int len = 0;
  int ret = 0;
  char buf[1024];
  FILE* fp;

  sock_listen = exp1_tcp_listen(11111);
  sock_client = accept(sock_listen, &addr, (socklen_t*) &len);

  fp = fopen("tmp.txt", "w");
  ret = recv(sock_client, buf, 1024, 0);
  while (ret > 0) {
    fwrite(buf, sizeof(char), ret, fp);
    ret = recv(sock_client, buf, 1024, 0);
  }
  close(sock_client);
  close(sock_listen);

  return 0;
}

```

### クライアント側ソースコードの一部: 009client.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock;
  char buf[1024];
  FILE* fp;
  int ret;

  if(argc != 3) {
    printf("usage: %s [ip address] [filename]\n", argv[0]);
    exit(-1);
  }

  sock = exp1_tcp_connect(argv[1], 11111);
  fp = fopen(argv[2], "r");
  ret = fread(buf, sizeof(char), 1024, fp);
  while(ret > 0) {
    send(sock, buf, ret, 0);
    ret = fread(buf, sizeof(char), 1024, fp);
  }
  close(sock);

  return 0;
}
```

### サーバ側実行方法

```shell
$ ./009server
```

クライアントからファイルを受け取ると`tmp.txt`に出力して終了します．

### クライアント側実行方法

サーバのIPアドレスと送信したいファイル名を指定して実行します．

以下のように入力します．

サーバー側の「`tmp.txt`」にファイルがコピーされれば正常動作です．

```shell
$ ./009client {あなたのサーバのIPアドレス} ../001/001.c
```

## \[必須課題10\] echo back

### サーバ側ソースコードの一部: 010server.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock_listen;
  int sock_client;
  struct sockaddr addr;
  int len = 0;
  int ret = 0;
  char buf[1024];

  sock_listen = exp1_tcp_listen(11111);
  sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
  ret = recv(sock_client, buf, 1024, 0);
  write(1, buf, ret);
  send(sock_client, buf, ret, 0);
  close(sock_client);
  close(sock_listen);

  return 0;
}
```

### クライアント側ソースコードの一部: 010client.c

```c
#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
  int sock;
  char* p;
  char buf[1024];
  int ret;

  if(argc != 2) {
    printf("usage: %s [ip address]\n", argv[0]);
    exit(-1);
  }

  sock = exp1_tcp_connect(argv[1], 11111);
  p = fgets(buf, 1024, stdin);
  send(sock, p, strlen(p), 0);
  ret = recv(sock, buf, sizeof(buf), 0);
  write(1, buf, ret);
  close(sock);

  return 0;
}
```

### サーバ側実行方法

```shell
$ ./010server
```

クライアントから文字列を受け取ると標準出力に出力して終了します．

### クライアント側実行方法

サーバのIPアドレスを指定して実行します．

以下のように入力します．

1行目を入力すると入力待ち受け状態になるので，文字列を入力してEnterを押すとサーバ側に文字列が表示されます．

さらに入力した文字がエコーバックされてclientのところに表示されます．

```shell
$ ./010client {あなたのサーバのIPアドレス}
hello world
hello world
```
