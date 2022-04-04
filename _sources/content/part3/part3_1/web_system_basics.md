# Webシステムの基礎

## はじめに

簡単に言えばWebブラウザで閲覧できるシステム，つまり「Webシステム」の構築方法はいくつかあります．
「Webシステム」の根幹をなす「Webサーバ」と「Webクライアント」において，Webサーバの機能を提供する方法がいくつかあるということです．

第二部では，Simple HTTPサーバを実装し，状況に応じて表示が変化するような「動的なWebページ」をWebブラウザへ返すような機能拡張をしたと思います．
例えば，WebサーバがPHP等で作成されたプログラムを呼び出すCGI (Common Gateway Interface)や，Webサーバ自身がHTML内に直接記述された命令を実行するSSI (Server Side Includes)などを用いて動的なWebページを返すことができました．
これをさらに機能拡張していくとApacheのような代表的なWebサーバ用ソフトウェアに近づいていきます．

一方で，従来のWebサーバの実現方法では，1万接続もあるとパフォーマンスが低下していくという課題（C10K問題）があります．
もちろん様々な工夫で高性能化させることができますが（第三部の最終課題），Non Blocking I/O型のWebサーバを実現することで，I/Oの待ちを削減し高性能化させるという方法もあります．

その代表例として，Node.jsというサーバサイドJavaScript実行環境（V8 JavaScriptエンジンで動作）があります．
Node.jsはバイナリにコンパイルされた多くの「コア・モジュール」から成り，CommonJSモジュール仕様に従って実装されたモジュールをnpm (Node Package Manager)コマンドで追加することで，Webアプリケーションで使われる様々な機能を追加できます．
Node.jsは，Non Blocking I/O型のJavaScript実行環境ですので，小さな計算の速度が速くリアルタイム性に富んでいることからWebサーバだけでなくクローラーといった，スケーラブルなネットワークプログラムを簡単に実現できる環境として利用されています（第三部の最終課題をNode.jsで実施しても構いません）．

それでは，Webシステムを構築する上で重要となるWebサーバについて，代表的なWebサーバ用ソフトウェアであるApacheだけでなく，Node.jsを用いたWebサーバを試してみましょう．

## Apacheの動作確認

皆さんのRaspberry PiにApacheが正常にインストールされており，Raspberry PiとノートPCが正常に接続されていれば，ノートPCのWebブラウザで以下のURLを入力すれば何か表示されるはずです．表示されない人は班員とも相談し，サーバ端末やApache設定に不具合があるのか，皆さんのノートPC側に不具合があるのか相談し解決させておいてください．

[http://192.168.1.101/](http://192.168.1.101/)

## node.jsのインストールと動作確認

node.jsとモジュール管理ツールnpmのインストールは，サーバ端末で以下を実行すれば終わります．それぞれ正常にインストールできたか確認するためにバージョン表示されるかも確認してみてください．

```
 $ sudo apt install nodejs npm
 $ node -v
 $ npm -v
```

Raspberry Pi OS(旧：Raspbian) の場合これだとうまくいかないことが多いので以下を試してみてください

```
$ curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
$ sudo apt-get install -y nodejs
```

参考URL: [https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions](https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions)

次に，node.js上で動作するWebサーバプログラム（`test.js`）を以下のように作成してみましょう．「8080」という記載はWebサーバの待ち受けポート番号を表しています．

```
var http = require('http');
http.createServer(function (req, res) {
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('Hello World\n');
}).listen(8080, '192.168.1.101');
console.log('Server running at http://192.168.1.101:8080/');
```

```
 $ cd /var/www/html
 $ sudo vi /var/www/html/test.js
 $ node test.js
```

-   [http://192.168.1.101:8080/](http://192.168.1.101:8080/)

もし手元のノートPCから上記Webサーバへ接続できない場合は，サーバ端末上のファイアウォールの設定を確認してみてください．例えば，ポート番号10080に対してTCP通信を許可するには，以下のように入力すると設定を変更できます．

```
$ sudo ufw disable
$ sudo ufw allow 10080/tcp
$ sudo ufw enable
```

## node.js上で動作するWebサーバの性能比較（発展課題）

興味のある人や余力のある人だけで構いません（加点します）．

第二部で実装したHTTPベンチマークプログラム，もしくは後述するAB (Apache Bench)を用いて，第二部で実装したHTTPサーバと今回のnode.js上で動作するWebサーバの性能比較をしてみてください．その際，Webサーバの返すコンテンツは同一にするといったフェアな比較になるよう条件に注意してください．また，もし意図したような結果にならない場合は，HTTPベンチマークプログラムの並列接続数を増価させてみるとか試行錯誤してみてください．いずれにせよ，どうしてそうなったのか仮説や考察内容が重要です．

## おわりに

順調に進んでますか？
