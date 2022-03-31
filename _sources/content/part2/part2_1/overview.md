# Day5の目的と概要

-   a) [Raspberry Piのセットアップ](./raspberrypi_setup "Raspberry Piのセットアップ")
-   b) [Simple HTTPサーバの実装](./simplehttpserver "Simple HTTPサーバの実装")
-   c) [HTTPベンチマークプログラムの実装](./httpbenchmark "HTTPベンチマークプログラムの実装")（必須）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: Raspberry Piのセットアップは終わりましたか？
- 15:00: Simple HTTPサーバとHTTPベンチマークプログラムは動きましたか？
- 17:00: グループ内でお互いのサーバへ負荷試験を行い結果について議論しましたか？
  - Simple HTTPサーバの性能計測
  - Simple HTTPサーバの性能に関する考察
```

## はじめに

　昨今のネットワークシステムは，バックエンドで動作しているプロトコルこそ大きく変化していませんが，少なくともユーザインタフェースは全てWebで統一されつつあると言えます．昨今では，以下に示すような様々な魅力的なWebデザインを実現するフロントエンド技術や通信プロトコルが出現しつつあります．

-   [今すぐ知っておくべき！モダンな10のWeb技術](https://techacademy.jp/magazine/11592)

-   [7つのWeb技術！Webデザイナーやフロントエンドエンジニアが押さえるべき流行をキャッチアップ](https://ferret-plus.com/5878)

-   [Webサーバ／アプリとの通信プロトコル種別をChromeブラウザの開発者ツールで調べる](http://www.atmarkit.co.jp/ait/articles/1610/21/news023.html)

-   [HTTP の進化](https://developer.mozilla.org/ja/docs/Web/HTTP/Basics_of_HTTP/Evolution_of_HTTP)

-   などなど

　第二部では，簡単に言えばWebブラウザで閲覧できるシステム，つまり「Webシステム」の根幹をなす「Webサーバ」と「Webクライアント」，それらのやりとりで使用される「HTTP (Hyper Text Transfer Protocol)」 について十分理解し，実装や拡張できるようになることを目的とします．Webに関係する技術の進化は早いですが，本質を理解し身に着けることができれば，どの業界に就職したとしても十分通用する知識や技術としてきっと役立つことと思います．

　第二部初日の今回は，最も単純なWebサーバとして単純なHTTPのみを処理できるSimple HTTPサーバを実際に実装し，簡単な評価（ベンチマーク）によってWebサーバの仕組みを理解しましょう．

## レポートに関して

以下のようにレポートの条件を設定します．

-   必須要件：全ての必須課題を実施すること（発展課題は加点対象です）．