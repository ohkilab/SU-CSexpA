# マルチプロセスによる多重化 - 情報科学実験I

[TOC]

## はじめに

マルチプロセスを用いてサーバプログラムを多重化します．  

以下にEchoBackのプログラムを題材としたサーバ側プログラムのアクセプト周辺のサンプルコードを示します（サンプルコード内で呼び出しているechoBackLoop関数（前回までのEchoBack通信部分のコードと大差ないので内容は未掲載）が実際に通信を行う部分になりますので，その関数に適切なプロセスを割り当て，適切なソケットを与えて実行することが各サンプルコードの主眼になります）．  

サーバ側プログラム全体は示しませんので、前回のサンプルコードを改良して複数クライアントに対応するサーバプログラムを完成させてください．  

## サンプルコード

あるクライアントとの間のEchoBack通信（クライアントからメッセージを受け取り，そのメッセージ（あるいはメッセージを加工したデータ）をクライアントに返送する通信）の繰り返しを個別のプロセスに担当させます．  
このプロセスはクライアントから切断されるまで存続します．  

当該クライアントとの通信が終了した場合には自身でプロセスを終了します．  

子プロセスのリソースはシグナルハンドラによって回収しますが，シグナルハンドラによる回収がうまくいかない場合には親プロセスによって回収します．

```c
void acceptLoop(int sock) {

  // 子プロセス終了時のシグナルハンドラを指定
  struct sigaction sa;
  sigaction(SIGCHLD, NULL, &sa);
  sa.sa_handler = sigChldHandler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGCHLD, &sa, NULL);

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
      getnameinfo((struct sockaddr *) &from, len, hbuf, sizeof(hbuf),
          sbuf, sizeof(sbuf),
          NI_NUMERICHOST | NI_NUMERICSERV);
      fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);

      // プロセス生成
      pid_t pid = fork();
      if (pid == -1) {
        // エラー処理
        perror("fork");
        close(acc);
      } else if (pid == 0) {
        // 子プロセス
        close(sock);  // サーバソケットクローズ
        echoBackLoop(acc);
        close(acc);    // アクセプトソケットクローズ
        acc = -1;
        _exit(1);
      } else {
        // 親プロセス
        close(acc);    // アクセプトソケットクローズ
        acc = -1;
      }

      // 子プロセスの終了処理
      int status = -1;
      while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // 終了した(かつSIGCHLDでキャッチできなかった)子プロセスが存在する場合
        // WNOHANGを指定してあるのでノンブロッキング処理
        // 各子プロセス終了時に確実に回収しなくても新規クライアント接続時に回収できれば十分なため．
        printChildProcessStatus(pid, status);
      }
    }
  }
}

// シグナルハンドラによって子プロセスのリソースを回収する
void sigChldHandler(int sig) {
  // 子プロセスの終了を待つ
  int status = -1;
  pid_t pid = wait(&status);

  // 非同期シグナルハンドラ内でfprintfを使うことは好ましくないが，
  // ここではプロセスの状態表示に簡単のため使うことにする
  printChildProcessStatus(pid, status);
}

void printChildProcessStatus(pid_t pid, int status) {
  fprintf(stderr, "sig_chld_handler:wait:pid=%d,status=%d\n", pid, status);
  fprintf(stderr, "  WIFEXITED:%d,WEXITSTATUS:%d,WIFSIGNALED:%d,"
      "WTERMSIG:%d,WIFSTOPPED:%d,WSTOPSIG:%d\n", WIFEXITED(status),
      WEXITSTATUS(status), WIFSIGNALED(status), WTERMSIG(status),
      WIFSTOPPED(status),
      WSTOPSIG(status));
}
```

## 参考

-   \[ネットワークプログラミング4 - マルチプロセス（リンクは \\\\fs.inf.in.shizuoka.ac.jp\\share\\class\\NetworkProgramming）にあります\]