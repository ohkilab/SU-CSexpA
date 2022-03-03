# 内部向け:備忘録(次年度への引継ぎメモ)

## 2021年度

### 連絡

### ソフトウェア情報

## 2020年度

### 連絡

-   今年度は新型コロナウィルスの影響により本実験を在宅で行うこととなりました.

今年度では以下のように対応しました.

-   資料→本実験ページ（例年通り）
-   連絡事項・質問→Microsoft Teams
-   学生間連絡→Microsoft Teams
-   発表→Remo([グループ発表会](https://exp1.inf.shizuoka.ac.jp/%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E7%99%BA%E8%A1%A8%E4%BC%9A "グループ発表会"))
-   レポート提出→Live Campus（例年通り）
-   ラズパイの配布・回収→対面（例年通り）＋郵送

-   オンラインでの実施のため, グループワークにおいて班員との進捗確認が取りにくい（返信しない学生がいる）ので, 解決策を検討中です.
-   ラズパイのオペレーティングシステム「Raspbian」の正式名称が2020年5月に「Raspberry Pi OS」に変更されました.  
    

それに伴い, 実験ページ全体で「Raspbian」の記述を「Raspberry Pi OS（旧：Raspbian）」に変更しました.

参考：[https://www.raspberrypi.org/downloads/](https://www.raspberrypi.org/downloads/)

-   毎年度ごとにこちらの備忘録のページとトラブルシューティングのページの更新をお願いします.(今年度まで何も記述がなかったので...)
-   \[済\]第二部で使用する実験共通ライブラリexp1lib.c内のexp1\_tcp\_connect関数が原因となっているコアダンプが多発するという不具合がありました. 修正をしましたので動作確認お願いします.(7月, 野口先生)
-   \[済\]第三部で使用するcsvファイル内部に不正なデータが存在します. 修正をお願いします.([最終課題の詳細 データセットのダウンロード](https://exp1.inf.shizuoka.ac.jp/%E6%9C%80%E7%B5%82%E8%AA%B2%E9%A1%8C%E3%81%AE%E8%A9%B3%E7%B4%B0#.E3.83.87.E3.83.BC.E3.82.BF.E3.82.BB.E3.83.83.E3.83.88.E3.81.AE.E3.83.80.E3.82.A6.E3.83.B3.E3.83.AD.E3.83.BC.E3.83.89 "最終課題の詳細"))

```
1622611:zî1acspmsftiecsrgbööóhpcprtp3desclwtptðbkptrxyzgxyzbxyzdmndtpdmddävuedlviewôlumiømeastech0rtrcgtrcbtrctextcopyrightc1998hewlettpackardcompanydescsr},,,,
6292424:nsavedstevtinstanceidxmpiid0a801174072068118083b39db25ae1c9stevtwhen20120831t0937170700stevtsoftwareagentadobephotoshoplightroom41macintoshstevtchanged},,,,

```

-   第三部で行うCSVファイルのDBへの流し込み作業にかなりの時間を取られるため, 解決策を検討中です.([最終課題の詳細 テーブルの作成](https://exp1.inf.shizuoka.ac.jp/%E6%9C%80%E7%B5%82%E8%AA%B2%E9%A1%8C%E3%81%AE%E8%A9%B3%E7%B4%B0#.E3.83.86.E3.83.BC.E3.83.96.E3.83.AB.E3.81.AE.E4.BD.9C.E6.88.90 "最終課題の詳細"))

-   配布するイメージに最初から入れておく
-   HeidiSQL(データベース管理ソフト)を利用
-   Creat文の見直し

など

-   できれば実験用ラズパイ全台にヒートシンクの装着をお願いします.
-   第三部の[住所検索アプリの作成](https://exp1.inf.shizuoka.ac.jp/DBMS%E3%81%A8%E3%81%AE%E6%8E%A5%E7%B6%9A#.E5.90.84.E7.8F.AD.E3.81.A7.E5.AE.9F.E8.A3.85.E7.8A.B6.E6.B3.81.E7.A2.BA.E8.AA.8D "DBMSとの接続")が[第三部レポート](https://exp1.inf.shizuoka.ac.jp/%E7%AC%AC%E4%B8%89%E9%83%A8%E3%83%AC%E3%83%9D%E3%83%BC%E3%83%88 "第三部レポート")の項目に含まれていませんでした.
-   mediawikiのVer.1.35.0が9月25日にリリースされ, こちらのページもアップデートされてました(更新日不明).

Ver.1.35.0バグ？

-   thumbやframeの画像が左端に表示される.(右に設置できない)
-   表のclass「wikitable」では通常罫線が表示されるが, 表示されなくなった.
-   ページ上部に表示されていた実験教員等の情報が表示されなくなった. ([MediaWiki:Hf-nsheader-](https://exp1.inf.shizuoka.ac.jp/MediaWiki:Hf-nsheader- "MediaWiki:Hf-nsheader-"))
-   Chromeからアクセスできない(場合がある). (net::ERR\_HTTP2\_PROTOCOL\_ERROR 200)

### 新規・更新ページ情報

-   [RaspberryPiの接続（固定IP)](https://exp1.inf.shizuoka.ac.jp/RaspberryPi%E3%81%AE%E6%8E%A5%E7%B6%9A%EF%BC%88%E5%9B%BA%E5%AE%9AIP%EF%BC%89#.E3.83.86.E3.82.AD.E3.82.B9.E3.83.88.E3.82.A8.E3.83.87.E3.82.A3.E3.82.BF.E3.82.92.E4.BD.BF.E3.81.A3.E3.81.9F.E6.8E.A5.E7.B6.9A_.28.E4.BB.BB.E6.84.8F.29 "RaspberryPiの接続（固定IP）")
-   [Atomを使った環境構築](https://exp1.inf.shizuoka.ac.jp/Atom%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%9F%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89 "Atomを使った環境構築")
-   [Raspberry Piについて](https://exp1.inf.shizuoka.ac.jp/Raspberry_Pi%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6 "Raspberry Piについて")
-   [よくあるトラブル(第１部)](https://exp1.inf.shizuoka.ac.jp/%E3%82%88%E3%81%8F%E3%81%82%E3%82%8B%E3%83%88%E3%83%A9%E3%83%96%E3%83%AB(%E7%AC%AC%EF%BC%91%E9%83%A8) "よくあるトラブル(第１部)")\[編集中\]
-   [よくあるトラブル(第２部)](https://exp1.inf.shizuoka.ac.jp/%E3%82%88%E3%81%8F%E3%81%82%E3%82%8B%E3%83%88%E3%83%A9%E3%83%96%E3%83%AB(%E7%AC%AC%EF%BC%92%E9%83%A8) "よくあるトラブル(第２部)")\[編集中\]
-   [よくあるトラブル(第３部)](https://exp1.inf.shizuoka.ac.jp/%E3%82%88%E3%81%8F%E3%81%82%E3%82%8B%E3%83%88%E3%83%A9%E3%83%96%E3%83%AB(%E7%AC%AC%EF%BC%93%E9%83%A8) "よくあるトラブル(第３部)")\[編集中\]
-   [各アプリケーションのアンインストール方法](https://exp1.inf.shizuoka.ac.jp/%E5%90%84%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E3%81%AE%E3%82%A2%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E6%96%B9%E6%B3%95 "各アプリケーションのアンインストール方法")\[編集中\]
-   [未定](https://exp1.inf.shizuoka.ac.jp/%E6%9C%AA%E5%AE%9A "未定")\[編集中\]
-   [全テンプレート](https://exp1.inf.shizuoka.ac.jp/%E7%89%B9%E5%88%A5:%E3%83%9A%E3%83%BC%E3%82%B8%E4%B8%80%E8%A6%A7?from=&to=&namespace=10)

### ソフトウェア情報

-   Raspberry Pi OS（旧：Raspbian）

```
$ cat \etc\debian_version
10.3

$lsb_release -a
No LSB modules are available.
Distributor ID:Raspbian
Description:Raspbian GNU/Linux 10 (buster)
Release:10
Codename:buster
```

公式サイト：[https://www.raspberrypi.org/downloads/](https://www.raspberrypi.org/downloads/)

-   mariaDB

-   Apache

-   ApacheBench