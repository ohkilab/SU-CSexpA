# 複数クライアント対応
## はじめに

複数クライアントからのHTTPリクエストを並列に処理できるようにする方法は，以下に示すように複数の方法があります．  
それぞれの方法には，リソース消費量，処理速度，並列性，プログラミングの容易さ，各処理終了時のリソース開放，移植性といった面で，様々なメリット，デメリットがあります．  
全てにおいて万能な方法はありませんので，何を一番の目的にするかによって適切な方法を選択する必要があります．  
なお，本ページで説明する必須課題についてはグループ内で分担して実装を行っても構いませんが，<u>全員が全ての実装コードの動作を確認した上で，動作の内容を説明できるよう</u>に理解しておいてください．  
また，性能比較については全員が行うようにしてください．

-   シングルプロセス，シングルスレッドでの複数クライアント対応（select(), poll(), EPOLLなど）
-   マルチプロセス化による複数クライアント対応（fork()の利用）
-   マルチスレッド化による複数クライアント対応（pthread()の利用）

## \[必須課題\] select()での実装

select()を用いると，標準入力や複数のソケットが読み込み可能な状態かどうか複数のディスクリプタを同時に監視することができ，入力の多重化が容易にできます．  
いずれかのディスクリプタが読み込み可能になるまで動作をブロックし，プログラムをスリープ状態にするためCPUパワーを消費しません．  
select()を用いたサンプルソースコードは示しませんので，各自で調べて実装してみてください．ここでは，select()を用いる際のポイントのみ説明します．  

select()を利用する際の基本的な流れ

-   FD\_ZERO(): 監視するディスクリプタを登録するマスクをクリア
-   FD\_SET(): 監視するディスクリプタをマスクに登録
-   select(): マスクに登録されたディスクリプタがレディになるのを待つ
-   FD\_ISSET(): どのディスクリプタがレディになったか監視

ここで，select()の第1引数で設定する値は，「監視するディスクリプタの最大値＋1」を設定しなければならないことに注意しましょう．  
また，引数が多く複雑に見えますが，よく使うのは，第1引数（監視するディスクリプタの最大値＋1），第2引数（入力を監視したいディスクリプタを登録したマスク），第5引数（タイムアウトまでの待ち時間）ですので，ここだけはしっかり理解し設定するようにしましょう．

また，select()は呼び出すと，引数で指定したfd\_set型のマスクデータを書き換えますので，もしループ処理の中でselect()を呼び出している場合は，毎回セットし直すか，一度セットしたら毎回別の変数に代入して渡す必要があります．

※ select()実装で意図せぬ動作する原因は，引数設定の誤りやマスクの再セットを忘れていることが多いのでよく注意してください．

また，select()と同様の機能を実現するシステムコールとして，select()をより使いやすく拡張したpoll()や，poll()を高速化したシステムコール群であるEPOLLがあります．  
poll()は，select()の毎回マスクを保存する必要があった点が改善されただけでなく，ディスクリプタを格納するデータを配列として与えられるためOSの制約（最大ディスクリプタ数）が緩和され，使いやすくなっています（select()より細かい指定も可能）．  
ただし，select()やpoll()を用いることで比較的簡単に複数クライアント対応できますが，非常に多くのディスクリプタを扱う場合（数千を超えるなど），レディになった後にどのディスクリプタがレディになったかを調べる実装では性能面で問題が出る可能性があります．  
そこで，レディになったディスクリプタのみを通知するEPOLLが生まれました．  
いずれもLinux専用ですが，興味ある人はpoll()やEPOLLでの実装もチャレンジしてみてください．

## \[必須課題\] fork()での実装

select()やpoll()，EPOLLを用いたシングルプロセス内での多重化は，シンプルで理解しやすい仕組みですが，同じループ内でアクセプトや受信処理を行うなど設計を上手に行わないと複雑なプログラムになりがちです．  
ここでは，OSの持つ並列処理機能であるfork()を用いて，アクセプトした後の処理を並列化させることで，複数クライアント対応を実現する方法を実装してみてください．

プロセスとはOSから見た一つの実行単位です．  
プログラムを実行すると，通常一つのプロセスとしてOSの管理下で動作し，その割り当てられたメモリ空間の中でスタックが積まれて関数が実行されたりします．  
このプロセスを複数同時に動作させるとマルチプロセスになります．  
ここでは，処理の途中までは一つのプロセスが処理を行い，処理の多重化が必要になった時点でfork()を呼び出し，親のプロセスの配下で子供のプロセスを生成するようにして複数クライアント対応を実現してみてください．

fork()の基本的な動作は以下となります．

-   子プロセスの生成: fork()を実行した時点で子プロセスが複製され並列処理を開始
-   子プロセスの終了: 子プロセスが\_exit()した時点で終了
-   子プロセス終了の検知: 親プロセスにSIGCHLDが通知されるため，wait()，waitpid()で終了状態を取得可能

簡単ではありますが，fork()を用いたサンプルソースコードも示しておきます．  
fork()の戻り値が0になるのは，プロセスが複製された時の子プロセス側になります．  
fork()の戻り値が0以上になるのは，親プロセス側で，戻り値は子プロセスのプロセスIDになります．  
プロセスIDは，kill()で強制終了させたい場合や，子プロセスの終了を得るために重要です．  
子プロセス，親プロセスがどのように動作することで，標準出力（皆さんのディスプレイ）に何が表示されるか，どちらのプロセスがどのタイミングで何を出力しているのか理解し，グループメンバ全員とその理解で正しいかどうか確認してください.

```c
#include "exp1.h"

int main(int argc, char **argv)
{
  while(1){
    int pid = fork();

    if(pid == 0){
      sleep(5);
      printf("hello from child\n");
      _exit(0);
    }else{
      sleep(1);
      printf("pid = %d created\n", pid);
    }
    sleep(1);
  }
}
```

なお，子プロセスの終了を親プロセスでwait()しないで放置すると，子プロセスで使用されたリソースがいつまでも解放されない状態（ゾンビプロセス）になってしまいます．  
killコマンドを使っても，ゾンビプロセスは親プロセスが終了するまでは，完全に終了させることができず見た目もよろしくないため，必ずwait()してリソースを回収する習慣をつけてください．  
ただし，wait()は一つでも子プロセスの終了が得られないとブロックしてしまいますので，waitpid()で第3引数にWNOHANGを指定してノンブロッキングで定期的に子プロセスの終了をwaitpid()するだけでなく，子プロセス終了時に届くSIGCHLDで，wait()を行い子プロセスの終了状態を取得するようにしましょう．  

※ 既に調べていると思いますが，シグナルハンドラ内では，fprintf()などの非同期シグナルに非安全（未定義の動作を引き起こす可能性のある）とされる関数は使うべきではありません．  
非同期シグナルハンドラ内では，安全な関数のみ使うようにしてください．

## \[必須課題\] pthreadでの実装

並列処理で主流なのは，マルチスレッドです．  
マルチプロセスでは，それぞれのプロセスは異なるメモリ空間で動作しますが，スレッドは一つのプロセスの中で動作しますので，共通のメモリ空間に存在します．  
そのため，スレッド間でのデータ共有が容易です．  
逆に，どれか一つのスレッドが異常を起こすと，プロセス全体が影響を受けてしまう可能性があるので注意が必要です．  
例えば，異なるスレッドが，共用のメモリ空間上にあるデータを書き換えてしまうことが容易にできるため，意図せぬ動作によって不具合が生じてしまいプロセス全体を終了せざるを得ない場合が発生します（排他制御が必要）．  
また，1プロセスで使用できるディスクリプタ数の制限もありますので，安易に膨大な数のスレッドを生成するのは危険です．  
そのため，マルチスレッドが主流の昨今でも，一部をマルチプロセスで実装しプログラム全体の安全性を向上させる場合もあります．  

また，C言語は，マルチスレッドという概念が登場する前から使用されていたため，標準ライブラリとして用意されているライブラリ関数の中には，マルチスレッドで使用すると問題が生じてしまう関数もいくつか存在します．  
そのため，マルチスレッドでも安全に使用できるように改良されたスレッドセーフ版がある場合も多いため，マルチスレッドのプログラムを開発する場合は，「関数名\_r()」という名称であることの多いスレッドセーフ版を使用しましょう．

マルチプロセスでは，fork()した時点で並列処理が開始されますので，関数の途中でif()文で枝分かれして並列処理を実現していました．  
マルチスレッドでは，pthread\_create()という関数に明示的にスレッドとして生成したい関数を指定し，その指定された関数が別スレッドになって並列処理を実現します．

pthread実装の基本的な流れ

-   スレッド生成: pthread\_create()でスレッド生成し，呼び出す関数で並列処理を開始
-   スレッド切り離し: pthread\_detach()しておくと，スレッド終了時に使用リソースを自動解放
-   スレッド終了: 呼び出した関数が返ることでスレッド終了（pthread\_exit()で終了も可能）
-   スレッド合流: pthread\_join()でスレッドの終了を待ち，戻り値を得ることも可能

```c
#include "exp1.h"

void* exp1_thread(void *param){
  int* psock;
  int sock;

  pthread_detach(pthread_self());
  psock = (int*) param;
  sock = *psock;
  free(psock);
  
  sleep(5);
  
  printf("thread %d is ending\n", sock);
}

void exp1_create_thread(int sock)
{
  int *psock;
  pthread_t th;
  psock = malloc(sizeof(int));
  *psock = sock;

  pthread_create(&th, NULL, exp1_thread, psock);
  printf("thread %d is created\n", sock);
}


int main(int argc, char **argv)
{
  int i = 0;

  while(1){
    exp1_create_thread(i);
    i++;
    sleep(1);
  }
}
```
ここで注意しなけらばならないのは，スレッドでは1つだけ引数を呼び出す関数へ渡せるのですが，受け渡す変数のメモリ領域を確保しておかなければなりません．  
以下の例では，malloc()でメモリ領域を確保してスレッドを呼び出し，スレッドがその値を受け取ったらそのメモリ領域をfree()で開放しています．  
ここで，プロセスが終了すれば使用中のリソースは解放されますが，マルチスレッドの場合はあくまで1プロセスの中での処理ですので，メモリ領域の開放を忘れるとプロセスが終了するまで解放されません．  
つまり，スレッドのためのメモリ確保領域の開放を忘れたまま膨大なスレッドが生成されると，1プロセスで管理できるアドレス領域を超えてしまい，必ずpthread\_create()が失敗するようになりますので注意してください．  
興味ある人は試してみてください（もちろんこのような自主的な試みはレポートに分かるように記載して積極的にアピールしてください）．

## \[必須課題\] Apacheでの実装

上記実装との性能比較用です．  
単にSimple HTTPサーバーと同じページが表示されるようApacheの設定に関して以下を行うだけとなります．

-   test.htmlの配置
-   各種画像ファイルの配置

Apacheの設定では，pre-forkやpre-threadなど指定することもできますが，第三部で全グループ実施しますので，ここでは先取りして試してみてもよいですし，試さなくても構いません．  
少なくとも上記のみは実施してください．

## \[発展課題\] 独自方式での実装

上記の実装方式とは別に複数クライアント対応できる方法を考えて実装してみてください．  
その際，上記実装方式との違い（メリット，デメリット）を様々な側面から文章で整理して考察するだけでなく，事前に想定した性能に対して実際にどの程度の性能が得られたか，なぜそうなったかなどまで検討して考察してください．

## \[発展課題\] HTTPベンチマークプログラムの機能拡張

Simple HTTPサーバの機能拡張に合わせて，HTTPベンチマークプログラムも対応できるよう機能拡張してみてください．  
例えば以下のような機能拡張が考えられます．  
もちろんその他の創意工夫は自由ですので，どのような拡張機能を実装したか目立つよう明確かつ積極的にアピールしてください．

-   getopt()利用してコマンドラインからオプション指定を取り込む機能
-   1スレッドあたりのHTTPリクエスト数を設定できるように拡張
    -   多数のスレッドを生成しなくても性能計測がし易くなるかもしれません
-   TCP接続完了までの時間や，HTTPリクエスト送信完了までの時間など，より細かな内訳分析できるよう拡張
-   POSTへの対応
-   Keep-Aliveへの対応
-   Web Socketへの対応
-   HTTP/2への対応
-   などなど

## \[必須課題\] 性能比較

これまでに機能拡張したHTTPサーバやHTTPベンチマークプログラムソフトを使って，各実装方式での性能比較を行い計測結果をグラフ化するとともに，そのような結果になった理由を考察してください．  
もちろん実験を行う前に，班員間でどのような実験結果になるのか十分議論し予想内容を整理してから性能比較実験を行ってください．

各実装方式の性能比較は，シェルスクリプト等を書くと作業が楽になると思います．  
グラフの表示は，gnuplot等を使用すると楽と思います．  
例えば，横軸を「HTTPリクエスト数」，縦軸を「応答時間」や「セッションエラー率」にしてグラフ化してみるといったことが考えられます．  
応答時間に関して，

機能拡張したHTTPサーバ（各方式でどの程度違う？) ＜ Apache ＜ Simple HTTPサーバ  

といった結果が容易に想像できると思いますが，より深く検討と考察してみてください．

ここで，皆さんの使用しているサーバ機やクライアント機における**CPUのコア数やスレッド数**（ハードウェア的制約），Linuxにおける**ファイルディスクリプタ数の上限**（ソフトウェア的制約）についてもご注意ください．

## おわりに

ここまでで複数クライアント対応すべくHTTPサーバの高機能化の実装は終了です．

以上の実装や各実装方式の性能比較に関して，読み手が理解しやすい表現方法（グラフや表など）を意識してみてください．

つまり，今後の卒論や論文執筆を意識してみるとよいと思います．著名な論文誌に採択された論文に掲載されている表現方法など参考にするとよいと思います．