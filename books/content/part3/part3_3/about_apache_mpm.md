# Apache MPMについて

## 概要

Apache MPM (Multi Processing Module)とは，多数のWebブラウザからの接続要求をどのように処理するかをモジュール化したもので，設定ファイルを編集することで処理方法を変えることができます．

例えば，これまでの実験課題で，プログラムをマルチプロセスで実装するか，マルチスレッドで実装するか検討し実装したと思いますが，Apacheはこの部分がモジュール化されており，管理者の要望に応じてマルチプロセスでもマルチスレッドでもどちらでも運用できるようになっています．

MPMについて参考なりそうなリンクをいくつか掲載しておきます．他にもありますので適宜検索してみてください．

-   MPMとは何か？
    -   [マルチプロセッシングモジュール](http://httpd.apache.org/docs/2.4/ja/mpm.html)（Apache公式ページの解説）
    -   [Apache MPM の基礎をしっかりと理解しよう！](http://unoh.github.io/2008/03/21/apache_mpm.html)（内容はちょっと古いです）
-   設定のチューニングについて
    -   [Apache2 - worker MPM のプロセス＆スレッド数のチューニング](http://www.drk7.jp/MT/archives/001594.html)（内容はちょっと古いです）

## MPMモジュールの種類（Wikipediaより）

### prefork（Apache2.2までのデフォルト設定）

preforkは，「スレッドを使わず，先行して fork を行なうウェブサーバ」である．Apacheは伝統的に親プロセスを1つ持ち，クライアントからリクエストが来ると自分自身をコピーして子プロセスを起動する（これをforkという）．実際の通信は子プロセスが受け持つ．そのため，通信している数だけ子プロセスが起動することになる．この時，クライアントからリクエストを受けたあとでforkするとfork完了までに待ち時間が出来て通信のパフォーマンスが遅くなる．そのため，あらかじめいくつかの子プロセスをforkしておき，forkの待ち時間をなくす方式をとっている．この方式が「prefork」である．すなわち“pre（＝前もって・先行して）”forkしておく，という意味である．

preforkのメリットは，forkされた子プロセス1つ1つが対応する通信を受け持つため，ある子プロセスが何らかの原因でフリーズしたとしても，他の子プロセスには影響を及ぼすことが無く通信を継続できる．このため安定した通信を行うことができる．一方，クライアントが多くなればなるほど子プロセスの数も増えるため，使用メモリ量やCPU負荷が比例的に増大していく．preforkで多数のクライアントをさばくには，それに応じた大量のメモリと高速なCPUが必要となる．

### worker

workerは，「マルチスレッドとマルチプロセスのハイブリッド型サーバ」である．Apacheの子プロセス1つ1つがマルチスレッドで動作し，スレッド1つが1つのクライアントを受け持つ方式である．すなわち，1つのプロセスがマルチスレッドを利用して複数の通信の面倒を見る．この点で1つのプロセスが1つの通信をみるpreforkとは異なる．また多くの子プロセスを起動せずに済むため，メモリの使用量も減らすことができる．

### event（Apache2.4からのデフォルト設定）

eventは，workerの一種でマルチスレッドで動作する．workerとの違いはKeep-Alive（持続的接続）の処理方法である．workerやpreforkは，Keep-Aliveの持続性を保つために一度利用したスレッド・プロセスをそのまま待機させている．しかしクライアントからの接続が持続的に行われる可能性は保証されているわけではないため，待機していること自体が無駄になる可能性もある．そこで，Keep-Aliveの処理を別のスレッドに割り振って通信を処理する．この方式は長らく実験的サポートであったが，バージョン2.4.1から正式に採用された．

#### ＜Raspbian OSは以下の手順＞

Apacheでは一般的に，設定は `/etc/apache2/apache2.conf` や `/etc/apache2/httpd.conf` に記述しますが，複数のモジュールを使うとこれらの設定ファイルが長くなりすぎて理解困難になってしまいます．そこで最近は，`/etc/apache2/conf-enabled` や `/etc/apache2/modes-enabled` 等のディレクトリ配下に読み込ませたい設定ファイルを用意した上でApacheを起動させればよいようになっています．以下に基本的な流れを説明しておきます．

まず，必要なモジュールをインストールすると，設定ファイルは `/etc/apache2/modes-available` に「モジュール名.load や モジュール名.conf」として用意されます．ここで，インストールできるモジュールリストは以下のコマンドで確認でき，インストールもできます．

```sh
$ apt search libapache2-mod                            <----- モジュールリストの確認
$ sudo apt install libapache2-mod-モジュール名         <----- 必要なモジュールのインストール
```

次に，インストールされた modes-available にあるモジュールの中から，実際にApacheに読み込ませてモジュールを有効にしたい場合は以下のコマンドを実行します．

```sh
$ sudo a2enmod モジュール名      <--- モジュールの有効化
$ sudo a2dismod モジュール名      <--- モジュールの無効化
```

最後に，apache2を再起動して有効化したモジュールを反映させて起動します．

```sh
$ sudo systemctl restart apache2
```


ということで本題ですが，PHP には，モジュール版（Webサーバのプロセスの中でPHPを実行）とCGI版（Webサーバとは別のプロセスで実行）という二種類があります．モジュール版のmod\_phpはスレッドセーフではないため，Apache2をmpm\_eventやmpm\_workerで運用している場合は使用できません．つまり，mod\_phpは，mpm\_prefork専用です．そのため，モジュール版のmod\_phpを無効にして，mpm\_worker等でも使用できるCGI版のmod\_fcgidを有効にし，FastCGIデーモンによるPHP環境を構築してみましょう（速度面では同一プロセス内で実行されるモジュール版，リソース管理の安全面からはCGI版と，サーバの運用用途によって使い分けてください）．

```sh
#mod_phpやmpm_preforkを無効にして，mpm_workerを有効に
$ sudo a2dismod php7.3
$ sudo a2dismod mpm_prefork
$ sudo a2dismod mpm_event
$ sudo a2enmod mpm_worker

$ sudo apt-get update
$ sudo apt install php-fpm libapache2-mod-fcgid
#自動的にphp7.3-fpmといった最新バージョンがインストールされるはず
#FPM (FastCGI Process Manager) は PHP の FastCGI 実装のひとつで，主に高負荷のサイトでリソース管理の安全運用な追加機能を用意しています

$ sudo a2enconf php7.3-fpm     <--- conf なので注意！

#proxy_fcgi_moduleが必要なのでこれも有効にする
$ sudo a2enmod proxy_fcgi     <--- mod なので注意！

$ sudo systemctl restart php7.3-fpm
$ sudo systemctl restart apache2
```


特にエラー等表示されなかった場合は，正常動作しているか確認してみてください．

```sh
#apache2が正常動作しているか確認
$ sudo systemctl status apache2

#php7.3-fpmが正常動作しているか確認
$ sudo systemctl status php7.3-fpm

#apache2の現在有効なモジュールを確認（fcgid_moduleがあればOK）
$ apache2ctl -M
```

以前作成した以下の `index.php` をWebブラウザで表示（`localhost/~pi/index.php` 等）したときに，「Server API: FPM/FastCGI」というような表示が出ればOKです．

```php
<?php
phpinfo();
?>
```


どうにもうまういかない方は，apache2を再インストールして試してみるのが早いかもしれません． （configファイル等だけでなく依存パッケージ含めて完全にapache2をアンインストールしたい場合は，`$ sudo apt-get autoremove --purge apache2` ですが，どこからやり直したいかによって以下で十分な場合が多いと思います）

```sh
$ sudo service apache2 stop
$ sudo apt-get remove --purge apache2       <--- purgeオプション付けてconfigファイル等も削除（依存パッケージは残す）

$ sudo apt install apache2
$ sudo systemctl enable apache2.service
$ sudo systemctl start apache2.service
$ sudo a2enmod userdir
$ sudo systemctl restart apache2
```

これで，`localhost/~pi` 等へアクセスして `index.html`が表示されたら，上記のMPMの内容を再度試してみてください．

#### ＜Fedora OSは以下の手順＞

Apacheの設定ファイルを修正することで簡単に設定変更できます．設定ファイル修正後は，Apacheの再起動を忘れずに！

```sh
$ sudo vim /etc/httpd/conf.modules.d/00-mpm.conf
```

有効にしたいMPMモジュールについて**LoadModule**の行頭にある **#** を外して，無効にしたいMPMモジュールの行頭に **#** を挿入してください．

設定ファイル修正後は，Apacheの再起動を忘れずに！

```apache
# Select the MPM module which should be used by uncommenting exactly
# one of the following LoadModule lines:

# prefork MPM: Implements a non-threaded, pre-forking web server
# See: http://httpd.apache.org/docs/2.4/mod/prefork.html
LoadModule mpm_prefork_module modules/mod_mpm_prefork.so

# worker MPM: Multi-Processing Module implementing a hybrid
# multi-threaded multi-process web server
# See: http://httpd.apache.org/docs/2.4/mod/worker.html
#
#LoadModule mpm_worker_module modules/mod_mpm_worker.so

# event MPM: A variant of the worker MPM with the goal of consuming
# threads only for connections with active processing
# See: http://httpd.apache.org/docs/2.4/mod/event.html
#
#LoadModule mpm_event_module modules/mod_mpm_event.so

```

## 性能計測のコツ

性能計測でまず気になるのは，「**どの程度の同時接続数までエラーなく処理できるか？**」です．そのため，まずは`ab`のパラメータを大まかに変えながら「**処理しきれなくなるポイント**」あるいは「**性能が大きく低下するポイント**」を探りましょう．大まかな感触を得られたら，その周辺の負荷でパラメータを細かく変えながら詳細な性能曲線を探っていきましょう．

```sh
ab -n 1000 -c 100 →リクエスト正常終了（Requests per second 36.74)
ab -n 3000 -c 300 →リクエスト正常終了（Requests per second 37.77)
ab -n 5000 -c 500 →リクエスト正常終了（Requests per second 37.58)
ab -n 6000 -c 600 →リクエスト正常終了（Requests per second 38.13)
ab -n 7000 -c 700 →リクエスト正常終了（Requests per second 37.19)
ab -n 8000 -c 800 →リクエスト失敗（apr_recv : Connection timed out)
ab -n 9000 -c 900 →リクエスト失敗（apr_recv : Connection timed out)
ab -n 10000 -c 1000 →リクエスト失敗（apr_recv : Connection timed out)

```

次に気になるのは，Apacheの設定をどのように変更すると，この「**処理しきれなくなるポイント**」がどのように変化するのかを把握することです．Apacheの設定をどのように変更するかは，どのMPMモジュールを使用しているかにも依存しますが，基本的には[MaxClients](http://httpd.apache.org/docs/2.0/ja/mod/mpm_common.html#maxclients)に設定されている値を変えながら調べてけばよいです．その他にも様々な設定値がありますので，周囲と相談しながら試行錯誤してみてください．

例えば，Apacheの設定における**MaxRequestWorkers（Apache2.4以前はMaxClients）**を**10**といった極端に少ない値に設定して負荷試験を実施してみてください．このMaxClientsの値を変化させることで，「**処理しきれなくなるポイント**」がどのように変化していくのか計測しながらApacheの並列処理の仕組みを考察していってもよいと思います．

### ＜Raspbian OSの場合＞

`/etc/apache2/mods-available` ディレクトリ配下にある `mpm_event.conf` や `mpm_prefork.conf` や `mpm_worker.conf` の設定を変更し，各モジュールを有効化して試してみてください．

Apache MPM（Apache公式ページの解説）

-   [Apache MPM prefork](https://httpd.apache.org/docs/2.4/en/mod/prefork.html)
-   [Apache MPM event](https://httpd.apache.org/docs/2.4/en/mod/event.html)
-   [Apache MPM worker](https://httpd.apache.org/docs/2.4/en/mod/worker.html)

```
＜mpm_prefork_module デフォルト設定＞
   StartServers 5
   MinSpareServers 5
   MaxSpareServers 10
   MaxRequestWorkers 150
   MaxConnectionsPerChild 0
```

```
＜mpm_event_module デフォルト設定＞
   StartServers 2
   MinSpareThreads 25
   MaxSpareThreads 75
   ThreadLimit 64
   ThreadsPerChild 25
   MaxRequestWorkers 150
   MaxConnectionsPerChild 0
```

```
＜mpm_worker_module デフォルト設定＞
   StartServers 2
   MinSpareThreads 25
   MaxSpareThreads 75
   ThreadLimit 64
   ThreadsPerChild 25
   MaxRequestWorkers 150
   MaxConnectionsPerChild 0
```

### ＜Fedora OSの場合＞

[**Apache HTTP Server: MPMパラメータ チートシート**](https://heartbeats.jp/hbblog/2015/02/apache-mpm.html)


＜Preforkデフォルト設定＞

Timeout 300

KeepAlive On

MaxKeepAliveRequests 500

KeepAliveTimeout 10

**StartServers: 5** (最初に起動するプロセスの数; 起動時のみ使用)

MaxClients: 500 (ピーク時の負荷を処理するための合計プロセス数)ピーク時のリクエスト処理用に作成する子プロセス数を決定

**ServerLimit: 500** (プロセス存続期間中のMaxClientsの最大構成値)MaxClientsがデフォルトよりも高い値に設定されている場合，ServerLimit値には残りのパラメータより高い値を設定する必要がある

MinSpareServers, MaxSpareServers: 運用中はこれらの値で親プロセスがリクエスト処理用の子プロセス作成を規制

MaxRequestsPerChild: 0 （各子プロセスが処理するリクエスト数の制限を設定．0はプロセスが失効しないことを示す）


＜Workerデフォルト設定＞

Timeout 300

KeepAlive On

MaxKeepAliveRequests 500

KeepAliveTimeout 10

**StartServers: 3** (最初に起動するプロセスの数．起動時のみ使用)

MaxClients: 500 (ピーク時の負荷を処理するための合計プロセス数．ピーク時のリクエスト処理用に作成する子プロセス数を決定）

**ServerLimit: 25** (プロセス存続期間中のMaxClientsの最大構成値．MaxClientsがデフォルトよりも高い値に設定されている場合，ServerLimit値には残りのパラメータより高い値を設定する必要がある）

MinSpareServers, MaxSpareServers: 運用中はこれらの値で親プロセスがリクエスト処理用の子プロセス作成を規制

**ThreadsPerChild: 25** (単一のhttpdプロセス内のワーカー・スレッド数)

MaxRequestsPerChild: 0 (各子プロセスが処理するリクエスト数の制限を設定．0はプロセスが失効しないことを示す)

## チューニングスクリプト

Apache のMaxClientsの設定値を自動計算するスクリプトもあるようです．

-   [Apache チューニングスクリプト（MaxClientsを自動計算する）](https://qiita.com/bezeklik/items/85f4001a2517be5fe4c0)
