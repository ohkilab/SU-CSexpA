# 非同期IOによる多重化

## はじめに

ノンブロッキング非同期I/Oライブラリを用いてサーバプログラムを多重化し，他の多重化方法との性能比較を実施せよ．
本項の内容は以下の三つから構成されます。

- libeventによる多重化
- libuvによる多重化
- epollによる多重化


ノンブロッキング非同期I/Oライブラリを利用する場合，通常コールバック関数を利用した設計になりますのでこれまでのサンプルコードとは構造が異なりますので注意してください．

ノンブロッキング非同期I/Oライブラリはいくつか公開されていますが，まずは以下の2種類を比較してみてください． 余力があれば，別のライブラリを利用した場合や，epoll, kqueueなどのシステムコールを直接扱う場合を比較しても良いでしょう．

-   libevent ([https://libevent.org](https://libevent.org/))
-   libuv ([https://libuv.org](https://libuv.org/))

最初に利用するライブラリをインストールしておきます．

```shell
$ sudo apt install libevent-dev
$ sudo apt install libuv1 libuv1-dev
```

# libeventによる多重化

## はじめに

最初に利用するライブラリをインストールしておきます．

```shell
$ sudo apt install libevent-dev
```

## サンプルコード

各ソケットがアクセス可能状態になった場合の処理をコールバック関数として実装します．
異なるソケットに対応する処理（このサンプルコードの場合，初回接続時，接続後の各クライアントとの通信時）を1つの大きなループの中で条件分岐で記述するのではなく，別のコールバック関数で記述することができます．
また，OS依存の関数呼び出し（epoll や kqueue など）をライブラリ関数内に隠蔽することでコードの移植性を保っています．

-   [tcpserverle](https://exp1.inf.shizuoka.ac.jp/shizudai-only/day4/tcpserverle.tgz)(学内からのみアクセス可能)

### in TCPServerLE.c

```c
// SIGINTイベントハンドラ
void sigintHandler(evutil_socket_t fd, short ev, void* arg) {
  // イベントループを終了する
  struct event_base* base = (struct event_base*)arg;
  event_base_loopexit(base, NULL);
}

bool start(const char* portNum) {

  // libeventの初期化
  struct event_base* base = event_base_new();

  // SIGINTのイベントハンドラを登録
  struct event* sig;
  sig = evsignal_new(base, SIGINT, sigintHandler, base);
  if ( evsignal_add(sig, NULL) != 0 ) {
    perror("evsignal_add: sig");
    return false;
  }

  int ssocket = serverTCPSocket(portNum);
  if (ssocket == -1) {
    fprintf(stderr, "server_socket(%s):error\n", portNum);
    return false;
  }
  fprintf(stderr, "ready for accept\n");

  // libeventにサーバソケットを登録
  struct event ev;
  event_assign(&ev, base, ssocket, EV_READ|EV_PERSIST, acceptHandler, base);
  if ( event_add(&ev, NULL) != 0 ) {
    perror("event_add: ev");
    close(ssocket);
    return false;
  }

  // イベント処理実行
  event_base_dispatch(base);

  fprintf(stderr, "evnet_dispatch() -> end\n");

  // リソース開放
  event_del(&ev);
  event_del(sig);
  free(sig);
  event_base_free(base);

  close(ssocket);
  return true;
}
```

### in libserver.c

```c
void acceptHandler(evutil_socket_t sock, short event, void* arg) {

  struct event_base* base = (struct event_base*)arg;

  if (event & EV_READ) {
    // サーバソケットレディの場合
    struct sockaddr_storage from;
    socklen_t len = sizeof(from);
    int acc = 0;
    if ((acc = accept(sock, (struct sockaddr *) &from, &len)) == -1) {
      // エラー処理
      if (errno != EINTR) {
        perror("accept");
        return;
      }
    } else {
      // クライアントからの接続が行われた場合
      char hbuf[NI_MAXHOST];
      char sbuf[NI_MAXSERV];
      getnameinfo((struct sockaddr *) &from, len, hbuf, sizeof(hbuf),
          sbuf, sizeof(sbuf),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);
    }

    // 接続済ソケットが読み込み可能になった場合のイベントを登録する
    // この関数が終了した後も利用する（イベントがコールされた時）ためヒープ領域にイベント構造体を確保する
    struct event* ev = (struct event*)malloc(sizeof(struct event));
    if ( ev == NULL ) {
      perror("malloc: ev");
      close(acc);
      return;
    }
    event_assign(ev, base, acc, EV_READ | EV_PERSIST, echobackHandler, ev);
    if (event_add(ev, NULL) != 0) {
      perror("ev_add: ev");
      return;
    }
  }
}

void echobackHandler(evutil_socket_t acc, short event, void* arg) {

  // 引数からイベント構造体を受け取る
  // このハンドラは何回も呼び出されるためこの段階で値をコピーして解放してはいけない
  struct event* ev = (struct event*)arg;

  if (event & EV_READ) {
    char buf[512];

    // ソケットから入力を受け取る
    ssize_t len = 0;
    if ((len = recv(acc, buf, sizeof(buf), 0)) == -1) {
      perror("recv");
      return;
    }

    if (len == 0) {
      // クライアント側から切断（正常に切断）
      fprintf(stderr, "recv:EOF\n");

      // イベント登録を解除し，確保したリソースを開放する
      event_del(ev);
      free(ev);
      close(acc);
      return;
    }

    // 改行コードを行末に差し替える
    buf[len] = '\0';
    char* retPtr = NULL;
    if ((retPtr = strpbrk(buf, "\r\n")) != NULL) {
      *retPtr = '\0';
    }

    // 入力された内容に ":OK" を付与して送信する
    fprintf(stderr, "[client]%s\n", buf);  // コンソールに出力
    strncat(buf, ":OK\r\n", sizeof(buf) - strlen(buf) - 1);
    len = strlen(buf);
    if ((len = send(acc, buf, len, 0)) == -1) {    // クライアントに送信
      perror("send");

      // イベント登録を解除し，確保したリソースを開放する
      event_del(ev);
      free(ev);
      close(acc);
      return;
    }
  }
}
```

# libuvによる多重化

## はじめに

最初に利用するライブラリをインストールしておきます．

```shell
$ sudo apt install libuv1 libuv1-dev
```

## サンプルコード

TCP/IP通信に必要となる各処理に対するユーザプログラム独自の処理をコールバックで実現することで，ユーザプログラム側で待ち状態となることを減らします．TCP/IP通信に必要となる各処理（bind, listen, accept, get\_sockname, read, write, close, ...）が libuv のライブラリ関数で置き換えられており，通信相手からの通信受信時の処理をコールバック関数で実現しています．

-   [tcpserveruv](https://exp1.inf.shizuoka.ac.jp/shizudai-only/day4/tcpserveruv.tgz)(学内からのみアクセス可能)

### in TCPServerUV.c

```c
struct sockaddr_in addr;
#define DEFAULT_BACKLOG 128

bool start(const int portNum) {

  loop = uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  uv_ip4_addr("0.0.0.0", portNum, &addr);

  uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
  int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
  if (r) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    return 1;
  }
  return uv_run(loop, UV_RUN_DEFAULT);
}
```

### in libserver.h

```c
void on_new_connection(uv_stream_t *server, int status)
{
  if (status < 0)
  {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    // error!
    return;
  }

  uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t *)client) == 0)
  {
    struct sockaddr_storage from;
    int len;
    uv_tcp_getsockname(client, (struct sockaddr *) &from, &len);
    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    getnameinfo((struct sockaddr *) &from, len, hbuf,
        sizeof(hbuf), sbuf, sizeof(sbuf),
        NI_NUMERICHOST | NI_NUMERICSERV);
    fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);
    uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
  }
  else
  {
    uv_close((uv_handle_t *)client, on_close);
  }
}
```

# epollによる多重化

## はじめに

Linux の epoll API を直接利用した場合について，必要なライブラリAPI等の調査・実装を行って計測対象に加えてみてください． 特に libevent, libuv 等のライブラリ関数を利用した場合との性能及び移植性の差異を比較して考察してみてください．
