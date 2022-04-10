# 環境構築

## VNCクライアントのインストール
皆さんのノートPCにインストールされているWindowsから，ネットワーク接続を介してリモートでRaspberry Piのデスクトップ環境を利用するために，VNCクライアント（サーバは不要です）をノートPCのWindowsへインストールしておきます．
Raspberry Pi上のRaspbian OS に標準でインストールされている VNC Server との接続は，以下の Real VNC のクライアントソフトウェアが接続しやすいようです（他のVNCクライアントソフトウェアでもサーバ側の設定を変更すれば接続可能）．ノートPCのWindowsへクライアント（VNC Viewer）のみをインストールして下さい（WindowsへVNC Serverをインストールする必要はありません）．Real VNC社のVNC Connectは商用ソフトウェアですが，Viewerに関しては Free で配布されています．

[RealVNC](https://www.realvnc.com/en/connect/download/viewer/windows/)

> RealVNC社の "Frequently asked questions" より引用
> Do I need to license the software?
> No. VNC® Viewer is always free to use. Just accept the EULA the first time you run.

## Virtual BoxとVMの準備

### 概要
従来，情報科学実験Aではグループを構成して実験を進める計画になっています．
基本的には皆さんのノートPCとRaspberry Pi間での通信実験を行いますが，たとえばグループ内の複数のRaspberry Pi間での通信を行うことで，通信環境の違いによる通信性能への影響を計測・分析することもあります．様々な場合に対応できるように，ここでは各自のノートPC上にLinux環境を構築しておきましょう．

![vm-raspi.jpeg](../../images/preparation/vm-raspi.jpeg)

### Virtual BoxとLinux VMの導入

```{important}
他教科（ネットワークプログラミングなど）と異なる設定を行うことや，様々なソフトウェアをインストールすることで環境が変わる可能性があるため，本講義で利用するためのVMは，本講義用のものを個別に構築しておくことをおすすめします．
```

[Oracle VM VirtualBox](https://www.virtualbox.org/wiki/Downloads)

本講義用のVMを構築する方法は基本的に1年生の授業で構築した方法と同じです．
静岡大学情報学部で学年度ごとに用意しているLinux VMとインストールガイドは以下にありますので，参照しながらVM構築を行ってください． 
（oooo部分は入学年度で置き換えてください．）

[http://whitebear.cs.inf.shizuoka.ac.jp/linux/oooo/](http://whitebear.cs.inf.shizuoka.ac.jp/linux/oooo/)

新規VMの追加設定は後にも行います．上記ページのダウンロード後の設定手順を終えたらそのままにしておいてください．

## Slackへの登録
本年度は学生間の連絡にSlackを使います．機材配布時に配った資料を参考に，Slackへの参加登録を済ませておいてください．
