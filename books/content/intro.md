# 情報科学実験Ⅰ

```{admonition} 連絡先
- 担当教員：大木哲史，野口靖浩，峰野博史
- 技術部：水野 匠，柴田頼紀
- TA：M2成田，M2鎌田，M1，M1，B4，B4
- 質問窓口：csexp1-staff@ml.inf.shizuoka.ac.jp
- レポート提出先： csexp1-report@ml.inf.shizuoka.ac.jp
```

```{list-table} 情報科学実験Ⅰスケジュール
:header-rows: 1
:class: full-width
:name: schedule

* - Day
  - 日程
  - 対面
  - 在宅
* - Day1
  - 4月12日
  - 奇数
  - 偶数
* - Day2 
  - 4月19日
  - 偶数
  - 奇数
* - Day3
  - 4月26日
  - 奇数
  - 偶数
* - Day4
  - 5月10日
  - \-
  - 奇数&偶数
* - Day5
  - 5月17日
  - 偶数
  - 奇数
* - Day6
  - 5月24日
  - 奇数
  - 偶数
* - Day7
  - 5月31日
  - 偶数
  - 奇数
* - Day8
  - 6月7日
  - \-
  - 奇数&偶数
* - Day9
  - 6月14日
  - \-
  - 奇数&偶数
* - Day10
  - 6月21日
  - 奇数
  - 偶数
* - Day11
  - 6月28日
  - 偶数
  - 奇数
* - Day12
  - 7月5日
  - 奇数
  - 偶数
* - Day13
  - 7月12日
  - 偶数
  - 奇数
* - Day14
  - 7月19日
  - 奇数
  - 偶数
* - Day15
  - 7月26日
  - \-
  - 奇数&偶数
```

## 事前準備 （在宅）

2021年度はRaspberryPiについてはセットアップ済のイメージをこちらで用意します．

実験ガイダンス時に機材一式を配布しますので，初回までに環境構築と動作確認を済ませておいてください．（必ず実験ガイダンスに参加すること！）

[環境構築および動作確認](preparation/preparation)

<!-- ~\* [Raspbian環境のダウンロード](https://exp1.inf.shizuoka.ac.jp/Raspbian%E7%92%B0%E5%A2%83%E3%81%AE%E3%83%80%E3%82%A6%E3%83%B3%E3%83%AD%E3%83%BC%E3%83%89 "Raspbian環境のダウンロード")~

-   [VNCクライアントのインストール](https://exp1.inf.shizuoka.ac.jp/VNC%E3%82%AF%E3%83%A9%E3%82%A4%E3%82%A2%E3%83%B3%E3%83%88%E3%81%AE%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB "VNCクライアントのインストール")
-   RaspberryPiの準備

~\*\*a) [RaspberryPiのセットアップ](https://exp1.inf.shizuoka.ac.jp/RaspberryPi%E3%81%AE%E3%82%BB%E3%83%83%E3%83%88%E3%82%A2%E3%83%83%E3%83%97 "RaspberryPiのセットアップ")~ ~\*\*b) [VNC接続](https://exp1.inf.shizuoka.ac.jp/VNC%E6%8E%A5%E7%B6%9A "VNC接続")~ ~\*\*c) [SSH接続](https://exp1.inf.shizuoka.ac.jp/SSH%E6%8E%A5%E7%B6%9A "SSH接続")~ ~\*\*d) [日本語入力環境](https://exp1.inf.shizuoka.ac.jp/%E6%97%A5%E6%9C%AC%E8%AA%9E%E5%85%A5%E5%8A%9B%E7%92%B0%E5%A2%83 "日本語入力環境")（必要な人のみ）~

-   [Virtual BoxとVMの準備](https://exp1.inf.shizuoka.ac.jp/Virtual_Box%E3%81%A8VM%E3%81%AE%E6%BA%96%E5%82%99 "Virtual BoxとVMの準備")
-   [Teamsの準備](https://exp1.inf.shizuoka.ac.jp/Teams%E3%81%AE%E6%BA%96%E5%82%99 "Teamsの準備") -->

### 出席と進捗状況の確認について

毎回の演習時間の最初と最後に，スタッフが各班に出席および進捗状況を確認しに伺います（オンラインの場合はTeams等を使います）．

## 第一部 ソケットプログラミング

### \[4月13日\] 環境構築と実験の開始

-   [RaspberryPiの接続（固定IP）](https://exp1.inf.shizuoka.ac.jp/RaspberryPi%E3%81%AE%E6%8E%A5%E7%B6%9A%EF%BC%88%E5%9B%BA%E5%AE%9AIP%EF%BC%89 "RaspberryPiの接続（固定IP）")

-   [第一部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%80%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第一部レポート")（初版提出: 5/8（土）23:59，最終版提出: 5/15（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 13:30: RaspberryPiを起動できましたか？
- 14:30: RaspberryPiにPCからVNC接続できましたか？
- 15:00: RaspberryPiにPCからファイルを転送できましたか？
- 15:30: RaspberryPiにVMからSSH接続できましたか？
- 16:00: VM上のLinuxにPCからファイルを転送できましたか？
  - 問題なく設定できた場合はそのまま次回の内容に進んでください．
```

### \[4月20日\] 通信性能の計測

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/%E3%80%8C%E9%80%9A%E4%BF%A1%E6%80%A7%E8%83%BD%E3%81%AE%E8%A8%88%E6%B8%AC%E3%80%8D%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "「通信性能の計測」の目的と概要")
-   a) [ファイル入出力プログラミング](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E5%85%A5%E5%87%BA%E5%8A%9B%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0 "ファイル入出力プログラミング")
-   b) [ネットワーク入出力プログラミング](https://exp1.inf.shizuoka.ac.jp/%E3%83%8D%E3%83%83%E3%83%88%E3%83%AF%E3%83%BC%E3%82%AF%E5%85%A5%E5%87%BA%E5%8A%9B%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0 "ネットワーク入出力プログラミング")
-   c) [入出力性能の計測](https://exp1.inf.shizuoka.ac.jp/%E5%85%A5%E5%87%BA%E5%8A%9B%E6%80%A7%E8%83%BD%E3%81%AE%E8%A8%88%E6%B8%AC "入出力性能の計測")（必須課題）
-   [第一部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%80%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第一部レポート")（初版提出: 5/8（土）23:59，最終版提出: 5/15（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 13:30: 必須A課題1～6を進められましたか？
- 14:00: 必須B課題7～10を進められましたか？
- 15:00: バッファサイズを変更する前の入出力性能，並びにバッファサイズを変更した後の入出力性能は計測・記録できましたか？
  - ファイルコピー性能
  - 通信性能
- 16:00: バッファサイズの変更による通信性能の変化について，なぜそのような変化となるのかについての原因の仮説を立てた上で，その仮説を支持する証拠を見つける実験を行えましたか？
- 17:00: 本日の内容をレポートにまとめることができましたか？
```

### \[4月27日\] 多対多接続の設計・実装

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/%E3%80%8C%E5%A4%9A%E5%AF%BE%E5%A4%9A%E6%8E%A5%E7%B6%9A%E3%81%AE%E8%A8%AD%E8%A8%88%E3%83%BB%E5%AE%9F%E8%A3%85%E3%80%8D%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "「多対多接続の設計・実装」の目的と概要")
-   a) サーバプログラムの多重化

-   a1) [select 関数による多重化](https://exp1.inf.shizuoka.ac.jp/select_%E9%96%A2%E6%95%B0%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "select 関数による多重化")
-   a2) [マルチスレッドによる多重化](https://exp1.inf.shizuoka.ac.jp/%E3%83%9E%E3%83%AB%E3%83%81%E3%82%B9%E3%83%AC%E3%83%83%E3%83%89%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "マルチスレッドによる多重化")
-   a3) [マルチプロセスによる多重化](https://exp1.inf.shizuoka.ac.jp/%E3%83%9E%E3%83%AB%E3%83%81%E3%83%97%E3%83%AD%E3%82%BB%E3%82%B9%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "マルチプロセスによる多重化")

-   b) [ベンチマークプログラムの設計と実装](https://exp1.inf.shizuoka.ac.jp/%E3%83%99%E3%83%B3%E3%83%81%E3%83%9E%E3%83%BC%E3%82%AF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E3%81%AE%E8%A8%AD%E8%A8%88%E3%81%A8%E5%AE%9F%E8%A3%85 "ベンチマークプログラムの設計と実装")
-   c) [性能計測](https://exp1.inf.shizuoka.ac.jp/%E6%80%A7%E8%83%BD%E8%A8%88%E6%B8%AC "性能計測") (必須課題)
-   d) [非同期I/Oによる多重化](https://exp1.inf.shizuoka.ac.jp/%E9%9D%9E%E5%90%8C%E6%9C%9FI/O%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "非同期I/Oによる多重化") (発展課題)

-   d1) [libeventによる多重化](https://exp1.inf.shizuoka.ac.jp/libevent%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "libeventによる多重化")
-   d2) [libuvによる多重化](https://exp1.inf.shizuoka.ac.jp/libuv%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "libuvによる多重化")
-   d3) [epollによる多重化](https://exp1.inf.shizuoka.ac.jp/epoll%E3%81%AB%E3%82%88%E3%82%8B%E5%A4%9A%E9%87%8D%E5%8C%96 "epollによる多重化")

-   e) [通信失敗の原因分析](https://exp1.inf.shizuoka.ac.jp/%E9%80%9A%E4%BF%A1%E5%A4%B1%E6%95%97%E3%81%AE%E5%8E%9F%E5%9B%A0%E5%88%86%E6%9E%90 "通信失敗の原因分析") (発展課題)
-   [第一部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%80%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第一部レポート")（初版提出: 5/8（土）23:59，最終版提出: 5/15（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 13:30: 多重化したサーバプログラムの動作を確認できましたか？
- 14:00: ベンチマークプログラムの動作を確認できましたか？
- 15:00: 仮説を立てて性能計測を実施しましたか？
- 16:00: 仮説と性能計測結果との関係をレポートに向けて整理できましたか？
- 17:00: 本日の内容をレポートにまとめることができましたか？
```

### \[5月11日\] 第一部レポートピアレビュー

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/%E3%80%8C%E7%AC%AC%E4%B8%80%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88%E3%83%94%E3%82%A2%E3%83%AC%E3%83%93%E3%83%A5%E3%83%BC%E3%80%8D%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "「第一部レポートピアレビュー」の目的と概要")
-   a) [ピアレビュー](https://exp1.inf.shizuoka.ac.jp/%E3%83%94%E3%82%A2%E3%83%AC%E3%83%93%E3%83%A5%E3%83%BC "ピアレビュー")
-   [第一部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%80%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第一部レポート")（初版提出: 5/8（土）23:59，最終版提出: 5/15（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: ピアレビューを実施しましたか？
- 15:00: それぞれどこを修正すべきか議論しましたか？
- 16:00: 再実験するなど修正すべき箇所を更新できましたか？
- 17:00: 第一部レポート（最終版）完成の目途はたちましたか？
```

## 第二部 Webサーバの実装

### \[5月18日\] 個別レポートコメント，Simple HTTPサーバの構築

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day6%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day6の目的と概要")
-   a) [Raspberry Piのセットアップ](https://exp1.inf.shizuoka.ac.jp/Raspberry_Pi%E3%81%AE%E3%82%BB%E3%83%83%E3%83%88%E3%82%A2%E3%83%83%E3%83%97 "Raspberry Piのセットアップ")
-   b) [Simple HTTPサーバの実装](https://exp1.inf.shizuoka.ac.jp/Simple_HTTP%E3%82%B5%E3%83%BC%E3%83%90%E3%81%AE%E5%AE%9F%E8%A3%85 "Simple HTTPサーバの実装")
-   c) [HTTPベンチマークプログラムの実装](https://exp1.inf.shizuoka.ac.jp/HTTP%E3%83%99%E3%83%B3%E3%83%81%E3%83%9E%E3%83%BC%E3%82%AF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E3%81%AE%E5%AE%9F%E8%A3%85 "HTTPベンチマークプログラムの実装")（必須）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: Raspberry Piのセットアップは終わりましたか？
- 15:00: Simple HTTPサーバとHTTPベンチマークプログラムは動きましたか？
- 17:00: グループ内でお互いのサーバへ負荷試験を行い結果について議論しましたか？
  - Simple HTTPサーバの性能計測
  - Simple HTTPサーバの性能に関する考察
```

### \[5月25日\] HTTPサーバの高機能化(1)

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day7%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day7の目的と概要")
-   a) [第二部達成基準](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E9%81%94%E6%88%90%E5%9F%BA%E6%BA%96 "第二部達成基準")
-   b) [複数クライアント対応](https://exp1.inf.shizuoka.ac.jp/%E8%A4%87%E6%95%B0%E3%82%AF%E3%83%A9%E3%82%A4%E3%82%A2%E3%83%B3%E3%83%88%E5%AF%BE%E5%BF%9C "複数クライアント対応")（必須）
-   c) [セキュリティホール](https://exp1.inf.shizuoka.ac.jp/%E3%82%BB%E3%82%AD%E3%83%A5%E3%83%AA%E3%83%86%E3%82%A3%E3%83%9B%E3%83%BC%E3%83%AB "セキュリティホール")（必須＋発展）
-   d) [HTTPサーバ機能拡張](https://exp1.inf.shizuoka.ac.jp/HTTP%E3%82%B5%E3%83%BC%E3%83%90%E6%A9%9F%E8%83%BD%E6%8B%A1%E5%BC%B5 "HTTPサーバ機能拡張")（選択）

```{admonition} 本日の進捗確認チェックリスト
- 13:15: 第二部の達成基準を理解しグループ内で役割分担を話し合いましたか？
- 15:00: 複数クライアント対応（必須）の全てを実装しテストできましたか？
    - select, fork, pthread, Apache
    - 簡易性能比較
- 16:30: セキュリティホール（必須）の全てを実装し動作を確認しましたか？
- 17:00: 選択課題や発展課題への挑戦スケジュールを相談しましたか？
```

### \[6月1日\] HTTPサーバの高機能化(2)

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day8%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day8の目的と概要")
-   a) [第二部の実装で詰まった時の対処法](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E3%81%AE%E5%AE%9F%E8%A3%85%E3%81%A7%E8%A9%B0%E3%81%BE%E3%81%A3%E3%81%9F%E6%99%82%E3%81%AE%E5%AF%BE%E5%87%A6%E6%B3%95 "第二部の実装で詰まった時の対処法")
-   b) [第二部参考書籍](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E5%8F%82%E8%80%83%E6%9B%B8%E7%B1%8D "第二部参考書籍")
-   c) [第二部グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "第二部グループ発表会")に向けた準備

```{admonition} 本日の進捗確認チェックリスト
- 14:00: 進捗状況をグループメンバーでお互いに報告し合い，本日の進め方について十分意識共有できましたか？
- 15:00: 第二部の必須課題は全員しっかり理解できましたか？
- 16:00: 第二部の選択課題や発展課題のどれに誰が取り組むか決めましたか？
- 17:00: 発表資料の分担方法やスケジュールについて十分話し合って決めましたか？
  - 【グループ発表PPT〆 6/8（火）12:00】
```

### \[6月8日\] グループレビュー，第二部グループ発表会

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day9%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day9の目的と概要")
-   a) [グループレビュー](https://exp1.inf.shizuoka.ac.jp/%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E3%83%AC%E3%83%93%E3%83%A5%E3%83%BC "グループレビュー")
-   [第二部グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "第二部グループ発表会")
-   [第二部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第二部レポート")（初版提出: 6/12（土）23:59，最終版提出: 6/19（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: 各班の発表資料を眺めて投票を済ませましたか？
- 15:00: 他の班の発表内容や発表方法など参考になる部分を糧にしていますか？
- 16:00: 他の班の発表内容を踏まえて第二部レポート内容の改良や追加実装＆追評価を考えてますか？
- 17:00: 第二部レポートの各自担当部分を，どのように他班と差別化し各自レポートに整理するか議論しあいましたか？
  - 【個別：第二部レポート（初版）〆 6/12（土）23:59】
```

### \[6月15日\] 第二部レポートピアレビュー

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day10%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day10の目的と概要")
-   a) [ピアレビュー](https://exp1.inf.shizuoka.ac.jp/%E3%83%94%E3%82%A2%E3%83%AC%E3%83%93%E3%83%A5%E3%83%BC "ピアレビュー")
-   [第二部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%BA%8C%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第二部レポート")（初版提出: 6/12（土）23:59，最終版提出: 6/19（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: ピアレビューを実施しましたか？
- 15:00: それぞれどこを修正すべきか議論しましたか？
- 16:00: 再実験するなど修正すべき箇所を更新できましたか？
- 17:00: 第二部レポート（最終版）完成の目途はたちましたか？
  - 【個別：第二部レポート（最終版）〆 6/19（土）23:59】
```

## 第三部 Webシステムの性能

### \[6月22日\] Webシステムの基礎

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day11%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day11の目的と概要")
-   a) [第三部の準備](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%81%AE%E6%BA%96%E5%82%99 "第三部の準備")
-   b) [Webシステムの基礎](https://exp1.inf.shizuoka.ac.jp/Web%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0%E3%81%AE%E5%9F%BA%E7%A4%8E "Webシステムの基礎")（発展）
-   c) [サンプルプログラム(PHP)](https://exp1.inf.shizuoka.ac.jp/%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0(PHP) "サンプルプログラム(PHP)")
-   d) [サンプルプログラム(JavaScript)](https://exp1.inf.shizuoka.ac.jp/%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0(JavaScript) "サンプルプログラム(JavaScript)")
-   e) [ベンチマークソフトab（Apache Bench）](https://exp1.inf.shizuoka.ac.jp/%E3%83%99%E3%83%B3%E3%83%81%E3%83%9E%E3%83%BC%E3%82%AF%E3%82%BD%E3%83%95%E3%83%88ab%EF%BC%88Apache_Bench%EF%BC%89 "ベンチマークソフトab（Apache Bench）")（必須）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: 第三部に必要な環境は正常に動作していますか？
- 15:00: node.jsのHTTPサーバは動きましたか？
- 16:00: サンプルプログラム２つ（PHP，JavaScript）とも動きましたか？
- 17:00: サンプルプログラムの違いが何であるか，キャプチャしたトラフィックを用いて班内で議論し合いましたか？
```

### \[6月29日\] Webシステムの設計と実装

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day12%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day12の目的と概要")
-   a) [DBMSとの接続](https://exp1.inf.shizuoka.ac.jp/DBMS%E3%81%A8%E3%81%AE%E6%8E%A5%E7%B6%9A "DBMSとの接続")（必須）
-   b) [最終課題の詳細](https://exp1.inf.shizuoka.ac.jp/%E6%9C%80%E7%B5%82%E8%AA%B2%E9%A1%8C%E3%81%AE%E8%A9%B3%E7%B4%B0 "最終課題の詳細")
-   c) [第三部グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "第三部グループ発表会")
-   d) [第三部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第三部レポート")（最終版提出: 8/7（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: PHPからDBMSへアクセスできるようになりましたか？
- 15:00: 「可(C)」レベルの実装ができましたか？
- 16:00: 「良(B)」or 「優(A)」レベルの実装ができましたか？
- 17:00: 各自の実装の性能を分析しボトルネックが分かりましたか？
```

### \[7月6日\] ベンチマークと分析

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day13%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day13の目的と概要")
-   a) [Apache MPMについて](https://exp1.inf.shizuoka.ac.jp/Apache_MPM%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6 "Apache MPMについて")
-   b) [実装の参考となるリンク集](https://exp1.inf.shizuoka.ac.jp/%E5%AE%9F%E8%A3%85%E3%81%AE%E5%8F%82%E8%80%83%E3%81%A8%E3%81%AA%E3%82%8B%E3%83%AA%E3%83%B3%E3%82%AF%E9%9B%86 "実装の参考となるリンク集")
-   c) [WebSocketについて](https://exp1.inf.shizuoka.ac.jp/WebSocket%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6 "WebSocketについて")
-   d) [検討状況の確認](https://exp1.inf.shizuoka.ac.jp/%E6%A4%9C%E8%A8%8E%E7%8A%B6%E6%B3%81%E3%81%AE%E7%A2%BA%E8%AA%8D "検討状況の確認")（必須）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: 第三部の達成基準を理解しグループ内で役割分担を話し合いましたか？
- 15:00: 基礎課題と応用課題への挑戦スケジュールを相談しましたか？
- 17:00: 各課題の結果と問題点を班内で議論し合いましたか？
```

### \[7月13日 / 7月20日\] パフォーマンスチューニング

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day14%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day14の目的と概要")
-   [第三部グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "第三部グループ発表会")の準備（実施日：7月27日）
-   [第三部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第三部レポート")（最終版提出: 8/7（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 14:00: 進捗状況をグループメンバーでお互いに報告し合い，本日の進め方について十分意識共有できましたか？
- 15:00: 第三部の基礎課題は全員しっかり理解できましたか？
- 16:00: 第三部の応用課題の進捗状況を班内で共有しましたか？
- 17:00: 発表資料作成の分担方法・スケジュールについて十分話し合って決めましたか？
  - 【グループ発表ポスターPPT中間提出〆 7/20（火）23:59→ fsに提出】
  - 【グループ発表ポスターPPT〆 7/27（火）12:00→ fsに提出】
```

### \[7月27日\] グループレビュー，第三部グループ発表会

-   [目的と概要](https://exp1.inf.shizuoka.ac.jp/Day15%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A8%E6%A6%82%E8%A6%81 "Day15の目的と概要")
-   [最終グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E6%9C%80%E7%B5%82%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "最終グループ発表会")（本日）
-   [第三部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第三部レポート")（最終版提出: 8/7（土）23:59）

```{admonition} 本日の進捗確認チェックリスト
- 12:45: 発表の準備は完了しましたか？
- 17:00: 実験機材のクリーンアップと解体をいつ実施し誰がメール報告するか決めて，リーダはメールで連絡しましたか？
  - 【個別：第三部レポート（最終版）〆 8/7（火）23:59】
```

## 実験終了後（必須）

- [実験機材のクリーンアップと解体](end/cleanup)　※必須課題

## その他

### トラブルシューティング

- [質問の行い方](trouble_shooting/question)
- [よくあるトラブル](trouble_shooting/trouble)
- [各アプリケーションのアンインストール方法](trouble_shooting/uninstall)

### スタッフ向け情報

- [内部向け:各回の進め方](staff/procedure)
- [内部向け:備忘録(次年度への引継ぎメモ)](staff/memo)

<!-- -   [内部向け:各回の進め方](https://exp1.inf.shizuoka.ac.jp/%E5%86%85%E9%83%A8%E5%90%91%E3%81%91:%E5%90%84%E5%9B%9E%E3%81%AE%E9%80%B2%E3%82%81%E6%96%B9 "内部向け:各回の進め方")
-   [内部向け:備忘録(次年度への引継ぎメモ)](https://exp1.inf.shizuoka.ac.jp/%E5%86%85%E9%83%A8%E5%90%91%E3%81%91:%E5%82%99%E5%BF%98%E9%8C%B2(%E6%AC%A1%E5%B9%B4%E5%BA%A6%E3%81%B8%E3%81%AE%E5%BC%95%E7%B6%99%E3%81%8E%E3%83%A1%E3%83%A2) "内部向け:備忘録(次年度への引継ぎメモ)") -->

### 過去のページ

- [2017-21年度](https://exp1.inf.shizuoka.ac.jp/Home)
- [2015-6年度](http://exp1.inf.shizuoka.ac.jp/index.php?title=Home&oldid=875)
- [2014年度](http://www.minelab.jp/NWprog-CSexp1/2014/)
- [2013年度](http://www.minelab.jp/NWprog-CSexp1/2013/)