# DBMSとの接続

## はじめに

一般的なWebシステムは，「クライアント⇔Webサーバ⇔DBMS」という三層構造で構築されることが多いです．
ちなみに，Webサーバが内部的にサーバサイドのプログラムを実行し動的にコンテンツを生成するような場合，Webサーバと呼ばずにアプリケーションサーバ（APサーバ）と呼ばれることもあります（そのような製品群があります）．

それでは，いよいよクライアントからのリクエストに対し，WebサーバがDBMS (Data Base Management System)とやり取りして返信するコンテンツを動的に生成する仕組みを実体験していきましょう．

## MariaDBの基礎

MariaDB(マリアディービー)とは，「MySQL」から派生したオープンソースのRDBMS（リレーショナルデータベース管理システム）です．拡張性/処理性能/高品質といった特徴を持ち，世界的に使用されているデファクト的な存在で，数多くのLinuxディストリビューションでも採用されています（サービス名はmariadbですが，設定ファイルやコマンドなどmysqlを踏襲してます）．

MariaDBがまだインストールされていない人は以下のようにインストールしておいてください．

```sh
# Raspberry Piの場合
$ sudo apt install mariadb-server
# Fedoraの場合
$ sudo yum install mariadb-server

# 以下共通
$ sudo systemctl enable mariadb
$ sudo systemctl start mariadb

# 初期設定（初期状態ではrootパスワードは設定されていないので，何も入力せず<b>ENTER</b>を押す．すると新規にrootパスワード設定できる．以降は全て<b>Y</b>でOK）
$ sudo mysql_secure_installation

```

インストールできたら，以下のようにデータベースとデータベースユーザを作成します．データベース名とユーザ名は，各自で好きな名前を付けてください．以下の例では，データベース名「**CSexp1DB**」，ユーザ名「**shizutaro**」を作成しています．

```sh
# データベースユーザ作成のために rootでmysqlにログイン
$ mysql -u root -p
# MariaDBのrootパスワードを入力

データベース「CSexp1DB」を作成
> CREATE DATABASE CSexp1DB;

--全てのSQL文を発行できるユーザ「shizutaro」の作成（パスワードのhogehogeは適当に決めて書き換えてください）
> GRANT ALL PRIVILEGES ON CSexp1DB.* TO shizutaro@localhost IDENTIFIED BY 'hogehoge';
> FLUSH PRIVILEGES;

--mysqlからshellのコマンドラインへ戻るには exit or quit
> exit
```

ここで，もし**MariaDBのrootのパスワードが不明な場合**は，以下の手順で再設定してください．

```sh
# MariaDBのサービスを停める
$ sudo systemctl stop mariadb.service

# MariaDBをセーフモードで立ち上げる
$ sudo mysqld_safe --skip-grant-tables &

# MariaDBへrootでログイン
$ mysql -u root

# rootのパスワードを設定（MariaDBのコマンドライン上で）
MariaDB [(none)]> use mysql;
MariaDB [(mysql)]> select host,user,password,plugin from user;                         # 念のため確認
MariaDB [(mysql)]> update user set password=password('ここにパスワードを記述') where User='root';
MariaDB [(mysql)]> update user set plugin='' where User='root';                        # unix_socketを無効に

MariaDB [(mysql)]> flush privileges;
MariaDB [(mysql)]> exit;

# 簡単のため再起動（もちろんセーフモードで起動したMariaDBをkillしてもOK）
$ sudo reboot
```

## コマンドラインからデータベースへアクセスする方法

次にコマンドラインからデータベースへアクセスする方法を復習してみましょう．

ここでは，静岡県の郵便番号リストの登録されたデータベースを作成し，入力した住所の郵便番号を返すようなWebアプリケーションを実装してみましょう．例えば，Webブラウザ上で「城北」と入力すると，住所に城北を含んだ郵便番号のリストが結果のWebページに表示されるというイメージです．

### 静岡県の郵便番号一覧をダウンロード

郵便番号のリストは，[ゆうちょのページ](http://www.post.japanpost.jp/zipcode/download.html)で公開されています．ただし，半角カタカナだったり，特に今回は不要なフィールドが含まれていたりしますので，静岡県の郵便番号と住所を「,」で区切ったデータのみを抽出し，以下のGitリポジトリへ`zip-shizuoka.csv`というファイルで置いておきました．

- GitHubリポジトリ [https://github.com/ohkilab/SU-CSexpA-02](https://github.com/ohkilab/SU-CSexpA-02)

### テーブルの作成

データベースにデータを保存するにはまずテーブルを作らなくてはいけません．

テーブルは，SQLの**CREATE TABLE**文で作れます．まずMySQLに先ほど作成したユーザ「shizutaro」権限でログインしましょう．

```sh
 $ mysql -D CSexp1DB -u shizutaro -p

```

MySQLのプロンプトが返ってきたら，SQLコマンドを受け付ける状態になっています．ダウンロードしたCSVファイルをじっくり眺めて，スキーマを決めましょう．スキーマを決めたら，**CREATE TABLE**文を作ってコマンドラインから発行しましょう．

-   CREATE文を作る際に考慮すべき点
    -   フィールドは何個あるか？
    -   各フィールドの名前を何にするか？
    -   各フィールドの[データ型](https://dev.mysql.com/doc/refman/5.6/ja/data-types.html)を何にするか？

以降の作業で，グループ内で相談しやすいよう以下のようにテーブル名と列名を共通にしておきましょう．

-   テーブル名：**zipShizuoka**
-   列名：以下を参照

```{list-table} スキーマ例
:header-rows: 1
:name: schema

* - 列名
  - 概要
  - 例
* - zip
  - 郵便番号
  - 4328011
* - kana1
  - 都道府県名(全角カナ)
  - シズオカケン
* - kana2
  - 市区町村名(全角カナ)
  - ハママツシナカク
* - kana3
  - その他住所(全角カナ)
  - ジョウホク
* - addr1
  - 都道府県名
  - 静岡県
* - addr2
  - 市区町村名
  - 浜松市中区
* - addr3
  - その他住所
  - 城北
```
**CREATE TABLE**文そのものを忘れてしまった人は，インターネットから情報を集めて復習しましょう．

-   参考1 [https://dev.mysql.com/doc/refman/8.0/en/create-table.html](https://dev.mysql.com/doc/refman/8.0/en/create-table.html)
-   参考2 [http://memopad.bitter.jp/w3c/sql/sql\_create\_table.html](http://memopad.bitter.jp/w3c/sql/sql_create_table.html)
-   参考3 [http://yoshinin.blog.fc2.com/blog-entry-126.html](http://yoshinin.blog.fc2.com/blog-entry-126.html)

### CSVファイルからデータをテーブルへ流し込む

テーブルにデータを追加するには**INSERT文**を使いました．ただ，膨大な量のデータを一行一行**INSERT文**で挿入するのはとても大変です．そこで大抵のデータベースには，ファイルからデータを一括でテーブルへ流し込むコマンドがあります．MySQLだと以下のように実行します．

```sql
> load data local infile "/path/import.csv" into table table_name fields terminated by ',' lines terminated by '\r\n';
```

-   CSVファイルのパス（上記例だと`/path/import.csv`）は，git cloneをホームディレクトリで実行したなら`/home/各自のアカウント名/CSexp1-02/zip-shizuoka.csv`というような感じ
-   **table\_name**はこの課題では，zipShizuoka

パスやテーブル名は，皆さんの環境に合わせて修正してください．ここで，CSVファイルのパスは，**絶対パス**で記述しましょう．相対パスでも書けますが，MySQLのデータファイルのある位置からの相対パスとなりますので，なかなか期待通りには書けないと思いますから．

エラーが表示された場合は，例えば，データ型が一致していないといったスキーマの誤りである可能性があります．ご注意ください．

### テーブルに入ったデータの抽出

テーブルに入ったデータの抽出は，以下のように**SELECT文**で行います．いろいろ試してみてください．

```sql
> SELECT * FROM zipShizuoka LIMIT 3;
```

```sql
+---------+---------------+---------------------+-----------------------------+--------+------------+----------------------+
| zip     | kana1         | kana2               | kana3                       | addr1  | addr2      | addr3                |
+---------+---------------+---------------------+-----------------------------+--------+------------+----------------------+
| 4220000 | シズオカケン  | シズオカシアオイク  | イカニケイサイガ ナイバアイ | 静岡県 | 静岡市葵区 | 以下に掲載がない場合 |
| 4200838 | シズオカケン  | シズオカシアオイク  | アイオイチョウ              | 静岡県 | 静岡市葵区 | 相生町               |
| 4212309 | シズオカケン  | シズオカシアオイク  | アイブチ                    | 静岡県 | 静岡市葵区 | 相渕                 |
+---------+---------------+---------------------+-----------------------------+--------+------------+----------------------+
3 rows in set (0.00 sec)

```

以下に示すようなデータを抽出できるような**SELECT文**を考えてみてください．

-   「城北」が住所に含まれる地域の郵便番号を表示するには？(ヒント：[LIKE演算子](http://dev.mysql.com/doc/refman/4.1/ja/string-comparison-functions.html))
-   郵便番号を辞書順で降順にソートして表示するには？(ヒント：[ORDER BY句](https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0147))
-   各市区町村別がそれぞれ何個の郵便番号を持っているか？(ヒント：[GROUP BY句](http://dev.mysql.com/doc/refman/5.1/ja/group-by-modifiers.html))
-   出力を個数の多い順にソートして表示するには？(ヒント：[ORDER BY句](https://qiita.com/ozlee/items/b3983c96f23b27d9044d))

### 高度な問い合わせ例

それでは，具体的に高度な問い合わせをどのように作成していったらよいかコツを説明します．

例えば，「**中区城北**の郵便番号を調べる」にはどうしたらよいでしょうか．**中区**は**addr2**に格納されているし，**城北**は**addr3**に格納されていますので，**中区城北**のような住所の断片はどうやって問い合わせましょうか．

様々な方法が考えられますが，「**addr1と2と3を連結した文字列に対して，like演算で抽出する**」というような方法を思い浮かべたかもしれません．

では，文字列を連結するにはどうしたらよさそうでしょうか．MariaDBのリファレンスマニュアルを探したりすれば，使えそうな文字列関数が見つかるかもしれません．

[MariaDBリファレンスマニュアル 文字列関数](https://mariadb.com/kb/en/mariadb/string-functions/)

`CONCAT(str1,str2,...)`という関数がありますね．引数を全て連結した文字列を返す関数ですので，これが使えるかもしれません．連結したいのは，**addr1**，**addr2**，**addr3**ですから以下の様にできそうです．

`CONCAT(addr1,addr2,addr3)`

この関数の返す文字列の一部に，**中区城北**が含まれていればよいわけですから，SQLのWHERE句は次のように書けそうです．

`CONCAT(addr1,addr2,addr3) like '%中区城北%'`

つまり，テーブル名**zipShizuoka**から**中区城北**が含まれている文字列の郵便番号**zip**を返すSQL文は，以下のように書けそうです．

```sql
> SELECT zip FROM zipShizuoka WHERE CONCAT(addr1,addr2,addr3) like '%中区城北%';
```

さぁどのような結果が得られたでしょう？ちなみに中区城北の郵便番号は**4328011**です．表示されてますか？（表示されない人は何かが足りない？）

ここまでできたら，いよいよ次はPHPプログラムからデータベースへアクセスしてみましょう．

## PHPからデータベースへアクセスする方法

### 接続

次に，PHPからデータベースへアクセスする方法について説明します．そろそろこんなこともできるかなと思いついたものを調べて実装する力をつけていってほしいと思います．

まず実現方法を調べるために「[PHP MySQL](https://www.google.co.jp/search?q=PHP+MySQL)」で検索してみてください．

「[PHPのマニュアルページにあるMySQLの関数のリファレンス](http://php.net/manual/ja/book.mysqli.php)」等が見つかりましたか？PHPでMySQLへアクセスしたいわけですので，PHPのMySQL関連の関数について調べてみるのが第一です．

PHPのMySQL関連の関数といっても山のようにあります．MySQLに**接続**したいわけですから，きっと**connect**というキーワードが絡んでいそうです．

ページをよく眺めると，[mysqli\_connect](http://jp2.php.net/manual/ja/function.mysqli-connect.php)という関数がありますね！

早速クリックしてみるとこの関数の使い方が説明されています．これを見ると**mysqli\_connect**は**mysqli::\_\_construct**という関数の別名であり，５つの引数をとる事がわかります．

-   **host**：サーバ上のブログラムからサーバ上のプログラムへ接続するので，これはこのままで大丈夫だろうな
-   **username**：きっとユーザ名のことだろう．先に作成した**CSexp1**というMySQLのユーザ名を入れれば良いだろう
-   **passwd**：きっとMySQLのユーザ名に対するパスワードだろうな．．．
-   **dbname**：きっとMySQLのデータベース名だろうな．．．
-   **port**：きっと接続先ホストのポート番号だろうな．．．

というように必要な引数の使い方が何となく想像できると思います．このような雰囲気で，PHPのソースコードを作成し実際に試してみましょう．

「**接続に成功しました**」と表示されたら成功です．

### 問い合わせ

接続しただけではつまらないですよね．先ほどコマンドラインで実現した郵便番号テーブルからの検索を実現してみましょう．

データベースへの検索は，問い合わせや**Query**と呼びます．先ほどのMySQL関連の関数の中に**mysqli\_query**という関数があることに気づきましたか．

このように実現したいこととそのキーワードが分かれば，基本的にはホームページを検索することで大抵の情報は見つかります．

検索力を身に着けて実現したいことを実装していきましょう．

### 補足1: 手続き型とオブジェクト指向型

[MySQL Improved Extension](http://php.net/manual/ja/book.mysqli.php)のライブラリ群は，手続き型の利用方法とオブジェクト指向による利用方法の2通りの利用方法が用意されています．オブジェクト指向の利用方法の方が，モダンなコードで通常きれいなコードになりますのでこの機会にチャレンジしてみてくだい．以下のページの"Example #2 Object-oriented and procedural interface"のサンプルコードが参考になります．

-   [Dual procedural and object-oriented interface](http://php.net/manual/ja/mysqli.quickstart.dual-interface.php)

【注意】 手続き型のインタフェースとオブジェクト指向型のインタフェースは別物です．混在して使うとトラブルの元ですので，ライブラリ中の各関数については，手続き型で書くなら手続き型のインタフェースを使用し，オブジェクト指向型で書くならオブジェクト指向型のインタフェースを利用してください．例えば，手続き型ではmysqli\_query( \$link, \$query ) を使います．mysqli::query( \$query )は使えません．当然，mysqli\_query( \$query )は使えません（文法エラーにはなりませんので注意）．

### 補足2: Queryの設定方法（statements vs. prepared-statements）

queryの設定方法も2種類の方法が用意されています．

-   [完成したSQL文字列を用意し，クエリーを実行する方法](http://php.net/manual/ja/mysqli.quickstart.statements.php)
-   [パラメータを未定にしたSQL文字列を解析し，その後でパラメータを設定した上でクエリーを実行する方法](http://php.net/manual/ja/mysqli.quickstart.prepared-statements.php)

SQLインジェクション対策という観点から後者の方法が推奨されますので，前者で動作確認した後は，後者の方法を使ってみてください．

## 本日の課題について

-   以下の仕様を満たすPHPプログラムを実装してください．
	-   「郵便番号」or「住所」or「住所のカナ」の入力を受け付ける
	-   入力は一部でも良い
	-   入力が何であるかを自動判別する
	-   条件に合致するエントリーをテーブル形式で表示する
	-   表示が多すぎる時は複数ページへ分割する

![addr-postcode-search.png](../../../images/part3/part3_2/addr_postcode_search.png)

テキストフィールドに入力された文字列をPHPプログラムで取得する方法は，以下のページを参考にしてください．

[http://php.net/manual/ja/tutorial.forms.php](http://php.net/manual/ja/tutorial.forms.php)

### 確認項目

以下の達成レベルを必ず低いレベルのチェック項目（各レベル最低一つ以上）からクリアしていってください．

#### 「可(C)」レベル

-   上記要件のアプリケーションを実装する

#### 「良(B)」レベル

-   郵便番号を全国対応にする
-   以下のようなページ装飾系ツールを利用してページを綺麗にデザインする
    -   [Bootstrap](http://getbootstrap.com/)
    -   [Webix](http://webix.com/)
    -   などなど

#### 「優(A)」レベル

-   住所入力中に候補を表示する（Googleサジェストのような機能）
-   Google Maps等の地図系ツールと連携させる
    -   [Google Maps API](https://developers.google.com/maps/documentation/javascript/?hl=ja)
    -   [ジオコーディング](https://developers.google.com/maps/documentation/javascript/examples/geocoding-simple?hl=ja)
    -   [OpenLayers](http://openlayers.org/)
    -   などなど

#### 「秀(S)」レベル

-   「優」レベルをnode.jsで実装する
-   その他，秀に相当すると感じさせる工夫や実装

### 進め方
#### サンプルコード

以下に、可レベルに達していないサンプルコードを用意しました。

これを元に実装を進めていっても構いません。

また、詰まったら下のヒントも参考にしてみてください。

```php
<html>
  <head>
    <meta charset="UTF-8">
    <title>科学科実験Aサンプルプログラム</title>
  </head>

  <body>
    <h1>住所検索</h1>

    <form action="sample.php" method="GET">
      <input type="text" name="keyword">
      <input type="hidden" name="page" value="1">
      <input type="submit" value="検索">
    </form>
<?php
# 初期設定
$mysqli = new mysqli('localhost', 'username', 'password', 'dbname');

# mysqlとの接続
if ($mysqli->connect_error) {
  echo $mysqli->connect_error;
  exit();
} else {
  $mysqli->set_charset("utf8");
}

# クエリの受け取り
$keyword = $_GET['keyword'];
# keywordクエリの中身が何もなかった場合終了
if (!isset($keyword) || empty($keyword)) {
  exit();
}

echo "$keyword". "の検索結果";
echo "<br>";

# クエリの受け取り
$page = $_GET['page'];

# pageクエリの中身が何もなかった場合、もしくはpage番号が負の数であれば1を入れる
if (!isset($page) || $page < 0) {
  $page = 1;
}


# 入力データの形式を判定
$addr = "";
$zip = "";
$kana = "";
if (preg_match("/^[0-9]+$/", $keyword)) {
  $zip = $keyword;
# 興味がある方は正規表現を用いてカタカナを場合分けする方法を調べてみてください
# } else if (preg_match("", $keyword)){
#   $kana = $keyword;
} else {
  $addr = $keyword;
}

# CONCATを用いて入力されたものを結合して検索
$query = "SELECT addr2, addr3, zip FROM zipShizuoka WHERE CONCAT(addr1, addr2, addr3) like ? AND CONCAT(kana1, kana2, kana3) like ? AND zip like ? LIMIT ?, 10";

$addr = '%'.$addr.'%';
$zip = '%'.$zip.'%';
$kana = '%'.$kana.'%';

# offset値を訂正してください
$offset = $page;

# オフセット含めて10件のみ検索
if ($stmt = $mysqli->prepare($query)) {
  $stmt->bind_param("sssi", $addr, $kana, $zip, $offset);
  $stmt->execute();
  $stmt->bind_result($addr2, $addr3, $zipcode);
  while ($stmt->fetch()) {
    echo "$addr2 $addr3 $zipcode";
    echo "<br>";
  }
  $stmt->close();
} else {
  echo "db error";
}

# mysqlとの接続をやめる
$mysqli->close();

?>
    </div>
  </body>

</html>
```

#### ヒント

##### 任意のカラム（郵便番号 or 住所 or カナ）による入力を受け付ける

いくつか方法があるので紹介します

**(方法1)** 該当のカラムをCONCATで繋げたものをLIKEで検索（上の解説を参照）

**(方法2)** 1カラムずつ検索して，検索結果があったものを表示

-   検索結果があったかどうかは　mysqli\_num\_rows() の行数で確認できます

**(方法3)** 入力データの形式を正規表現(preg\_match()等)で判定して特定のカラムを検索

-   以下のサイト等で正規表現が正しいかどうかのチェックができます．カタカナの正規表現は少し複雑ですが検索等して調べてみましょう！
    -   [PHP: preg\_match() / JavaScript: match() 正規表現チェッカー ver3.0](http://okumocchi.jp/php/re.php)

それぞれのやり方にメリット・デメリットがあります．どのような例外が存在するかについても考えておきましょう．

##### 複数ページの実現

**次のページに行ったら全件表示されてしまう・・・**

検索ワードを次ページに受け渡していないのが原因です．$\_REQUEST 変数等を使って検索ワードを次ページに引き渡してあげましょう．

例）

```php
<a href="./index.php?page=<?php echo $page ?>&search=<?php echo $_REQUEST["search"] ?>">次のページへ</a>
```


**特定の範囲のデータをとってくる**

「LIMIT オフセット値, 件数」，もしくは「LIMIT 件数 OFFSET オフセット値」といったSQL文を記載することで指定の範囲のデータを取得することができます．オフセットをいくつにすればよいかはページ番号さえわかれば計算できますよね？

```
> SELECT zip from zipShizuoka LIMIT 1, 5;
+---------+
| zip     |
+---------+
| 4200838 |
| 4212309 |
| 4211307 | <-- 3件目
| 4200017 |
| 4211305 |
+---------+
5 rows in set (0.00 sec)

> SELECT zip from zipShizuoka LIMIT 3, 5;
+---------+
| zip     |
+---------+
| 4211307 | <-- 3件目が先頭にきている
| 4200017 |
| 4211305 |
| 4200963 |
| 4200948 |
+---------+
5 rows in set (0.00 sec)
```

**検索結果の全レコード数を数える**

ページ番号 3/20 といったような全体のページ数を計算したい場合，検索結果の全レコード数を毎回取得する必要があります． 上記のOFFSETを使ったSQLだと常にLIMITで指定した件数が返されるため，全件数が取得できません．方法として， 全件数を計算するSQLを発行するか，全件を取得してPHP側で数え上げるかのどちらかを行う必要があります．

**(方法1) COUNT() 関数を使う**

COUNT() 関数を使うことでSELECT文により選択されたレコードの件数を得ることができます．

```
> SELECT COUNT(*) as count FROM zipShizuoka WHERE kana1 LIKE '%ハママツ%';
+-------+
| count |
+-------+
|   477 |
+-------+
1 row in set (0.02 sec)
```

**(方法2) mysqli\_num\_rows() を使う**

PHPで mysqli\_query() による検索処理を行なった後に mysqli\_num\_rows() を使って検索結果の全行数を得ることができます．

実装するアプリケーションごとに，サーバとクライアント間の通信量やそれぞれが行う計算量は異なります．どのような場合にどちらのやり方が有効なのかについても考えてみましょう．

## \[必須課題\]性能評価

本日実装した住所・郵便番号検索のWebシステムについて，実装内容を報告するとともに，応答時間の計測結果等からボトルネックを分析し，どのようにしたら性能向上させられるか検討し，改善させてみてください．

1) アプリケーションの実装について工夫した点があれば説明してください（SABCのどこまで実装を行いましたか？）
2) 応答時間を計測し，各クエリの処理時間などまで詳細な分析結果を示したり，各種性能分析コマンドを駆使してボトルネックの原因を考察してください．
3) どのようにしたら性能向上させられるか検討してください．
4) 【発展】ボトルネック部分を改善させてみてください．どこまで改善させられるのか試行錯誤してみてください．

### 参考情報

-   [Chrome Developer Toolsでパフォーマンス計測・改善](https://qiita.com/y_fujieda/items/a0a69151cf7307039f74)
-   [ブレークポイントを使ったJavaScriptデバッグを整理してみた【再入門】](https://qiita.com/nekoneko-wanwan/items/02aed17773833c3ad3b2)
-   [JavaScript,jQueryの爆速コーディング、デバッグ方法論の勧め～実践向け逆引き(windows,chrome向け)～](https://qiita.com/shgkt/items/0054670269e1d3aa49b8)
-   [Linux Performance](http://www.brendangregg.com/linuxperf.html)

## おわりに

上記の課題を実装できた人は，Day10の内容を参考に性能評価を行いましょう． 課題の実装について工夫した部分（SABC）についてはレポートで報告してください．
