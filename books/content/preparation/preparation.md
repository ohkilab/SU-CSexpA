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
情報科学実験Aはグループを構成して実験を進める演習となっています．基本的には，ノートPC上にLinux環境を構築して通信アプリケーションの片側を実行し，Raspberry Piを接続してその通信内容や通信性能を計測・分析しながら実験を進めます．ただし，時にはグループ内の複数のRaspberryPi間で通信を行う必要があるかもしれません．その場合，ノートPCとRaspberryPi同士の通信環境とは異なる環境になる点に注意してください．異なる環境間で計測結果の比較を行うことは妥当ではありませんが，通信アプリケーションの開発や動作確認には使うことができるでしょう．

![vm-raspi.jpeg](../../images/preparation/vm-raspi.jpeg)

### Virtual BoxとLinux VMの導入

Virtual Boxを導入してください．拡張パック（Extension Pack）も合わせて導入してください．

[Oracle VM VirtualBox](https://www.virtualbox.org/wiki/Downloads)

静岡大学情報学部で学年度ごとに用意しているLinux VMとインストールガイドは以下にあります． 以下のoooo部分は入学年度で置き換えてください．

[http://whitebear.cs.inf.shizuoka.ac.jp/linux/oooo/](http://whitebear.cs.inf.shizuoka.ac.jp/linux/oooo/)

## Slackへの登録
本年度は学生間の連絡にSlackを使います．機材配布時に配った資料を参考に，Slackへの参加登録を済ませておいてください．