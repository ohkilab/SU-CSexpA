# HTTPベンチマークプログラムの実装
## はじめに

ネットワークプログラムは，通信を行う相手が必ず存在します．

そのため，ネットワークプログラムを自作する際には，必ず接続して動作確認をする必要があります．

今回のようにHTTPサーバを自作して実験する場合は，Webブラウザを使えばよいですし，メールクライアントプログラム（メーラ）を作成する場合は，既存のSMTPサーバやPOP3サーバ，IMAPサーバへ接続して動作確認できます．

ここでは，Simple HTTPサーバの性能を確認するためのベンチマークプログラムを実装します．今後の拡張機能実装にも関係しますので，スレッドの使い方も理解してください．

以下に示すソースコードを実装し，各自でHTTPベンチマークプログラムを実行してみてください．

ソースコードの簡単な解説も載せましたが，よく分からない部分はグループ内で相談し十分理解し合うようにしてください．

## ソースコード解説

　[スレッド](https://ja.wikipedia.org/wiki/%E3%82%B9%E3%83%AC%E3%83%83%E3%83%89_(%E3%82%B3%E3%83%B3%E3%83%94%E3%83%A5%E3%83%BC%E3%82%BF))と[mutex](https://ja.wikipedia.org/wiki/%E3%83%9F%E3%83%A5%E3%83%BC%E3%83%86%E3%83%83%E3%82%AF%E3%82%B9)による排他制御を利用して HTTPベンチマークプログラムを実装します（簡易的にWikipediaへリンク貼りましたが，適切な書籍やホームページを参照してしっかり使い方を復習しておきましょう）．

### main()

まずはmain()関数から概説します．

例えば，コンパイルして実行形式のファイル名を`bench.exe`としたら`$ ./bench 192.168.1.xxx 15`というように実行すると，192.168.1.xxxのIPアドレスへ15回HTTPリクエストを送信し，全てのHTTPリプライが返ってくるまでの時間とセッションエラー率を表示してくれます．

少し長いですが，やっていることは単純です（実装を容易にするためいくつかグローバル変数を使用してますのでご注意ください）．

-   main(int argc, char\*\* argv)でコマンドライン引数を取得（引数の数が誤っていたら使用方法を表示）
-   clock\_gettime()で開始時刻を記録
-   コマンドライン引数で与えられた回数分だけ，pthread\_create()でHTTPセッションを生成するスレッドexp1\_eval\_thread()を作成
-   その前に，0-99までの乱数（0-99の値をランダムに並べた配列）を生成し，pfileidへ格納（生成されたスレッドが要求する画像のIDを渡すため）
-   各HTTPセッションが終了するのをpthread\_join()で待機（pthread\_create()で作ったスレッドは，detach()しない限りはjoin()しないとスタックなどのリソースを解放しないため，join()を忘れるとメモリリークを起こすので注意）
-   clock\_gettime()で終了時刻を記録し，差分を表示

```c
 void randamize_array(int arr[], int size) {
   srand(time(NULL));
   for ( int i=0; i<size; i++ ) {
     int temp = arr[i];
     int r = rand() % size;
       arr[i] = arr[r];
       arr[r] = temp;
   }
 }
```

```c
 void update_rlimit(int resource, int soft, int hard) {
   struct rlimit rl;
   getrlimit(resource, &rl);
   rl.rlim_cur = soft;
   rl.rlim_max = hard;
   if (setrlimit(resource, &rl ) ==  -1 ) {
     perror("setrlimit");
     exit(-1);
   }
 }
```

```c
 const char* base_path = "";
 
 char g_hostname[256];
 pthread_mutex_t g_mutex;
 int g_error_count;
 
 int main(int argc, char** argv) {
 
   if(argc != 3){
     printf("usage: %s [ip address] [# of clients]\n", argv[0]);
     exit(-1);
   }
   strcpy(g_hostname, argv[1]);
   int th_num = atoi(argv[2]);
 
   int fileid_arr_size = 100;
   int fileid_arr[fileid_arr_size];
   for ( int i=0; i<fileid_arr_size; i++ ) {
     fileid_arr[i] = i;
   }
   randamize_array(fileid_arr, fileid_arr_size);
 
   // set resource to unlimited
   update_rlimit(RLIMIT_STACK, (int)RLIM_INFINITY, (int)RLIM_INFINITY);
   
   g_error_count = 0;
   pthread_mutex_init(&g_mutex, NULL);
   pthread_t* th = malloc(sizeof(pthread_t) * th_num);
 
   struct timespec ts_start;
   struct timespec ts_end;
   clock_gettime(CLOCK_MONOTONIC, &ts_start);
 
   for(int i = 0; i < th_num; i++){
     int *pfileid = malloc(sizeof(int));
     *pfileid = fileid_arr[i%fileid_arr_size];
     pthread_create(&th[i], NULL, exp1_eval_thread, pfileid);
   }
 
   for(int i = 0; i < th_num; i++){
     pthread_join(th[i], NULL);
   }
 
   clock_gettime(CLOCK_MONOTONIC, &ts_end);
   time_t diffsec = difftime(ts_end.tv_sec, ts_start.tv_sec);
   long diffnanosec = ts_end.tv_nsec - ts_start.tv_nsec;
   double total_time = diffsec + diffnanosec*1e-9;
 
   printf("total time is %10.10f\n", total_time); 
   printf("session error ratio is %1.3f\n",
    (double)g_error_count / (double) th_num);
 
   free(th);
 }
```

### exp1\_eval\_thread()

exp1\_eval\_thread()では，HTTPのGETリクエストを作成してSimple HTTPサーバへTCPで接続（exp1\_tcp\_connect()）します

Simple HTTPサーバのプログラム中に待ち受けポート番号は皆さんの学籍番号の下5桁を入力したと思いますが，HTTPベンチマークプログラムが接続する相手のポート番号xxxxxにも自分の学籍番号の下5桁を入力してください．

接続エラーがあった場合には，exp1\_session\_error()でエラーカウントを増加させます．

```c
 void* exp1_eval_thread(void* param)
 {
   int sock;
   int* pfileid;
   int fileid;
   char command[1024];
   char buf[2048];
   int total;
   int ret;
 
   pfileid = (int*) param;
   fileid = *pfileid;
   free(pfileid);
 
   sock = exp1_tcp_connect(g_hostname, "80");
   if(sock < 0){
     exp1_session_error();
     pthread_exit(NULL);
   }
 
   sprintf(command, "GET %s/%03d.jpg HTTP/1.0\r\n\r\n", base_path, fileid);
   ret = send(sock, command, strlen(command), 0);
   if(ret < 0){
     exp1_session_error();
     pthread_exit(NULL);
   }
 
   total = 0;
   ret = recv(sock, buf, 2048, 0);
 
   char res_buf[2048];
   strcpy(res_buf, buf);
   char* res_code = strtok(res_buf, " ");
   res_code = strtok(NULL, " ");
 
   while(ret > 0){
     total += ret;
     ret = recv(sock, buf, 2048, 0);
   }
   fprintf(stderr, "GET %s/%03d.jpg HTTP/1.0", base_path, fileid);
   fprintf(stderr, " -> Response Code: %s: Recieved Size: %d bytes\n", res_code, total);
 
   pthread_exit(NULL);
 }
```

### exp1\_session\_error()

接続エラーがあった場合に，エラーカウント用にグローバル変数で用意したg\_error\_countを増加させます．

その際，生成された複数のスレッドがそれぞれグローバル変数を更新する可能性があるため，pthread\_mutex\_lock()とpthread\_mutex\_unlock()でクリティカルセクションを排他制御しています．

```c
 void exp1_session_error() {
   pthread_mutex_lock(&g_mutex);
   g_error_count++;
   pthread_mutex_unlock(&g_mutex);
 
   return;
 }
```

### exp1.h

基本的にこれまで提示したexp1.hと同じと思いますが，念のため再掲しておきます．

```c
 #include <stdio.h>
 #include <stdint.h>
 #include <string.h>
 #include <strings.h>
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
 #include <sys/resource.h>
 #include <dirent.h>
 #include <signal.h>
 #include <pthread.h>
```

## 実行方法

-   サーバー上で，Simple HTTPサーバを実行
-   クライアント側（自分のノートPCのVirtualBox上のコマンドプロンプト）で上記のHTTPベンチマークプログラム（bench.exeという名前でコンパイル）を以下のように実行（サーバのIPアドレスとHTTPリクエスト数を指定）すると，指定した数のHTTPリクエストに対する全HTTPリプライが返ってくるまでの時間とセッションエラー率を表示

例１）

```shell
 $ ./bench 192.168.1.xxx 10
 total time is 1.0095188618
 session error ratio is 0.000 
```

例２）

```shell
 $ ./bench 192.168.1.xxx 100
 total time is 3.2459170818
 session error ratio is 0.000 
```

例３）

```shell
 $ ./bench 192.168.1.xxx 1000
 total time is 63.1578819752
 session error ratio is 0.148 
```

total timeが実行時間で，session errorがセッションエラー率です．  
最後の例のように，クライアントスレッド1000個がHTTPリクエストを送信した時，全てのHTTPリプライが返ってくるのに約63秒かかっています．  
その内，exp\_tcp\_connect()やsend()で失敗したセッションが約14.8%あったいう結果になります．  

物理的に離れた場所でHTTPサーバプログラムとHTTPベンチマークプログラムを動作させると，ネットワークの距離や品質によって結果に違いが出てくると思います．  
また，`tc` コマンド（Traffic Control）を使ってネットワーク遅延やパケットロスを疑似的に発生させてみることもできますので，興味ある人は試してみると面白いと思います．  

以上のHTTPベンチマークプログラムを使った評価結果は，グラフにまとめレポートへ載せてください．  
例えば，横軸を「HTTPリクエスト数」，縦軸を「応答時間」や「セッションエラー率」にしてグラフ化してみるといったことが考えられます．  

## \[必須課題1\] Simple HTTPサーバの性能計測

初めに各自のRaspberry PiとPCの間で，自作のSimple HTTPサーバとHTTPベンチマークプログラムを実行し，Simple HTTPサーバの性能を計測してください．  
次に各自が計測した結果を，グループ内で共有して比較してください．最後にその考察をレポートに書いてください．

計測結果ですが，グループ全員が同じ結果になるのが理想です．  
しかし何かしらの原因によって異なる結果になるかもしれません．  
その場合は「グループ内で計測結果が異なるのは何が原因か？」まで考察したうえで，レポートに書いてください．

## \[必須課題2\] Simple HTTPサーバの性能に関する考察

Simple HTTPサーバの性能計測結果について，他グループの性能計測結果との比較を行い，その違いについて，ソフトウェア・ハードウェア・オペレーティングシステムといった性能計測条件の観点から考察を行ってください．  

性能差があるにせよ，ないにせよ，なぜそのような結果になったのか，プログラムはどのように実行されるのか，通信はどのように実現されるのかなど，場合によっては図を描きながら議論を行い，各自の言葉で考察としてまとめてください．  

また，考察を定量的かつ論理的に確認するためにはどのような実装や実験を行えばよいか相談し，各自の言葉で再実験案としてまとめてください．

## \[発展課題\]

興味ある人は実際に試行錯誤してみて再実験条件や再実験結果を行い，トラフィックキャプチャ結果をもとにさらなる考察と検証方法などを深堀し，レポートにまとめてください．

来年書く卒業論文への予行演習を意識してみてください．

## おわりに

ベンチマークプログラムは正常に動作しましたか？

次は，第二部のグループワークで行う項目について役割分担を決めてください．