# RaspberryPi の接続（固定 IP）

## 概要

実験室以外で実験を行う場合に，ディスプレイとマウス・キーボードがない状態で RaspberryPi へ接続する方法を説明します．

情報科学実験 I で配布した RaspberryPi は Ethernet デバイスに固定 IP（192.168.1.101）を設定してありますので，それを前提として PC から接続します．

## RaspberryPi と PC の有線ケーブルでの P2P 接続

### RaspberryPi を電源と接続

RaspberryPi の microUSB 端子に電源を接続します．

![raspi-usb.jpg](../../../images/part1/part1_1/400px-raspi-usb.jpg)

### RaspberryPi と PC との接続

RaspberryPi と PC を LAN ケーブルで直結します（以下の例では USB 接続の LAN アダプタを使っていますが，PC に備え付けの有線 LAN ポートがあればそれに接続します）．

![raspi-pc.jpg](../../../images/part1/part1_1/raspi-pc.jpg)

## IPアドレスと接続の確認

### PC の有線 LAN デバイスに IP アドレスを設定

WindowsPC 上のイーサネットデバイスに RaspberryPi と同じサブネットワークの IP アドレス（192.168.1.102）を設定します．

ただし，Windows 上で複数のイーサネットデバイスを認識している場合がありますので（特に VirtualBox などの VM 環境を構築している場合は仮想イーサネットデバイスが存在するはず），RaspberryPi と接続したイーサネットデバイスを確認してから作業を進めてください．

設定の「ネットワークとインターネット」の「状態」から「アダプターのオプションを設定する」を選択します．

![raspi-con1.png](../../../images/part1/part1_1/400px-raspi-con1.png)

RaspberryPi と接続したイーサネットデバイスのプロパティを開きます．

![raspi-con2.png](../../../images/part1/part1_1/400px-raspi-con2.png)

インターネットプロトコルバージョン 4（TCP/IPv4）のプロパティを開きます．

![raspi-con3.png](../../../images/part1/part1_1/400px-raspi-con3.png)

IP アドレスとサブネットマスクを設定します．

- IP アドレス: 192.168.1.102
- サブネットマスク: 255.255.255.0

![raspi-con4.png](../../../images/part1/part1_1/400px-raspi-con4.png)

コマンドプロンプトから ipconfig コマンドを実行し，IP アドレスとサブネットマスクが設定されていることを確認する．

![raspi-con5.png](../../../images/part1/part1_1/400px-raspi-con5.png)

### RaspberryPi との接続確認

コマンドプロンプトから RaspberryPi の IP アドレスに ping を実行し，接続を確認する．

![raspi-con6.png](../../../images/part1/part1_1/400px-raspi-con6.png)

## PC から RaspberryPi への VNC による接続

### VNC Client による接続

既にインストール済の VNC Client を起動し，RaspberryPi に設定済みの固定 IP アドレス 192.168.1.101 に接続します．

- ユーザ名: pi
- パスワード: raspberry

VNC を起動し，RaspberryPi の IP アドレス (192.168.1.101) に接続します．

![vnc1.png](../../../images/part1/part1_1/400px-vnc1.png)

初回のみセキュリティ警告が出ますので Continue します．

![vnc2.png](../../../images/part1/part1_1/400px-vnc2.png)

ユーザ名とパスワードを入力します．

![vnc3.png](../../../images/part1/part1_1/400px-vnc3.png)

デスクトップ画面にログインできます．

![vnc4.png](../../../images/part1/part1_1/400px-vnc4.png)

### パスワードの変更

初期パスワードのままではセキュリティ上問題がありますので，パスワード変更を行います．

ターミナルを開いて，以下のコマンドでパスワード変更をしてください．

パスワード変更はコンソールログイン，VNC 接続や SSH 接続，sudo でのコマンド実行など全てに影響します．

ここで設定したパスワードを忘れると復旧は難しいので特に注意してください．

```shell
 $ passwd
```

### 無線 LAN の無効化

![wifi-off.png](../../../images/part1/part1_1/wifi-off.png)

RaspberryPi のセキュリティアップデートを実施するなど（最初に配布したイメージは配布時点のセキュリティアップデート済です），特にネットワーク接続が必要な場合以外は無線 LAN を無効にしておきます．

今回の実験では PC と接続している有線 LAN ネットワークを利用しますので，まだ十分な知識とスキルを身に着けていない方は，混乱がないよう無線 LAN ネットワークを無効にしておいてください．

画像のように表示される場合は既に Wifi は OFF に設定されています．

## SSH クライアントのインストール

RaspberryPi へのファイル転送には SSH プロトコルを利用します．SSH プロトコルはターミナル接続用途にもファイル転送用途にも利用できます（通信アプリケーションは異なります）．

### Windows を使う場合

Web アプリケーション開発に必要なソフトは 3 つです

- SSH (ターミナル接続)
  - お勧め:Putty [https://www.putty.org/](https://www.putty.org/)
  - （お勧め:TeraTerm [https://osdn.net/projects/ttssh2/releases/](https://osdn.net/projects/ttssh2/releases/) ）
- SCP (ファイル転送)
  - お勧め：WinSCP [http://winscp.net/eng/docs/lang:jp](http://winscp.net/eng/docs/lang:jp)

### Linux を使う場合

- SSH (ターミナル接続)
  - 大抵の場合標準で使用できます．コマンドラインから ssh と打ってみてください．
  - [http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230889/](http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230889/)
- SCP (ファイル転送)
  - 大抵の場合標準で使用できます．コマンドラインから scp と打ってみてください．
  - [http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230878/](http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230878/)

## RaspberryPi へのターミナル接続

Windows 環境からの接続を例に説明します．

- TeraTerm を起動，予め調べておいた RaspberryPi の IP アドレス（192.168.1.101）を入力して SSH で接続

![tterm1.png](../../../images/part1/part1_1/400px-tterm1.png)

- RaspberryPi 上のアカウント（pi）でログイン

![tterm3.png](../../../images/part1/part1_1/400px-tterm3.png)

- 初回のみセキュリティ警告が出てくるので\[続行\]を押す

![tterm2.png](../../../images/part1/part1_1/400px-tterm2.png)

- ログインに成功すればコマンドプロンプトが出てきます．

![tterm4.png](../../../images/part1/part1_1/400px-tterm4.png)

## RaspberryPi へのファイル転送

WinSCP を起動します． ログイン画面が開くので新規のホストを設定しログインします．

- 転送プロトコル: SFTP
- ホスト名: 192.168.1.101
- ユーザ名: pi
- パスワード: 設定したパスワード

![winscp1.png](../../../images/part1/part1_1/600px-winscp1.png)

最初にアクセスした時のみセキュリティ警告が出ますので OK します．

![winscp2.png](../../../images/part1/part1_1/400px-winscp2.png)

エクスプローラのようにドラッグ＆ドラップや右クリックメニューでファイルの転送や操作が可能です．

![winscp3.png](../../../images/part1/part1_1/600px-winscp3.png)

## VM から RaspberryPi への接続の確認

### VM 環境の準備

- [Virtual Box と VM の準備](./VirtualBox_preparation)を参考に，事前に準備しておいてください．

### VM 環境から RaspberryPi への SSH 接続の確認

VM 上の Linux のターミナルを開いて，RaspberryPi に Ping を送って通信状態を確認します．

```shell
 $ ping 192.168.1.101
```

![vm2raspi1.png](../../../images/part1/part1_1/400px-vm2raspi1.png)

SSH で RaspberryPi にログインします．VM 上の Linux とユーザ名が異なりますので，明示的にログインユーザ名を指定してください（ -l pi ）．

最初の接続のみ，ホスト鍵を受け入れるか聞かれますので "yes" とします．

```shell
 $ slogin 192.168.1.101 -l pi
```

![vm2raspi2.png](../../../images/part1/part1_1/400px-vm2raspi2.png)

ユーザ名とパスワードが一致すれば，ログインできます．

![vm2raspi3.png](../../../images/part1/part1_1/400px-vm2raspi3.png)

## VM 上の Linux に PC からファイルを転送

ホスト OS（Windows）からゲスト OS（Linux）にファイルを転送します．

ゲスト OS 上に ssh server が起動していることが前提となりますので，インストールされていない場合は以下のコマンドでインストールします．

```shell
 $ sudo apt-get update
 $ sudo apt-get install openssh-server
```

VirtualBox のメニューから「仮想マシン」→「設定」を開きます．

![scpvm1.png](../../../images/part1/part1_1/400px-scpvm1.png)

ネットワークの設定から「高度」タブを開き「ポートフォワーディング」を開きます．

![scpvm2.png](../../../images/part1/part1_1/400px-scpvm2.png)

右上のアイコンをクリックして，フォワーディングルールを追加します．

ここでは，ホスト OS（Windows）のポート 22 への接続をゲスト OS（Linux）のポート 22 に転送する設定を行います．

![scpvm3.png](../../../images/part1/part1_1/400px-scpvm3.png)

WinSCP を起動し，接続先のホストとして以下の指定して接続します．

- ホスト名: localhost
- ユーザ名: ゲスト OS に設定したユーザ名
- パスワード: ゲスト OS に設定したパスワード

![scpvm4.png](../../../images/part1/part1_1/400px-scpvm4.png)

### 外部から接続する場合

VirtualBox の設定上は外部から SSH 接続できる設定ですが，実際には Windows ファイアウォールが接続を遮断して接続できない場合がありますので注意してください．

例えば，RaspberryPi から Windows のイーサネットアダプタに設定した IP アドレスへ SSH 接続を試みると，ポートフォワーディング設定並びに，ユーザ名・パスワードが正しければログインできるはずですが，ログインできない場合があります，

```shell
 $ slogin 192.168.1.102 -l XXXX （Linux上のユーザ名）
```

この場合，Windows ファイアウォールの設定を行えば接続が可能になりますが，セキュリティ上のリスクを伴いますので無理に行う必要はありません． 実験では，RaspberryPi 側をサーバプログラム，PC 上のゲスト OS 側をクライアントプログラムとして実験を進めてください．

## テキストエディタを使った接続 (任意)

テキストエディタ Atom を用いて開発環境の構築を行います.
興味のある方は実施してみてください.

なお, 上記の SSH 接続や WinSCP の設定などが済んでいることを前提として話を進めていきます.
必ず上記の SSH 接続や WinSCP の設定を済ましてから取り組んでください.

[Atom を使った環境構築](./environment_building_with_atom)