# ベンチマークの方法

## 概要
みなさんが作成したWEBシステムの性能について，ベンチマークサーバを用いて評価します．
WEBシステムからベンチマークサーバに計測リクエストを送信すると，ベンチマークサーバは，みなさんが作成したWebシステムに対して，大量の同時アクセスを行い，1秒あたりのリクエスト処理数といった性能を計測します．

```{note}
本ベンチマークは，学内ネットワークに設置したベンチマークサーバから，同じく学内ネットワークに設置された皆さんのRaspberry Piに向かって負荷を与えることで性能測定を行います．このため，**皆さんの Raspberry Pi は学内ネットワークに接続されている必要があります**．有線あるいは無線を用いて，学内ネットワークに接続した後，Raspberry PiがDHCPで取得したIPアドレスを確認しておいてください．
```

## 予選・本選ベンチマークサーバ
### ログイン
**ベンチマークサーバ**にアクセスします．（学内からでないとアクセスできません．アクセス先は本ページでは非公開です．Mattermost等でアナウンスしますのでそちらを確認してください）予選期間は予選ベンチマークサーバにのみ，本選期間は本選ベンチマークサーバにのみアクセス可能です．
```{image} ../../../images/part3/part3_2/login.png
:alt: ベンチマークサーバのトップ画面
:width: 800px
:align: center
```

グループ名，事前に配布されたパスワードを入力してベンチマークサーバにログインしますs．

### 計測
みなさんが作成したwebシステムのURLを入力し，「ベンチマーク開始」をクリックすることで計測が開始されます．
```{image} ../../../images/part3/part3_2/bench_start.png
:alt: ベンチマーク計測画面
:width: 800px
:align: center
```


WebシステムのURLは以下のようになると思います．

**フォーマット：**
```
http://<Raspberry PiのIPアドレス>/~<username>/<プログラム名>
```
**入力例：**
```
http://10.70.174.167/~pi/progA.php
```
なお，IPアドレスはRaspberry Piを学内ネットワークに接続した後に取得したIPアドレスに置き換えてください．
IPアドレスは学内ネットワークに接続後，ターミナルで以下のコマンドを実行することで確認できます．
```
$ ip a
```

```{image} ../../../images/part3/part3_2/raspi_ip.png
:alt: ip aの例
:width: 800px
:align: center
```

上記の場合，`10.70.174.167`がRaspberry PiのIPアドレスとなります．

ベンチマーク開始ボタンをクリックするとキューに追加され，計測が開始します．
待ちがない場合は，そのままwebシステムの性能の計測を行いますが，先に性能を計測しているグループが存在する場合は，それらのグループの計測が終了するまで待機します．
```{image} ../../../images/part3/part3_2/bench_inProgress.png
:alt: ベンチマークサーバの計測中画面
:width: 800px
:align: center
```


webシステムの計測が終了すると，計測結果が表示されます．
```{image} ../../../images/part3/part3_2/bench_result.png
:alt: ベンチマークサーバの計測結果画面
:width: 800px
:align: center
```


また，結果一覧ページでも過去の計測結果を確認することが可能です．
```{image} ../../../images/part3/part3_2/result_list.png
:alt: ベンチマークサーバの計測結果画面
:width: 800px
:align: center
```

## ランキング
ランキングページで各グループのスコアランキングを見ることができます．
```{image} ../../../images/part3/part3_2/ranking.png
:alt: スコアランキング画面
:width: 800px
:align: center
```


他のグループに負けないような高性能なwebシステムの設計を目指しましょう．

## ベンチマークサーバの仕様
### 計測結果のステータス
計測結果画面には，「Success」の他に，「Connection Failed」，「Validation Error」，「Timeout」ステータスについても表示されることがあります．

```{image} ../../../images/part3/part3_2/result_connection_failed.png
:alt: 接続失敗画面
:width: 800px
:align: center
```

```{image} ../../../images/part3/part3_2/result_validation_error.png
:alt: 検証失敗画面
:width: 800px
:align: center
```

```{image} ../../../images/part3/part3_2/result_timeout.png
:alt: タイムアウト画面
:width: 800px
:align: center
```

「Connection Failed」ステータスは，ベンチマークサーバとの接続に失敗した場合に表示されます．IPアドレスやサーバーの起動を確認してください．
「Validation Error」ステータスは，レスポンスの形式が異なる場合に表示されます．[競技ルール](./regulation.md "レスポンス形式")を今一度確認してください．
「Timeout」ステータスは，決められた時間内にレスポンスが返却されなかった場合に表示されます．