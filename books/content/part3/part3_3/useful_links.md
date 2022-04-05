# 実装の参考となるリンク集

## PHP関係

[PHPリファレンス](http://php.net/manual/ja/index.php)

### PHP実行時のエラー表示設定

Raspberry PiではPHPプログラムにエラーがあった場合，標準では `/var/log/apache2/error\log`というファイルにエラー内容を書きだします（Fedora の場合は `/var/log/httpd/error\log`でした）．

このファイルはとても行数が多くなっていることが多いので，`tail`コマンドを使ってファイルの最後から中身を見ることが一般的です．

また，エラーログが大きくなりすぎないように自動的に複数ファイルに分割されていることもありますので，最新のエラーログフィルを最後から眺めてみてください．

```sh
# Rasbian OS の場合
$ sudo tail -n 50 /var/log/apache2/error_log
```

```sh
# Fedora OS の場合
$ sudo tail -n 50 /var/log/httpd/error_log
```

ここで，毎回，コマンドラインでこのコマンドを入力してエラーログを確認するのは手間ですし，複数人で同時にエラーが生じた場合，誰のエラーに対するエラーログかよくわかりません．

そこで，Webブラウザの画面にPHP実行時のエラーを表示させる設定もあります．

PHP関係の設定は，`/etc/php.ini`というファイルに記載されてますので，適宜関係しそうな記述を修正してみてください．

Apacheを再起動すると反映されます．

例えば，PHPのエラーを表示させるには，`php.ini`ファイルの一番末尾に以下の一行を加えてください．

```ini
 display_errors = On
```

ただし，この設定は**セキュアではない**ことに注意してください．

実装したPHPプログラムにどのような不具合があるのかを世界中に公開しているようなものですから，あくまで開発時の一時的な設定だと考えて，検証が終わったら設定をOffにしておくことを忘れずに！

補足：別のやり方としてエラーログを監視する方法もあります

```sh
$ sudo less +F /var/log/httpd/error_log
```

等するとファイルの更新を監視するモードになります．その状態でWebブラウザを開き，PHPにアクセスしてみてください．（エラーがあれば）`error_log`にエラーが追記されたことがわかるでしょう． 監視モードから元に戻るにはCtrl+C（さらにQを押して終了），再度監視モードに戻るにはShift+Fで戻ることができます．

### エラーログの消去方法

12万行のデータを扱っていると，デバッグ時に大量のエラーログが出力され，ディスクの使用量を一杯にしてしまうことがあります．

ディスク容量の確認方法や，エラーログのサイズ確認，削除方法は以下を参考にしてください．もちろん他の方法でもできます．

-   SSDやマイクロSDカード等の使用量の確認方法

```sh
$ df
```

-   `/dev/mapper/fedora-root`の使用率が100%に近い場合は，以下のコマンドでログファイルの大きさをチェック

```sh
$ sudo ls -l /var/log/httpd/
```

-   `error_log`のサイズが大きいようなら次のコマンドでエラーログの中身を削除（もちろん中身を確認し問題ないなら）

例）HTTPログファイルの場合（`echo`で何も表示させない結果を`error_log`へ書き込むことで中身を消去）

```sh
$ sudo sh - c "echo '' > /var/log/httpd/error_log"
```

### Flickr APIについて

-   [Flickr APIのリファレンス](http://www.flickr.com/services/api/)

-   [新着画像を得る(flickr.photos.getRecent)](http://www.flickr.com/services/api/flickr.photos.getRecent.html)

-   [画像検索をする(flickr.photos.search)](http://www.flickr.com/services/api/flickr.photos.search.html)

#### flickr.photos.searchの使用例

以下の二つのページを見比べながら実装を進めると便利です．

-   [オプション（関数の引数みたいなもの）のマニュアル](http://www.flickr.com/services/api/flickr.photos.search.html)

-   [API explore：簡易的にAPIの結果を確認するツール](http://www.flickr.com/services/api/explore/flickr.photos.search)

1\. まずAPI exploreを使ってURLを生成してみてください．

-   以下にPHPで使用しやすい設定例を示します
    -   Output: **PHP Serial**
    -   **Do not sign call?** をチェック
    -   **extras**：得たい画像のメタデータ項目をカンマ区切りで入力
    -   **per\_page**：結果ページ１枚内に含まれる画像枚数（最大500枚）
    -   **page**：結果における何ページ目かの数

2\. **Call Method**をクリックすると，結果とURLがページ下部に表示されます．

3\. 表示されたURLを，以下の例のようにPHPを用いて簡易表示させて動作確認

```php
<?php
print_r(unserialize(file_get_contents("http://api.flickr.com/services/rest/?method=flickr.photos.search&api_key=70fb3845ef7d1964fe89aeb7fc80258a&bbox=+-180%2C+-90%2C+180%2C+90&per_page=500&page=1&format=php_serial")));
?>
```

-   この例で使用している関数のマニュアル
    -   [http://php.net/manual/ja/function.print-r.php](http://php.net/manual/ja/function.print-r.php)
    -   [http://php.net/manual/ja/function.unserialize.php](http://php.net/manual/ja/function.unserialize.php)
    -   [http://php.net/manual/ja/function.file-get-contents.php](http://php.net/manual/ja/function.file-get-contents.php)

## HTML，JavaScript関係

-   [JavaScriptリファレンス](http://www.w3schools.com/jsref/)
-   [HTML5リファレンス](http://www.htmq.com/html5/)
-   [CSS3リファレンス](http://memopad.bitter.jp/w3c/css3/css3_reference.html)

## MySQL関係

-   [SQLリファレンス](https://dev.mysql.com/doc/refman/5.6/ja/sql-syntax-data-manipulation.html)
-   [SQLの基礎 「SELECT」文を覚えよう](http://www.atmarkit.co.jp/ait/articles/0006/21/news001.html)

### rootのパスワードを強制再設定

MySQLのrootのパスワードが分からない場合は以下を実行してみてください．

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

もしくは

```sh
$ sudo systemctrl stop mysqld.service            # <--- MySQLサーバを停止
$ mysqld_safe -skip-grant-tables &               # <--- MySQLサーバをセーフモードでかつ権限無視モードで起動
$ mysqladmin -u root password 'mynewpassword'    # <--- rootのパスワードを「mynewpassword（例）」に強制変更
$ sudo systemctrl restart mysqld.service         # <--- MySQLサーバを再起動
$ mysql -u root -p                               # <--- ログイン試行
```

## Apache，Linux（Fedora，Raspbian）関係

検索してみよう！

## 性能分析について

-   [Linux Performance](http://www.brendangregg.com/linuxperf.html)
-   [Systems Performance 読んでいく](http://tombo2.hatenablog.com/archive/category/SystemsPerformance)
-   [シンプルでシステマチックな Linux 性能分析方法](https://www.slideshare.net/yoheiazekatsu/linux-72826405)
-   [SQL大量発行処理をいかにして高速化するか](https://www.slideshare.net/ShogoWakayama/sql-79530929?next_slideshow=1)
-   [Linuxカーネルドキュメントプロジェクト](https://ja.osdn.net/projects/linux-kernel-docs/wiki/FrontPage)
-   [15 Tips to Optimize Your PHP Script for Better Performance for Developers](http://www.thegeekstuff.com/2014/04/optimize-php-code)

## Node.js関係

-   [node.jsリファレンス](https://nodejs.org/docs/latest-v10.x/api/)
