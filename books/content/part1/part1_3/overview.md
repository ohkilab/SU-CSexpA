# Day3の目的と概要

- a) [サーバプログラムの多重化](./multiplexing_server_program "サーバープログラムの多重化")
- b) [ベンチマークプログラムの設計と実装](./benchmarking_program "ベンチマークプログラムの設計と実装")
- c) [性能計測](./performance_measurement "性能計測") (必須)
- d) [非同期I/Oによる多重化](./multiplexing_asynchronous_IO "非同期I/Oによる多重化") (発展)
- e) [通信失敗の原因分析](./cause_analysis_communication_failure "通信失敗の原因分析") (発展)

```{admonition} 本日の進捗確認チェックリスト
- 13:30: 多重化したサーバプログラムの動作を確認できましたか？
- 14:00: ベンチマークプログラムの動作を確認できましたか？
- 15:00: 仮説を立てて性能計測を実施しましたか？
- 16:00: 仮説と性能計測結果との関係をレポートに向けて整理できましたか？
- 17:00: 本日の内容をレポートにまとめることができましたか？
```

## はじめに

複数クライアントからの接続に対して，同時にサービスを提供できるサーバプログラムを設計・開発します．実現するための技術が複数あります．以下の技術を用いて設計・実装したサーバプログラムに対してベンチマーク計測を実施し，多重化を行わない場合と各技術で多重化を行った場合の特徴を明らかにしてください．

- 多重化なし（必須課題）
- サーバープログラムの多重化（必須課題）
  - select 関数による多重化
  - マルチプロセスによる多重化
  - マルチスレッドによる多重化
- 非同期I/Oによる多重化（発展課題）
  - libevent による多重化
  - libuv による多重化

ユーザプログラムの通信終了までの実行時間について，以下の問に対して仮説を設定し，計測結果をもってその仮説を説明してください． また，そのような結果となった理由を考察してください．

**問1）最も実行速度が速い方法はどれか？**

**問2）最も通信待ち時間が少ない方法はどれか？**

**問3）最も通信失敗が少ない方法はどれか？**

**問4）最も少ないリソースで実行できる方法はどれか？**

測定条件によって結果が異なる可能性があるため，それぞれの測定条件を周囲と相談した上で適切な測定条件を明記してください．

## 計測準備

計測のためには各技術を利用して設計・実装したサーバプログラムと，そのサーバに負荷をかけてアクセスするクライアントプログラムが必要になります．

### サーバプログラム

[サーバプログラムはネットワーク入出力プログラミング](../part1_2/network_IO_programming)の必須課題10のサンプルコードを参考にします．必須課題10ではEchoBack（クライアントプログラムから送信されたメッセージを受信すると，受信メッセージに追加メッセージを付与して返信する動作）のプログラムを開発しました．

このプログラムは単一のクライアントからの受信メッセージに対して1回だけ応答して終了します．そのため，複数回の受信メッセージには対応できませんし，複数のクライアントからの接続にも対応していません．そこで，あるクライアントから複数回メッセージを送られた場合に対応できるように拡張してください．更に，複数のクライアントからの接続に対して対応できる以下の実験用サーバに拡張してください（複数のクライアントからの接続があった場合，最初に接続したクライアントからの通信が切断されるまで他のクライアントを待たせる形で良い）．

- 多重化なしのEchoBackサーバ

このサーバは，複数のクライアントからの接続があると最初に接続したクライアントとの通信が終わるまで他のクライアントからの接続は受け付けられません（他のクライアントを待たせる振る舞いになります）．そこで，このプログラムを複数のクライアントからの接続に対して同時に応答できるように拡張し，以下の実験用サーバプログラムを開発します．

- select 関数によって多重化したEchoBackサーバ
- マルチプロセスによって多重化したEchoBackサーバ
- マルチスレッドによって多重化したEchoBackサーバ
- 非同期I/Oによって多重化したEchoBackサーバ

### クライアントプログラム

この実験で使用するクライアントプログラムは，サーバプログラムに負荷をかけることで様々なサーバ実装方法の特徴や違いを明らかにすることを意図しています．そこで，クライアントプログラムでは並列処理を用いてサーバに同時接続を行い，サーバ側のCPU等に大きな負荷をかけられるように実装します．シングルスレッドでメッセージ送受信を繰り返すようなプログラムでは（例えば，while文などでサーバへの接続，メッセージ送信とサーバからのメッセージ受信を繰り返した場合），クライアントの処理時間や通信遅延などもありますので，サーバ側のCPU等へ大きな負荷をかけることは困難です．
