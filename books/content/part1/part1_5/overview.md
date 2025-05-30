# Day5の目的と概要

-   a) [第一部達成基準](./standardforachievement "第一部達成基準")
-   b) [複数クライアント対応](./multipleclient "複数クライアント対応")（必須）
-   c) [セキュリティホール](./securityhole "セキュリティホール")（必須＋発展）
-   d) [HTTPサーバ機能拡張](./httpserver_expansion "HTTPサーバ機能拡張")（選択）

```{admonition} 本日の進捗確認チェックリスト
- 13:15: 第一部の達成基準を理解しグループ内で役割分担を話し合いましたか？
- 15:00: 複数クライアント対応（必須）の全てを実装しテストできましたか？
    - select, fork, pthread, Apache
    - 簡易性能比較
- 16:30: セキュリティホール（必須）の全てを実装し動作を確認しましたか？
- 17:00: 選択課題や発展課題への挑戦スケジュールを相談しましたか？
```

## はじめに

　前回実装したSimple HTTPサーバは，同時には一つのクライアントからのHTTPリクエストしか応答することができません．しかし，世の中で一般的なWebサーバは，同時に複数のクライアントからのHTTPリクエストを処理しています．そこで，Simple HTTPサーバを拡張し，同時に複数のクライアントからのHTTPリクエストを処理できるよう改良してください．

複数のクライアントからのHTTPリクエストを並列に処理できるようにする方法は，以下に示すように複数の方法があります．

-   シングルプロセス，シングルスレッドでの複数クライアント対応（select(), poll(), EPOLLなど）
-   マルチプロセス化による複数クライアント対応（fork()の利用）
-   マルチスレッド化による複数クライアント対応（pthread()の利用）

  
　それぞれの方法には，リソース消費量，処理速度，並列性，プログラミングの容易さ，各処理終了時のリソース開放，移植性といった面で，様々なメリット，デメリットがあります．全てにおいて万能な方法はありませんので，何を一番の目的にするかによって適切な方法を選択する必要があります．そのためには，全ての方法を実際に使用した経験があり，正しい判断ができるようになることが重要です．一人で全てを実装するのは大変ですので，グループ内で役割分担を決め実装し，どのような場合にどの複数クライアント対応方法がよいのか悪いのか，実際に試してみることで深い知識と能力を身に着けてほしいと思います．

## レポートに関して

以下のようにレポートの条件を設定します．

-   必須要件：全ての必須課題の実施、および選択課題のうちいずれかの実施（発展課題は加点対象です）．