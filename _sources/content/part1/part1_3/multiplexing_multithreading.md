# マルチスレッドによる多重化 - 情報科学実験I

[TOC]

## はじめに

マルチスレッドを用いてサーバプログラムを多重化します．  

以下にEchoBackのプログラムを題材として，サーバ側プログラムのアクセプト周辺のサンプルコードを示します（サンプルコード内で呼び出しているechoBackLoop関数（前回までのEchoBack通信部分のコードと大差ないので内容は未掲載）が実際に通信を行う部分になりますので，その関数に適切なスレッドを割り当て，適切なソケットと共に実行することが各サンプルコードの主眼になります）．  

サーバ側プログラム全体は示しませんので、前回のサンプルコードを改良して複数クライアントに対応するサーバプログラムを完成させてください．  
以下のサンプルコードは pthread ライブラリを利用していますので，ビルド時に -lpthread オプションを指定する必要があります．

## サンプルコード

あるクライアントとの間のEchoBack通信（クライアントからメッセージを受け取り，そのメッセージ（あるいはメッセージを加工したデータ）をクライアントに返送する通信）の繰り返しを1つのスレッドとして処理します．  

このスレッドはクライアントから切断されるまで存続します．  

当該クライアントとの通信が終了した場合には自身でスレッドを終了します（スレッドのリソースもスレッド自身で終了処理します）．

```c
// thread function
void* thread_func(void *arg) {

 int* accP = (int*)arg;
 int acc = *accP;
 free(accP);

 pthread_detach(pthread_self());

 // echoBackLoop関数は自分で作成すること
 echoBackLoop(acc);
 close(acc);

 pthread_exit(NULL);
}
```

クライアントから新規の接続があった場合に新規にスレッドを作成して，そのクライアントとの間の通信を作成したスレッドに任せます．

```c
void acceptLoop(int sock) {
 while (true) {
  struct sockaddr_storage from;
  socklen_t len = sizeof(from);
  int acc = 0;
  if ((acc = accept(sock, (struct sockaddr *) &from, &len)) == -1) {
   // エラー処理
   if (errno != EINTR) {
    perror("accept");
   }
  } else {
   // クライアントからの接続が行われた場合
   char hbuf[NI_MAXHOST];
   char sbuf[NI_MAXSERV];
   getnameinfo((struct sockaddr *) &from, len, hbuf,
     sizeof(hbuf), sbuf, sizeof(sbuf),
     NI_NUMERICHOST | NI_NUMERICSERV);
   fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);

   // スレッド生成
   int* param = (int*)malloc(sizeof(int));
   *param = acc;
   pthread_t th;
   if ( pthread_create(&th, NULL, thread_func, param) != 0 ) {
    perror("pthread_create");
    close(acc); // 切断
   }
  }
 }
}
```

## 参考

-   \[ネットワークプログラミング3 - マルチスレッド（リンクは \\\\fs.inf.in.shizuoka.ac.jp\\share\\class\\NetworkProgramming）にあります\]