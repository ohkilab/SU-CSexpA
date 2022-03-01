# libuvによる多重化 - 情報科学実験I

[TOC]

## はじめに

最初に利用するライブラリをインストールしておきます．

```shell
$ sudo apt install libuv1 libuv1-dev
```

## サンプルコード

TCP/IP通信に必要となる各処理に対するユーザプログラム独自の処理をコールバックで実現することで，ユーザプログラム側で待ち状態となることを減らします．TCP/IP通信に必要となる各処理（bind, listen, accept, get\_sockname, read, write, close, ...）が libuv のライブラリ関数で置き換えられており，通信相手からの通信受信時の処理をコールバック関数で実現しています．

-   [tcpserveruv](https://exp1.inf.shizuoka.ac.jp/shizudai-only/day4/tcpserveruv.tgz)

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