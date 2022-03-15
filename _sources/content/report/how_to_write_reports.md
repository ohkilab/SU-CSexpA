# レポートの書き方

## ツール紹介

レポートを作成する際に便利なツールを紹介します．

このツールを利用することが必須ではありません．むしろ，自分の方法にあったツールを自分で探す（探す方法を確立する）方がより重要です．

### TeX

例えば，TeXworksは，TeXでドキュメントを作成するためのシンプルな統合環境です．macOS用の優れた統合環境であるTeXShopをモデルに，同様の環境を全てのシステム上で実現すべく開発されています．

- [TeX Wiki](https://texwiki.texjp.org/)
- [TeX Wiki: TeX入手方法](https://texwiki.texjp.org/?TeX%E5%85%A5%E6%89%8B%E6%B3%95)
- [TeX Wiki: LaTeX入門](https://texwiki.texjp.org/?LaTeX%E5%85%A5%E9%96%80)

### 図・グラフなどの作成（TeXの場合）

図・グラフなどは各自が選んだソフトウェアで作成してください．ただし，TeXに取り込む場合には eps 形式に変換する必要があります（他の形式（例えば画像形式やPDF形式）でも不可能ではないです）．

-   [PowerPointからTeX用のepsファイルを生成する方法](http://kano.arkoak.com/2015/09/26/eps/)
-   [Metafile to EPS Converter で Windows Metafile を EPS に変換](http://qiita.com/tubo28/items/1f8672fb164aafa9ab31)
    -   [Meta file to EPS Converter](https://wiki.lyx.org/Windows/MetafileToEPSConverter)
-   [How to Convert PowerPoint Pictures for Use in LaTeX](https://www.cs.bu.edu/~reyzin/pictips.html)

### シーケンス図などの UML の図の作成

シーケンス図などの UML で定義された図を作成する場合は，Illustrator や PowerPoint などのドローソフトで作成しても良いですが，モデリングツールを利用しても良いでしょう．

- [Astah\* community edition](http://astah.change-vision.com/ja/product/astah-community.html)
- [MS Visio: UML シーケンス図を作成する](https://support.office.com/ja-jp/article/UML-%E3%82%B7%E3%83%BC%E3%82%B1%E3%83%B3%E3%82%B9%E5%9B%B3%E3%82%92%E4%BD%9C%E6%88%90%E3%81%99%E3%82%8B-c61c371b-b150-4958-b128-902000133b26)

### ネットワーク構成図の作成

Illustrator や PowerPoint などのドローソフトとルータ等の機材のフリー画像（PowerPoint の場合は「挿入 → ネットワーク画像」から "ネットワーク機器" などのキーワードで検索（クリエイティブ・コモンズライセンスに限定））を使って作成しても良いですが，ネットワーク構成図を作成する機能を持つアプリケーションを使っても良いでしょう．

- [IT エンジニア向け Visio シェイプ](http://www.microsoft.com/ja-jp/visio/free/ite.aspx)
  - MS Visio に同梱されているネットワーク関係のステンシルでも十分だが，特定メーカの機器のステンシルを使いたい場合はここからダウンロード．
- [ネットワーク構成図を無料で作成する\[アイコン/素材\]](http://www.petitmonte.com/network/network_diagram.html)
- [draw.io](https://www.draw.io/)
  - 新規に作成するダイアグラムの種類から Network Diagram を作成
- [blockdiag - simple diagram images generator](http://blockdiag.com/en/)
  - テキストベースでグラフを作成するツール．ネットワーク構成図を作るには[nwdiag](http://blockdiag.com/en/nwdiag/index.html)を用いると良い．

### スクリーンショットの撮り方

画面のスクリーンショットを撮る場合は OS 標準の方法と専用のアプリケーションを導入する方法があります．機能に多少の違いがありますので，必要なスクリーンショットに合わせて使い分けてください．

レポートにスクリーンショットを貼り付ける場合のファイルフォーマットは PNG にします（JPG のような印刷時に劣化するフォーマットを避けるという意図です）．

- Windows 標準の方法
  - [Windows のスクリーンショットの撮り方](https://allabout.co.jp/gm/gc/20843/)
  - [Snipping Tool を使ってスクリーン ショットをキャプチャする](https://support.microsoft.com/ja-jp/help/13776/windows-use-snipping-tool-to-capture-screenshots)
- 専用アプリケーションの例
  - [Lightscreen](http://lightscreen.com.ar/)

### PDFの作成

-   MS Officeの場合
    -   直接PDFファイルをエクスポート可能です．
        -   参考: [PDFに保存または変換する](https://support.office.com/ja-jp/article/PDF-%E3%81%AB%E4%BF%9D%E5%AD%98%E3%81%BE%E3%81%9F%E3%81%AF%E5%A4%89%E6%8F%9B%E3%81%99%E3%82%8B-d85416c5-7d77-4fd6-a216-6f4bf7c7c110)
-   TeXの場合
    -   DVI形式のファイルに対して dvipdfmx コマンドを使ってPDFに変換します．

## 提出前のチェックリスト

### 構成

- 使用機材は明記しているか？
  - 最低限，読んだ人が実験を再現できるだけの情報が必要です．
- グループメンバーのコメントは全て削除されているか？
　- レポートは授業におけるあなたの成果をまとめた文書です．体裁を整えることを忘れずに．
- 変更履歴は反映されているか？
　- 上記と同じ理由です．

### 書式

- 指定したフォーマットを使用しているか？
  - 学会，会社，行政文書など，すべてに規定されたフォーマットがあります．
  - プリントフォーマットが一致しているだけではなく，ドキュメント上で指定されたスタイルが指定されている必要があります．
- すべての図と表にキャプションがついているか？
- すべての図と表は本文中で引用されているか？
- すべての図と表は本文中で引用されている箇所が明確か？
  - 読者が意図通りの箇所を参照できるような本文中の表現，図・表の表現が必要です．特に図・表に含まれる情報量が過剰な場合，読者は混乱します．本文の論旨に必要な箇所を本文中の図・表とし，全体の情報は付録で提示するなどの工夫を検討してください．
- 図・表のキャプションの書式は適正か？
  - 図のキャプションは図の下側に配置．
  - 表のキャプションは表の上側に配置．
- すべての参考文献は本文中で引用されているか？
- フォントの種類やサイズが統一されているか？
  - 文書全体で 1 つのフォントに統一する必要はありませんが，第 1 章の見出しはボールド体，第 2 章の見出しは明朝体などといった不整合は許容されません．
- 句読点は統一されているか？ (、。もしくは，．のいずれかに統一すること)
  - 半角のカンマ・ピリオド(, . )を使う場合には直後にスペースが必要です．

### 文体

- 箇条書きを必要以上に多用していないか？
- こそあど言葉(これ，それ，このなど)を多用していないか？
  - 読者が参照先に迷うため，極力こそあど言葉は避けてください．
  - 具体的な名詞を使って代替することができるはずです．
- 接続詞(そのため，しかしなど)を必要以上に多用していないか？
  - 接続詞を多用しなければならないときは文章の構成がおかしい場合が多いです．
  - 一般的には接続詞は続く文章を強調したい場合に使います．
- 必要以上に 1 文が長くなっていないか？
  - 「～し，」という表現を多用していないか？
    - 「～し，」という言葉を使うと一見文章がつながってみますが，実は論理的につながっていない場合が多いです．
    - 「～し，」という表現を使いそうになったら，2 つの文章に分けることができないか考えてみてください．

### グラフ

- 適切なグラフの種類を選択しているか？
  - 用途に応じてグラフの種類が存在します．例えば，折れ線グラフと散布図は見かけ上類似点がありますが，利用目的が異なります．折れ線グラフは 1 つのものごとの移り変わりを表現する場合（典型的には時系列データ），散布図は 2 つの数値データの関係を表現する場合に採用します．これ以外のグラフについても，用途に合わせて適切なグラフの種類を選択してください．
    - 参考: [Excel2010：折れ線グラフと散布図の違いとは](http://office-qa.com/Excel/ex194.htm)
- 軸ラベルが用意されているか？
- 軸ラベルには単位が記載されているか？
- グラフ中の値を読むことができるサイズ・品質か？
  - 対数表示の利用も選択肢のひとつです．
    - MS Excel の場合は軸の書式設定で「対数目盛を表示する」をチェックします．

![graph.jpg](../../images/part3/part3_2/graph.jpeg)

## MS Word の参考リンク

- [Word はなぜ思い通りにならないのか?](https://news.mynavi.jp/series/word)
  - 特に以下は参考になります．
    - 第 1 回 画像を好きな場所に配置できない
    - 第 6 回 インデントとぶら下げ
    - 第 14 回～ 16 回のスタイル関係
    - 第 19 回 アウトラインレベルの指定
    - 第 21 回 目次の作成
    - 第 22 回 箇条書きとリストのレベル
    - 第 24 回 改ページ位置の自動修正
    - 第 33 回 変更履歴の活用
- [Word 入門へようこそ](http://word.eins-z.jp/)
  - 特に以下は参考になります．
    - スタイルの適用、スタイルの変更
    - 段落番号・箇条書き・アウトライン
    - 図の挿入と調整
    - グラフの挿入
    - ページ番号
    - 図形の挿入
    - 表の挿入と体裁
    - 目次の作成と更新
    - 引用文献・文献目録・資料文献の管理
    - 図表番号・図表目次・更新・相互参照
    - コメントの活用
    - 編集履歴の記録
    - アウトライン（ただし，アウトラインは，スタイルをきちんと設定していてこその機能です）
- [Excel と Word で業務効率化！情報技術ライダー BLOG](http://siland.jp/note/word/%E6%A8%99%E6%BA%96%E3%81%AE%E3%82%B9%E3%82%BF%E3%82%A4%E3%83%AB%E3%81%AE%E5%BD%B9%E5%89%B2%E3%81%A8%E3%80%81%E6%AE%B5%E8%90%BD%E3%81%8C%E4%BD%95%E3%81%8B%E3%82%92/%7C%E3%80%90%E3%83%AF%E3%83%BC%E3%83%89%E8%B6%85%E5%9F%BA%E7%A4%8E%EF%BC%91%E3%80%91%E6%A8%99%E6%BA%96%E3%81%AE%E3%82%B9%E3%82%BF%E3%82%A4%E3%83%AB%E3%81%A8%E6%AE%B5%E8%90%BD%E3%82%92%E7%9F%A5%E3%82%89%E3%81%AA%E3%81%84%E3%81%A8%E3%83%AF%E3%83%BC%E3%83%89%E3%81%AF%E8%B6%85%E4%B8%8D%E4%BE%BF%E3%81%AA%E3%82%93%E3%81%A7%E3%81%99)
- [Word（ワード）で引用文献や参考文献を挿入、管理する方法](https://prau-pc.jp/word/insert-citation/)

## 補足：Teams 課題機能によるレポート提出方法

![teams report 001.png](../../images/part1/report/600px-teams_report_001.png)

左側のタブから「課題」を選択し，「割り当て済み」の課題から対象のレポートを選択します

![teams report 002.png](../../images/part1/report/600px-teams_report_002.png)

「作業の追加」をクリックして提出するファイルをアップロードします．

![teams report 003.png](../../images/part1/report/600px-teams_report_003.png)

ファイルがアップロードされていることを確認したら，右上の「提出」ボタンをクリックします

![teams report 004.png](../../images/part1/report/600px-teams_report_004.png)

右上の提出ボタンが「提出を取り消す」に変わって，提出日時が表示されていたら提出完了です．

## 参考文献

- [日本語の作文技術 (朝日文庫): 本多 勝一](https://www.amazon.co.jp/dp/4022608080/)
- [【2019 年版】APA スタイルでの参考文献の書き方と引用の仕方](https://studyusa-log.com/apa-style-citation)
- [APA Style Introduction](https://owl.purdue.edu/owl/research_and_citation/apa6_style/apa_style_introduction.html)
- [参考文献の役割と書き方(SIST)](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=4&cad=rja&uact=8&ved=0ahUKEwi927ShrdLaAhXKH5QKHYXYDqcQFghbMAM&url=https%3A%2F%2Fjipsti.jst.go.jp%2Fsist%2Fpdf%2FSIST_booklet2011.pdf&usg=AOvVaw0KygsN7cVz5UnIJoRTU22f)
- [\[改訂第7版］LaTeX2ε美文書作成入門](http://gihyo.jp/book/2017/978-4-7741-8705-1)