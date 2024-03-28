# RaspberryPiの無線接続

２つの機材を用いて作業を行なう都合上，ファイルのやり取りなどで通信を行う必要が発生します．本項では主にRaspberryPiとPCを無線で接続して通信を行う方法をいくつか紹介します．

## RaspberryPiのIPアドレスの確認方法

RaspberryPiのIPアドレスは以下のコマンドを用いて確認します．本講義ではwlan0のIPを高頻度で扱うため確認方法を覚えておくと良いでしょう．

```shell
$ ip a

1: lo: ...
...
2: eth0: ...
...
3: wlan0: ...
inet 192.168.100.17/24
...
```

上記出力では結果を省略していますが，wlan0においてIPが`192.168.100.17`であることが解ると思います．

また，WindowsのIPアドレスはコマンドプロンプトにおいて以下のコマンドを用いて確認します．

```shell
$ ipconfig
```

## PCからRaspberryPiへのSSH接続

コマンドプロンプトを開いて，RaspberryPiにPingを送って通信状態を確認します．大かっこの中は適宜読み替えてください．

```shell
$ ping {確認したRaspberryPiのIPアドレス}
```

SSHでRaspberryPiにログインします．最初の接続のみ，ホスト鍵を受け入れるか聞かれますので"yes"とします．

```shell
$ ssh pi@{確認したRaspberryPiのIPアドレス}
```

ユーザ名とパスワードが一致すればログインできます．SSH接続中は`exit`を打ち込むことで，SSH接続を切ることができます．

```shell
$ exit
```

## PCからRaspberryPiへのファイル転送

WinSCPを起動します．ログイン画面が開くので新規のホストを設定しログインします．

- 転送プロトコル:SFTP
- ホスト名:`ip a`で確認したIPアドレス
- ユーザ名:pi
- パスワード:設定したパスワード

![winscp1.png](../../../images/part1/part1_1/winscp1.png)

最初にアクセスした時のみセキュリティ警告が出ますのでOKします．エクスプローラのようにドラッグ＆ドラップや右クリックメニューでファイルの転送や操作が可能です．

## VM環境からRaspberryPiへのSSH接続

VM上のLinuxのターミナルを開いて，RaspberryPiにPingを送って通信状態を確認します．手順はWindows上での接続と同様です．

```shell
$ ping {確認したRaspberryPiのIPアドレス}
```

SSHでRaspberryPiにログインします．最初の接続のみ，ホスト鍵を受け入れるか聞かれますので"yes"とします．

```shell
$ ssh pi@{確認したRaspberryPiのIPアドレス}
```

ユーザ名とパスワードが一致すれば，ログインできます．SSH接続中は`exit`を打ち込むことで，SSH接続を切ることができます．

```shell
$ exit
```

## PCからVM環境へのファイル転送

ホストOS（Windows）からゲストOS（Linux）にファイルを転送します．

VirtualBoxのメニューから「仮想マシン」→「設定」を開きます．

![scpvm1.png](../../../images/part1/part1_1/scpvm1.png)

ネットワークの設定から「高度」タブを開き「ポートフォワーディング」を開きます．

![scpvm2.png](../../../images/part1/part1_1/scpvm2.png)

右上のアイコンをクリックして，フォワーディングルールを追加します．ここでは，ホストOS（Windows）のポート22への接続をゲストOS（Linux）のポート22に転送する設定を行います．

![scpvm3.png](../../../images/part1/part1_1/scpvm3.png)

WinSCPを起動し，接続先のホストとして以下の指定して接続します．

- ホスト名:localhost
- ユーザ名:ゲストOSに設定したユーザ名
- パスワード:ゲストOSに設定したパスワード

![scpvm4.png](../../../images/part1/part1_1/scpvm4.png)

## 外部から接続する場合（任意）

```{important}
VirtualBoxの設定上は外部からSSH接続できる設定ですが，実際にはWindowsファイアウォールが接続を遮断して接続できない場合がありますので注意してください．
```

例えば，RaspberryPiからWindowsのイーサネットアダプタに設定したIPアドレスへSSH接続を試みると，ポートフォワーディング設定並びに，ユーザ名・パスワードが正しければログインできるはずですが，ログインできない場合があります，

```shell
$ ssh <VMのユーザー名>@<VMのIP>
```

この場合，Windowsファイアウォールの設定を行えば接続が可能になりますが，セキュリティ上のリスクを伴いますので無理に行う必要はありません．実験では，RaspberryPi側をサーバプログラム，PC上のゲストOS側をクライアントプログラムとして実験を進めてください．
