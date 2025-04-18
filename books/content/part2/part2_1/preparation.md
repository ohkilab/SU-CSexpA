# 第二部の準備

## 第二部の概要

第二部では，「Webブラウザ<=>Webサーバ<=>DBMS」から構成される三層クライアントサーバ型の一般的なWebシステムの構築方法を理解するだけでなく，これまで勉強してきた専門科目やインターネット上の最新技術，情報を駆使してグループで相談しながら，膨大なリクエストに耐えられる超高性能なWebシステムにするにはどうしたらよいのか考えて実現してもらいます．


具体的には，1000万行程の大きなデータ（CSV形式）を2つ用意しましたので，Webブラウザで検索キーワードを入力すると，これら2ファイルを統合的に検索し結果をソートして表示するWebアプリケーションを実装してもらいます．

何も工夫せず安直に実装すると検索結果が表示されるまで数分かかってしまうこともありますので，三層クライアントサーバ型のWebシステムにおいて，どこにボトルネックが生じうるのか，どのようにボトルネックの箇所が遷移していくのか，グループ内で議論や試行錯誤し，できるだけ多くのリクエストに耐えられる超高性能なWebシステムを実現できるようグループ間で競ってもらいます．

様々なアプローチが考えられますが，大きくオンメモリ型か非オンメモリ型かでそれぞれ超高性能なWebシステムを目指してほしいと思います（どちらか一方のみでも構いません）．


第二部最後のグループ発表会では，各グループの実現した超高性能なWebシステムについて特徴を紹介してもらい，その性能測定結果をアピールしてもらいます．

どのような工夫で性能向上させるかは，世の中で実際に使用されうる手法であれば何でも構いません.
（もし実運用に耐えられないような小手先の仕組みの場合は，参考という形でアピールしてください．実施した内容や検討した内容はポジティブに受け取りますので）


最終課題は，グループ発表会と個別レポートで評価しますので，チームワークだけでなく主体性，独自性も養ってください．
また，これまでのグループワークを通し，実装の得意な人，計測や考察の得意な人，自由な発想で取り組みたい人，などそれぞれ何となく自身を分析できるようになってきたと思います．

まずは，下記の基礎課題全てをグループ内で議論しながら実施してもらいますが，以下の応用課題については各自の興味関心，得意分野に応じて，それぞれ複数人になるよう相談して決めてください.
（半々で実施する必要はありませんし，重複していても構いませんが「二兎を追う者は一兎をも得ず」にならないようご注意ください）

-   基礎課題（必須）
-   応用課題（分担で①は必須，②は任意）
    -   ①必須：Webシステムの**超高性能化**に重点的に取り組む人(**①超高性能化（ハイパフォーマンス）チーム**)
    -   ②任意：**超興味深い**Webシステムの自由課題に重点的に取り組む人(**②自由課題（イマジネーション）チーム**)

それでは，いきなりWebシステムを構築するのも難しいので，以下のステップに従って順を追って第二部の最終課題であるWebシステムを構築していきましょう．

## 第二部で利用する端末について

端末は第一部でセットアップした RaspberryPi を引き続き利用します． 今回もインターネットに接続しますので，第一部「[Raspberry Piのセットアップ](../../part1/part1_1/raspberry_setting "Raspberry Piのセットアップ")」を思い出しながら作業してください．

```{admonition} 補足
インターネットに接続することになりますので，Raspberry Piのソフトウェアの更新を適度におこなってください．
```
## Gitのインストール

まず，プログラムのソースコードなどの変更履歴を記録・追跡するための分散型バージョン管理システムの一つである[Git（ギット）](http://git-scm.com/)をインストールします．

もともとLinuxの開発チームが使用していたGitは，昨今では世界中の技術者に広まってバージョン管理システムの主流となっています．Gitを用いると，普段使っている自分のノートPCといったローカル環境に，過去の全変更履歴を含む完全なリポジトリ（貯蔵庫）の複製を作成できます．つまり，各ローカル環境がリポジトリのサーバとして動作できることになります．これまでの分散型でないバージョン管理システムでは，サーバ上にある1つのリポジトリを利用者が共同で使用するため，利用者が増えると変更内容が衝突し整合性の維持が大変でした．Gitでは，ローカル環境にもコードの変更履歴を保存（コミット）するので，リモートのサーバに常に接続する必要はなく，ネットワークに接続していなくても作業を行えます．

[GitHub(ギットハブ)](https://github.com/)は，このGitの仕組みを利用して世界中の人々が開発物を自由に保存，公開できるようにしたWebサービスの名称です．GitHubは，GitHub社によって運営されており，個人・企業問わず無料で利用できます．GitHub上のリポジトリは，基本的に全て公開されますが，有料サービスを利用すると指定ユーザーのみアクセスできるプライベートなリポジトリを作ることもできます．リポジトリに記録するには，コミットという操作を行います．コミットは時系列順につながった状態でリポジトリに格納されていますので，最新のコミットから辿ることで過去の変更履歴やその内容を知ることができます．

この実験では，主にサンプルソースコードのダウンロードのためにGitを使用しますが，もちろん本来の使い方であるオープンソースソフトウェアの開発を意識して，皆さんの作成したソフトウェアを公開して自己アピールしていただいても構いません（ただし，ソフトウェア公開は各自の責任で実施してください）．

### サーバ端末（OS: Raspberry Pi OS）へのインストール（必須）

最初のOSインストール時に一緒にgitもインストールされている可能性もあります．端末画面（コマンドライン）で「git」と入力し，コマンドマニュアルが表示された場合は以下の手順は不要です．

```sh
 $ sudo apt install git
```

インストールが完了したら git のバージョンを確認してみましょう．

```sh
 $ git --version
git version 2.11.0

```

### 各自ノートPC上のゲストOS（Ubuntu仮想環境）へのインストール

各自のノートPC上で動作するゲストOS（Ubuntu）へGitをインストールするのも簡単です．以下のような感じでインストールできると思います（必要に応じてPPA (Personal Package Archive)リポジトリを追加してください）．

```sh
 $ sudo apt update
 $ sudo apt install git
```

aptによるgitのインストールが成功しない場合は様々な問題が考えられます． まずはブラウザ等でインターネットに接続できているか確認をしましょう． apt updateによるパッケージリストの更新が成功しているか，ゲストマシンのネットワーク設定が間違っていないか（ブリッジの場合はアダプタが適当か），使っているUbuntuはサポートが終了したバージョンではないか等の確認をしましょう．

ユーザ名やメールアドレスの登録は以下のように入力します．

```sh
 $ git config --global user.name "User Name"
 $ git config --global user.email "hoge@hoge.com"
```

### 各自ノートPC上のホストOS（Windows）へのインストール

Windows端末でGitを使用するには，[GitHub Desktop](https://desktop.github.com/)をインストールするのが簡単です．GUIもCUI（Windows Powershell）も同時にインストールされますのでお手軽です．

-   [GitHub DesktopはWindows用Gitツール インストールと使い方の解説](https://ferret-plus.com/8498)
-   [GitHub Desktop を使って楽々 GitHub 入門](http://yohshiy.blog.fc2.com/blog-entry-326.html)

などなど

これ以外にも SourceTree や Fork などいくつかクロスプラットフォームで使えるGitクライアントがありますので，お好みで使ってみてください（チーム内では同じソフトで統一することをおすすめします）．

-   [SourceTree](https://www.sourcetreeapp.com/)
-   [Fork](https://git-fork.com/)
