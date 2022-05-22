# ベンチマークサーバの説明

## 概要
みなさんが作成したWEBシステムの性能について，ベンチマークサーバを用いて評価します．
WEBシステムからベンチマークサーバに計測リクエストを送信すると，ベンチマークサーバは，みなさんが作成したWebシステムに対して，大量の同時アクセスを行い，1秒あたりのリクエスト処理数といった性能を計測します．
ベンチマークサーバとRaspbeery Piの構成は以下のようになっています．
```{image} ../../../images/part3/part3_2/configuration.png
:alt: ベンチマークサーバの構成図
:width: 400px
:align: center
```


## 予選・本選ベンチマークサーバ
### ベンチマークサーバの使用方法
[ベンチマークサーバ](https://google.com)にアクセスします．（学内からでないとアクセスできません）

予選期間は予選ベンチマークサーバにのみ，本選期間は本選ベンチマークサーバにのみアクセス可能です．

```{image} ../../../images/part3/part3_2/bench-top.png
:alt: ベンチマークサーバのトップ画面
:width: 800px
:align: center
```


グループ名，事前に配布されたパスワード，みなさんが作成したwebシステムのURLを入力します．
WebシステムのURLは以下のようになると思います．

**フォーマット：**
```
http://<Raspberry PiのIPアドレス>/~<username>/<プログラム名>
```
**入力例：**
```
http://192.168.100.49/~pi/progA.php
```
また，Raspberry PiのIPアドレスはSUCSexpA Wi-Fiに接続後，ターミナルで以下のコマンドを実行することで確認できます．
```
$ ip a
```

```{image} ../../../images/part3/part3_2/raspi_ip.png
:alt: ip aの例
:width: 800px
:align: center
```

上記の場合，`192.168.0.124`がRaspberry PiのIPアドレスとなります．

フォームに情報を入力した後，計測開始ボタンをクリックします．

```{image} ../../../images/part3/part3_2/bench-top-example.png
:alt: ベンチマークサーバのトップ画面 入力例
:width: 800px
:align: center
```


計測開始ボタンをクリックするとキューに追加されます．
待ちがない場合は，そのままwebシステムの性能の計測を行いますが，先に性能を計測しているグループが存在する場合は，それらのグループの計測が終了するまで待機します．（1グループの計測に最大3分ほどかかります）
```{image} ../../../images/part3/part3_2/bench-queue.png
:alt: ベンチマークサーバのqueue待ち画面
:width: 800px
:align: center
```


webシステムの計測が正常に終了すると，計測結果が表示されます．
```{image} ../../../images/part3/part3_2/bench-result.png
:alt: ベンチマークサーバの計測結果画面
:width: 800px
:align: center
```


## ランキング
[http://expa-ranking.s3-website-ap-northeast-1.amazonaws.com](http://expa-ranking.s3-website-ap-northeast-1.amazonaws.com)やベンチマークサーバのトップページから，全グループの計測結果を見ることができます．

また，[http://expa-ranking.s3-website-ap-northeast-1.amazonaws.com?group_id=a1](http://expa-ranking.s3-website-ap-northeast-1.amazonaws.com?group_id=a1)のようにすることで，グループごとの結果も確認することができます．

他のグループに負けないような高性能なwebシステムの設計を目指しましょう．

## 在宅用ベンチマークサーバ
### 在宅用ベンチマークサーバの概要
在宅用ベンチマークサーバは，在宅で授業を受けており，予選・本選ベンチマークサーバにアクセスすることができない人向けのベンチマークサーバです．
在宅用ベンチマークサーバは，webシステムの性能評価方法の仕様は予選サーバと同等のものですが，計測結果をランキングに反映する機能などはありません．

### 在宅用ベンチマークサーバの使用方法
ベンチマークサーバをダウンロードします．
- `\\fs.inf.in.shizuoka.ac.jp\\share\\class\\情報科学実験I\\第三部サンプルデータ\\benchmarkserver.zip`（静大のネットワーク内からはWindowsファイル共有にて．外部からはVPNを利用してアクセス可能）
ダウンロードしたzipファイルを解凍し，解凍したフォルダ内のmain.exeをダブルクリックし，ベンチマークサーバを起動します．
以下のような警告がでた場合は，「アクセスを許可する」を選択してください．
```{image} ../../../images/part3/part3_2/firewall.png
:alt: ファイヤーウォール許可
:width: 600px
:align: center
```


すると，コマンドプロンプトが立ち上がり，以下のメッセージが表示されます．
```
2022/xx/xx xx:xx:xx Listening...
```
次に，webブラウザを開いて，以下のURLを入力し，ベンチマークサーバにアクセスします．
```
http://localhost:3000
```
これ以降の操作は予選サーバと同様です．

### 在宅用ベンチマークサーバのオプションを変更して起動する
在宅用ベンチマークサーバは，オプションを変更して起動することが可能です．
#### コマンドオプション
```{list-table} コマンドオプション
:header-rows: 1
:name: bench_options
* - オプション
  - 意味
  - デフォルト値
* - -n 数値
  - テストで発行するリクエストの回数を数値で指定
  - 10
* - -c 数値
  - テストで同時に発行するリクエストの数を数値で指定
  - 5
* - -t 数値
  - 1リクエストのタイムアウト時間を秒単位で指定
  - 2
* - -r 数値
  - 1の場合のみ計測に使用するタグ名をランダムな順番で選択
  - 1
* - -s 数値
  - 計測に使用するタグ数を指定．-1の場合は，ファイルに記載されているすべてタグを使用
  - 100
* - -p 文字列
  - 計測に使用するタグ名が記載されているファイルのPathを指定
  - ./searchtag.txt
```
#### 使い方
上記のパラメータを変更して起動するためには，コマンドプロンプトから在宅用ベンチマークサーバを起動する必要があります．
在宅用ベンチマークサーバが存在するフォルダへ移動した後，以下のように実行します．
```
main -n 5 
```
[コマンドプロンプトの基本的なコマンド一覧](https://docs.microsoft.com/ja-jp/windows-server/administration/windows-commands/windows-commands)