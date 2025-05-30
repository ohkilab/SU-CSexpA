# メインページ
```{admonition} Staff
- 担当教員：大木哲史，野口靖浩，峰野博史
- 技術部：水野匠，小澤卓也
- TA：M2笹，高橋，M1徳増，牧野
- 質問窓口：Mattermost もしくは csexp1-staff@ml.inf.shizuoka.ac.jp
```

## 今年度の講義について
今年度の講義日程やレポート課題の提出締切日などは[今年度の講義について](./schedule)を参照してください．

## 事前準備 （在宅）

RaspberryPiについてはセットアップ済のイメージをこちらで用意します．

実験ガイダンス時に機材一式を配布しますので，初回までに[環境構築および動作確認](preparation/preparation)を済ませておいてください．（必ず実験ガイダンスに参加すること！）

### 出席と進捗状況の確認について

毎回の演習時間の最初と最後に，スタッフが各班に出席および進捗状況を確認しに伺います（オンラインの場合はMattermost等を使います）．

## 第一部 ソケットプログラミング

### \[Day1\] 環境構築と実験の開始

- RaspberryPiのセッティング
- RaspberryPiの有線接続
- RaspberryPiの無線接続
- ソフトウェアのアップデート
- VSCodeを使った環境構築（任意）

### \[Day2\] 通信性能の計測

- ファイル入出力プログラミング
- ネットワーク入出力プログラミング
- 入出力性能の計測（必須）

### \[Day3\] 多対多接続の設計・実装

- サーバプログラムの多重化
- ベンチマークプログラムの設計と実装
- 性能計測 (必須)
- 非同期I/Oによる多重化 (発展)
- 通信失敗の原因分析 (発展)

### \[Day4\] 第一部レポートピアレビュー

- ピアレビュー

## 第一部 Webサーバの実装

### \[Day5\] 個別レポートコメント，Simple HTTPサーバの構築

- Raspberry Piのセットアップ
- Simple HTTPサーバの実装
- HTTPベンチマークプログラムの実装（必須）

### \[Day6\] HTTPサーバの高機能化(1)

- 第一部達成基準
- 複数クライアント対応（必須）
- セキュリティホール（必須＋発展）
- HTTPサーバ機能拡張（選択）

### \[Day7\] HTTPサーバの高機能化(2)

- 第一部の実装で詰まった時の対処法
- 第一部グループ発表会に向けた準備

### \[Day8\] 第一部グループ発表会

- 第一部グループ発表会

### \[Day9\] 第一部レポートピアレビュー

- ピアレビュー

## 第二部 Webシステムの性能

### \[Day10\] Webシステムの基礎

- 第二部の準備
- Webシステムの基礎（発展）
- サンプルプログラム(PHP)
- サンプルプログラム(JavaScript)
- ベンチマークソフトab（Apache Bench）（必須）

### \[Day11\] Webシステムの設計と実装

- DBMSとの接続（必須）
- 最終課題の詳細

### \[Day12\] ベンチマークと分析

- Apache MPMについて
- 実装の参考となるリンク集
- WebSocketについて
- 検討状況の確認（必須）

### \[Day13, Day14\] 発表会準備

- 第二部グループ発表会の準備

### \[Day15\] 第二部グループ発表会

- 最終グループ発表会

## 実験終了後（必須）

- [実験機材の返却](end/cleanup)　※必須

## その他

### トラブルシューティング

- [質問の行い方](trouble_shooting/question)
- [よくあるトラブル](trouble_shooting/trouble)

```{important}
よくあるトラブルへの対応方法や、記載事項の修正など、本Webサイトへの追記・修正リクエストは随時受け付けています．
リクエストがある場合下記GitHubレポジトリへのpull-requestあるいはIssueとしてお送りください（最終成績に加味します）．

https://github.com/ohkilab/SU-CSexpA/
```

### 過去のページ

- [2017-21年度](https://exp1.inf.shizuoka.ac.jp/Home)
- [2015-6年度](http://exp1.inf.shizuoka.ac.jp/index.php?title=Home&oldid=875)
- [2014年度](http://www.minelab.jp/NWprog-CSexp1/2014/)
- [2013年度](http://www.minelab.jp/NWprog-CSexp1/2013/)
