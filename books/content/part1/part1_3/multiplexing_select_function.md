# select 関数による多重化 - 情報科学実験I

[TOC]

## はじめに

select関数による同期I/Oの多重化を利用してサーバプログラムを多重化します．

以下にEchoBackのプログラムを題材として，サーバ側プログラムのアクセプト周辺に関してサンプルコードを示します（サンプルコード内で呼び出しているechoBack関数（前回までのEchoBack通信部分のコードと大差ないので内容は未掲載）が実際に通信を行う部分になりますので，その関数を適切なタイミングで適切なソケットと共に実行することが各サンプルコードの主眼になります）．

サーバ側プログラム全体は示しませんので、前回のサンプルコードを改良して複数クライアントに対応するサーバプログラムを完成させてください．

## サンプルコード

あるクライアントとの間のEchoBack通信（クライアントからメッセージを受け取り，そのメッセージ（あるいはメッセージを加工したデータ）をクライアントに返送する通信）を1つの単位として処理します．

このサーバは以下の2種類の通信に適切な応答を行う必要があります．

-   新規のクライアントからの接続
-   既存のクライアントからのEchoBack要求

新規のクライアントからの接続を待つ間に，既存のクライアントからのEchoBack要求が届くこともありますし，既存のクライアントからのEchoBack要求を待つ間に新規のクライアントからの接続要求や，別のクライアントからのEchoBack要求が届くこともあります．  
従って，特定の通信を期待した読み込み待ちでプログラムをブロックすると他の通信に対応できないことになります．

select関数はOSのシステムコールを活用することで複数のソケットを監視し，任意のソケットが読み込み可能な状態となった時点でその情報を返します．  
その後，読み込み可能状態になったソケットのみを対象としてブロッキングI/O関数を利用した受信待ちを行ったとしても実質的に待ちなしでデータを受信することができます（読み込み待ちを行った時点で既に読み込み可能状態になっているため）．

以下のサンプルコードの概要としては以下の処理を行っています．

1\. select関数に引数として与える監視するソケットの情報を生成する

-   サーバソケット
-   クライアント管理配列に登録されたソケット（既存のクライアントとの通信用ソケット）

2\. select関数を実行してソケットを監視する

3-1. 読み込み可能となったソケットが新規のクライアントからの接続の場合，accept 関数で新規のクライアントからの接続を受け入れ，当該クライアントとの通信用のソケットを生成する．生成したソケットをクライアント管理配列に登録する．

3-2. 読み込み可能となったソケットが既存のクライアントからのEchoBack要求の場合，EchoBack通信を行う．

```c
#define MAXCHILD 1200
void acceptLoop(int sock) {

  // クライアント管理配列
  int childNum = 0;
  int child[MAXCHILD];
  int i = 0;
  for (i = 0; i < MAXCHILD; i++) {
    child[i] = -1;
  }

  while (true) {

    // select用マスクの初期化
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(sock, &mask);  // ソケットの設定
    int width = sock + 1;
    int i = 0;
    for (i = 0; i < childNum; i++) {
      if (child[i] != -1) {
        FD_SET(child[i], &mask);  // クライアントソケットの設定
        if ( width <= child[i] ) {
          width = child[i] + 1;
        }
      }
    }

    // マスクを設定
    fd_set ready = mask;

    // タイムアウト値のセット
    struct timeval timeout;
    timeout.tv_sec = 600;
    timeout.tv_usec = 0;

    switch (select(width, (fd_set *) &ready, NULL, NULL, &timeout)) {
    case -1:
      // エラー処理
      perror("select");
      break;
    case 0:
      // タイムアウト
      break;
    default:
      // I/Oレディあり

      if (FD_ISSET(sock, &ready)) {
        // サーバソケットレディの場合
        struct sockaddr_storage from;
        socklen_t len = sizeof(from);
        int acc = 0;
        if ((acc = accept(sock, (struct sockaddr *) &from, &len))
            == -1) {
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

          // クライアント管理配列に登録
          int pos = -1;
          int i = 0;
          for (i = 0; i < childNum; i++) {
            if (child[i] == -1) {
              pos = i;
              break;
            }
          }

          if (pos == -1) {
            if (childNum >= MAXCHILD) {
              // 並列数が上限に達している場合は切断する
              fprintf(stderr, "child is full.\n");
              close(acc);
            } else {
              pos = childNum;
              childNum = childNum + 1;
            }
          }

          if (pos != -1) {
            child[pos] = acc;
          }
        }
      }

      // アクセプトしたソケットがレディの場合を確認する
      int i = 0;
      for (i = 0; i < childNum; i++) {
        if (child[i] != -1 && FD_ISSET(child[i], &ready)) {
          // クライアントとの通信処理
          // エコーバックを行う（echoBack関数は自分で作成すること）
          if ( echoBack(child[i]) == false ) {
            close(child[i]);
            child[i] = -1;
          }
        }
      }
    }
  }
}
```

## 参考

-   \[ネットワークプログラミング2 - 多重I/O（リンクは \\\\fs.inf.in.shizuoka.ac.jp\\share\\class\\NetworkProgramming）にあります\]