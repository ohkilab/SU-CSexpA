# ベンチマークプログラムの設計と実装

## はじめに

以下の動作を行うクライアントプログラムを開発します．

- 1接続あたりM回のEchoBack通信（1メッセージはサイズL Byte）を実施する
- 上記の「M回のEchoBack通信」をN並列で実行する

すなわち，サーバがN台のクライアントから同時に接続を受け，それぞれの接続の中でM回のEchoBack通信を行う状況を作り出します．

シングルスレッドでブロッキング関数を使ってサーバに接続した場合（具体的には send/recv 関数を使って送受信する場合），順番に送受信を行うことになりますので，ある時点ではサーバは1接続しか扱っていないことになります．これではサーバに負荷がかかりませんので，マルチスレッドを利用してN並列でサーバと接続可能なクライアントプログラムを実装します．

## サンプルコード

「M回のEchoBack通信を行う送受信ループ」をN並列（スレッド）で実行するベンチマークプログラムのサンプルを提供するので，それぞれの実験方法の必要に応じて修正等を行って利用してください．

- [tcpbenchmark サンプルコード](https://github.com/ohkilab/SU-CSexpA-day3-tcpbenchmark)

基本的な使い方は以下の通りですが，各自ソースコードリーディングを行い，何をどのように計測しているのかを把握してから利用するようにして下さい． また，スレッド数も1プロセスのリソース制限がありますので無制限に増やせる訳ではありません（必要に応じてマルチプロセスとマルチスレッドを組み合わせるなどして並列数を増やす拡張を行ってもよいでしょう）．

実行環境によってはこのベンチマークプログラムがPermissionError等で実行を拒否される場合があるようです． そういった場合は`sudo ./hogehoge`の様に**管理者権限で実行**をしてみて下さい．

```shell
$ tcpbenchmark 192.168.0.100 10000 3 5 2
```

- 第1引数: 接続先IPアドレス
- 第2引数: 接続先ポート番号
- 第3引数: 多重度の指定（スレッド数）
- 第4引数: メッセージサイズ
- 第5引数: メッセージ送信回数

このコマンド例では「192.168.0.185の10000番ポートに3スレッド同時アクセスを⾏い，各スレッドでは『5バイトのメッセージを送信し，その応答を受け取る』ことを2回実施する」ことになります．

以下のPDFにはもう少し詳細な説明が記載してあります．適時参照してください．

[TCPBenchMark.pdf](../../../resources/tcpbenchmark.pdf)

### 時間計測部分

POSIX標準ではナノ秒単位で時間を扱うことができるライブラリが提供されています．計測開示の時刻と計測終了時の時刻を構造体で管理し，差分を取って所要時間を表示しています．

```c
typedef struct __timeCounter {
  struct timespec startTs;
  struct timespec endTs;
} TIMECOUNTER;

void countStart(TIMECOUNTER* tc) {
  clock_gettime(CLOCK_MONOTONIC, &(tc->startTs));
}

void countEnd(TIMECOUNTER* tc) {
  clock_gettime(CLOCK_MONOTONIC, &(tc->endTs));
}

/**
 * amount required time
 */
double diffRealSec(TIMECOUNTER* tc) {
  time_t diffsec = difftime(tc->endTs.tv_sec, tc->startTs.tv_sec);
  long diffnanosec = tc->endTs.tv_nsec - tc->startTs.tv_nsec;
  return diffsec + diffnanosec*1e-9;
}

void printUsedTime(TIMECOUNTER* tc) {
  printf("UsedTime: %10.10f(sec)\n", diffRealSec(tc));
}
```

### スレッドの生成と計測部分

単位時間あたりの処理能力を計測する必要があるので，スレッド生成処理のタイムラグを除外するために，予め全てのスレッドを生成しておき，スタートフラグを以って全スレッドが並行してサーバに接続開始するように設計しています．

```c
  for (int i = 0; i < tplistNum; i++) {
    THREADPARAM *tp = (THREADPARAM *)malloc(sizeof(THREADPARAM));
    tp->id = i;
    strcpy(tp->serverInfo.hostName, hostName);
    strcpy(tp->serverInfo.portNum, portNum);
    tp->result = false;
    tp->failedConnectNum = 0;
    tp->failedSendRecvLoopNum = 0;
    tp->failedSendRecvNum = 0;
    tplist[i] = tp;

    if (pthread_create(&threadId[i], &attr, thread_func, tp)) {
      perror("pthread_create");
      return false;
    }
    fprintf(stderr, "thread %d created.\n", i);
  }

  // スレッドの準備終了
  isPrepared = true;
  fprintf(stderr, "start count down: 2\n");
  sleep(1);// 1 sec
  fprintf(stderr, "start count down: 1\n");
  sleep(1);// 1 sec

  // スレッドスタート及び計測開始
  countStart(tc);
  isRunnable = true;
  fprintf(stderr, "Thread Start!!\n");

  // スレッド終了待ち
  for (int i = 0; i < tplistNum; i++) {
    if (threadId[i] != -1 && pthread_join(threadId[i], NULL)) {
      perror("pthread_join");

      // 所要時間計測（ベンチマーク全体の所要時間（接続・転送失敗の通信も含む））
      countEnd(tc);
      return false;
    }
  }
  // 所要時間計測（ベンチマーク全体の所要時間（接続・転送失敗の通信も含む））
  countEnd(tc);
  return true;
```

### 接続時間と送受信時間の計測

各スレッドにおける接続時間と送受信時間を計測しています。

```c
  while (tp->result == false && tp->failConnectNum < RECONNECT_MAX ) {

    // 接続時間計測開始
    countStart(&(tp->connectTime));

    // サーバにソケット接続
    if ((csocket = clientTCPSocket(si->hostName, si->portNum)) < 0) {
      fprintf(stderr, "client_socket():error\n");
      tp->result = false;
      tp->failConnectNum++;

      // 接続時間計測終了
      countEnd(&(tp->connectTime));

      if ( csocket == -2 ) {
        // connection refused ( maybe server down )
        fprintf(stderr, "maybe server down.\n");
        break;
      }
      continue;
    }

    // 接続時間計測終了
    countEnd(&(tp->connectTime));

    // 送受信時間計測開始
    countStart(&(tp->sendRecvTime));

    // 送受信処理
    int successCount = sendRecvLoop(csocket, msg, strlen(msg), ECHOBACKNUM, tp->id, RESPONSE_POSTFIX);
    if (successCount == ECHOBACKNUM) {
      tp->result = true;
    } else {
      tp->failedSendRecvNum += (ECHOBACKNUM - successCount);
      tp->failedSendRecvLoopNum++;
    }

    // 送受信時間計測終了
    countEnd(&(tp->sendRecvTime));

    // ソケットクローズ
    close(csocket);
  }
```
