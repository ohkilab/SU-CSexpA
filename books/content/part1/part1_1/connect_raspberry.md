# RaspberryPiの接続（固定IP） - 情報科学実験I

## 概要

実験室以外で実験を行う場合に，ディスプレイとマウス・キーボードがない状態でRaspberryPiへ接続する方法を説明します．

情報科学実験Iで配布したRaspberryPiはEthernetデバイスに固定IP（192.168.1.101）を設定してありますので，それを前提としてPCから接続します．

## RaspbarryPiとPCの有線ケーブルでのP2P接続

### RaspberryPiを電源と接続

RaspberryPiのmicroUSB端子に電源を接続します．

[![raspi-usb.jpg](https://exp1.inf.shizuoka.ac.jp/images/thumb/9/91/raspi-usb.jpg/400px-raspi-usb.jpg)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-usb.jpg)

### RaspberryPiとPCとの接続

RaspberryPiとPCをLANケーブルで直結します（以下の例ではUSB接続のLANアダプタを使っていますが，PCに備え付けの有線LANポートがあればそれに接続します）．

[![raspi-pc.jpg](https://exp1.inf.shizuoka.ac.jp/images/3/3b/raspi-pc.jpg)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-pc.jpg)

### PCの有線LANデバイスにIPアドレスを設定

WindowsPC上のイーサネットデバイスにRaspberryPiと同じサブネットワークのIPアドレス（192.168.1.102）を設定します．

ただし，Windows上で複数のイーサネットデバイスを認識している場合がありますので（特にVirtualBoxなどのVM環境を構築している場合は仮想イーサネットデバイスが存在するはず），RaspberryPiと接続したイーサネットデバイスを確認してから作業を進めてください．

設定の「ネットワークとインターネット」の「状態」から「アダプターのオプションを設定する」を選択します．

[![raspi-con1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/4/4c/raspi-con1.png/400px-raspi-con1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con1.png)

RaspberryPiと接続したイーサネットデバイスのプロパティを開きます．

[![raspi-con2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/5/54/raspi-con2.png/400px-raspi-con2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con2.png)

インターネットプロトコルバージョン4（TCP/IPv4）のプロパティを開きます．

[![raspi-con3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/2/29/raspi-con3.png/400px-raspi-con3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con3.png)

IPアドレスとサブネットマスクを設定します．

-   IPアドレス: 192.168.1.102
-   サブネットマスク: 255.255.255.0

[![raspi-con4.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/1/18/raspi-con4.png/400px-raspi-con4.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con4.png)

コマンドプロンプトから ipconfig コマンドを実行し，IPアドレスとサブネットマスクが設定されていることを確認する．

[![raspi-con5.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/9/93/raspi-con5.png/400px-raspi-con5.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con5.png)

### RaspberryPiとの接続確認

コマンドプロンプトからRaspberryPiのIPアドレスに ping を実行し，接続を確認する．

[![raspi-con6.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/7/74/raspi-con6.png/400px-raspi-con6.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:raspi-con6.png)

## PCからRaspbarryPiへのVNCによる接続

### VNC Clientによる接続

既にインストール済のVNC Clientを起動し，RaspberryPiに設定済みの固定IPアドレス 192.168.1.101 に接続します．

-   ユーザ名: pi
-   パスワード: raspberry

VNCを起動し，RaspberryPiのIPアドレス (192.168.1.101) に接続します．

[![vnc1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/6/6b/vnc1.png/400px-vnc1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vnc1.png)

初回のみセキュリティ警告が出ますのでContinueします．

[![vnc2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/6/63/vnc2.png/400px-vnc2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vnc2.png)

ユーザ名とパスワードを入力します．

[![vnc3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/9/9b/vnc3.png/400px-vnc3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vnc3.png)

デスクトップ画面にログインできます．

[![vnc4.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/2/2c/vnc4.png/400px-vnc4.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vnc4.png)

### パスワードの変更

初期パスワードのままではセキュリティ上問題がありますので，パスワード変更を行います．

ターミナルを開いて，以下のコマンドでパスワード変更をしてください．

パスワード変更はコンソールログイン，VNC接続やSSH接続，sudo でのコマンド実行など全てに影響します．

ここで設定したパスワードを忘れると復旧は難しいので特に注意してください．

```shell
 $ passwd
```

### 無線LANの無効化

[![wifi-off.png](https://exp1.inf.shizuoka.ac.jp/images/5/57/wifi-off.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:wifi-off.png)

RaspberryPiのセキュリティアップデートを実施するなど（最初に配布したイメージは配布時点のセキュリティアップデート済です），特にネットワーク接続が必要な場合以外は無線LANを無効にしておきます．  


今回の実験ではPCと接続している有線LANネットワークを利用しますので，まだ十分な知識とスキルを身に着けていない方は，混乱がないよう無線LANネットワークを無効にしておいてください．


画像のように表示される場合は既にWifiはOFFに設定されています．

## SSHクライアントのインストール

RaspberryPiへのファイル転送にはSSHプロトコルを利用します．SSHプロトコルはターミナル接続用途にもファイル転送用途にも利用できます（通信アプリケーションは異なります）．

#### Windowsを使う場合

Webアプリケーション開発に必要なソフトは3つです

-   SSH (ターミナル接続)
    -   お勧め:Putty [https://www.putty.org/](https://www.putty.org/)
    -   （お勧め:TeraTerm [https://osdn.net/projects/ttssh2/releases/](https://osdn.net/projects/ttssh2/releases/) ）
-   SCP (ファイル転送)
    -   お勧め：WinSCP [http://winscp.net/eng/docs/lang:jp](http://winscp.net/eng/docs/lang:jp)

#### Linuxを使う場合

-   SSH (ターミナル接続)
    -   大抵の場合標準で使用できます．コマンドラインからsshと打ってみてください．
    -   [http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230889/](http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230889/)
-   SCP (ファイル転送)
    -   大抵の場合標準で使用できます．コマンドラインからscpと打ってみてください．
    -   [http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230878/](http://itpro.nikkeibp.co.jp/article/COLUMN/20060227/230878/)

## RaspberryPiへのターミナル接続

Windows環境からの接続を例に説明します．

-   TeraTermを起動，予め調べておいたRaspberryPiのIPアドレス（192.168.1.101）を入力してSSHで接続

[![tterm1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/7/74/tterm1.png/400px-tterm1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:tterm1.png)

-   RaspberryPi上のアカウント（pi）でログイン

[![tterm2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/a/a5/tterm2.png/400px-tterm2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:tterm2.png)

-   初回のみセキュリティ警告が出てくるので\[続行\]を押す

[![tterm3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/7/70/tterm3.png/400px-tterm3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:tterm3.png)

-   ログインに成功すればコマンドプロンプトが出てきます．

[![tterm4.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/2/23/tterm4.png/400px-tterm4.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:tterm4.png)

## RaspberryPiへのファイル転送

WinSCPを起動します． ログイン画面が開くので新規のホストを設定しログインします．

-   転送プロトコル: SFTP
-   ホスト名: 192.168.1.101
-   ユーザ名: pi
-   パスワード: 設定したパスワード

[![winscp1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/2/28/winscp1.png/600px-winscp1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:winscp1.png)\]

最初にアクセスした時のみセキュリティ警告が出ますのでOKします．

[![winscp2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/0/0c/winscp2.png/400px-winscp2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:winscp2.png)\]

エクスプローラのようにドラッグ＆ドラップや右クリックメニューでファイルの転送や操作が可能です．

[![winscp3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/7/7d/winscp3.png/600px-winscp3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:winscp3.png)

## VMからRaspbarryPiへの接続の確認

### VM環境の準備

-   [Virtual BoxとVMの準備](https://exp1.inf.shizuoka.ac.jp/Virtual_Box%E3%81%A8VM%E3%81%AE%E6%BA%96%E5%82%99 "Virtual BoxとVMの準備")を参考に，事前に準備しておいてください．

### VM環境からRaspbarryPiへのSSH接続の確認

VM上のLinuxのターミナルを開いて，RaspberryPiにPingを送って通信状態を確認します．

```shell
 $ ping 192.168.1.101
```

[![vm2raspi1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/6/6d/vm2raspi1.png/400px-vm2raspi1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vm2raspi1.png)

SSHでRaspberryPiにログインします．VM上のLinuxとユーザ名が異なりますので，明示的にログインユーザ名を指定してください（ -l pi ）．

最初の接続のみ，ホスト鍵を受け入れるか聞かれますので "yes" とします．

```
 $ slogin 192.168.1.101 -l pi
```


[![vm2raspi2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/8/8a/vm2raspi2.png/400px-vm2raspi2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vm2raspi2.png)

ユーザ名とパスワードが一致すれば，ログインできます．

[![vm2raspi3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/4/47/vm2raspi3.png/400px-vm2raspi3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:vm2raspi3.png)

## VM上のLinuxにPCからファイルを転送

ホストOS（Windows）からゲストOS（Linux）にファイルを転送します．

ゲストOS上に ssh server が起動していることが前提となりますので，インストールされていない場合は以下のコマンドでインストールします．

```shell
 $ sudo apt-get update
 $ sudo apt-get install openssh-server
```

VirtualBoxのメニューから「仮想マシン」→「設定」を開きます．

[![scpvm1.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/6/67/scpvm1.png/400px-scpvm1.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:scpvm1.png)

ネットワークの設定から「高度」タブを開き「ポートフォーワーディング」を開きます．

[![scpvm2.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/2/20/scpvm2.png/400px-scpvm2.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:scpvm2.png)

右上のアイコンをクリックして，フォワーディングルールを追加します．

ここでは，ホストOS（Windows）のポート22への接続をゲストOS（Linux）のポート22に転送する設定を行います．

[![scpvm3.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/8/86/scpvm3.png/400px-scpvm3.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:scpvm3.png)

WinSCPを起動し，接続先のホストとして以下の指定して接続します．

-   ホスト名: localhost
-   ユーザ名: ゲストOSに設定したユーザ名
-   パスワード: ゲストOSに設定したパスワード

[![scpvm4.png](https://exp1.inf.shizuoka.ac.jp/images/thumb/f/f4/scpvm4.png/400px-scpvm4.png)](https://exp1.inf.shizuoka.ac.jp/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB:scpvm4.png)

### 外部から接続する場合

VirtualBoxの設定上は外部からSSH接続できる設定ですが，実際にはWindowsファイアウォールが接続を遮断して接続できない場合がありますので注意してください．

例えば，RaspberryPiからWindowsのイーサネットアダプタに設定したIPアドレスへSSH接続を試みると，ポートフォワーディング設定並びに，ユーザ名・パスワードが正しければログインできるはずですが，ログインできない場合があります，

```shell
 $ slogin 192.168.1.102 -l XXXX （Linux上のユーザ名）
```


この場合，Windowsファイアウォールの設定を行えば接続が可能になりますが，セキュリティ上のリスクを伴いますので無理に行う必要はありません． 実験では，RaspberryPi側をサーバプログラム，PC上のゲストOS側をクライアントプログラムとして実験を進めてください．

## テキストエディタを使った接続 (任意)

テキストエディタAtomを用いて開発環境の構築を行います.  
興味のある方は実施してみてください.

なお, 上記のSSH接続やWinSCPの設定などが済んでいることを前提として話を進めていきます.  
必ず上記のSSH接続やWinSCPの設定を済ましてから取り組んでください.

[Atomを使った環境構築](https://exp1.inf.shizuoka.ac.jp/Atom%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%9F%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89 "Atomを使った環境構築")