# 非同期IOによる多重化 - 情報科学実験I

[TOC]

## はじめに

ノンブロッキング非同期I/Oライブラリを用いてサーバプログラムを多重化し，他の多重化方法との性能比較を実施せよ．

ノンブロッキング非同期I/Oライブラリを利用する場合，通常コールバック関数を利用した設計になりますのでこれまでのサンプルコードとは構造が異なりますので注意してくだｓさい．

ノンブロッキング非同期I/Oライブラリはいくつか公開されていますが，まずは以下の2種類を比較してみてください． 余力があれば，別のライブラリを利用した場合や，epoll, kqueueなどのシステムコールを直接扱う場合を比較しても良いでしょう．

-   libevent ([https://libevent.org](https://libevent.org/))
-   libuv ([https://libuv.org](https://libuv.org/))

最初に利用するライブラリをインストールしておきます．

```shell
$ sudo apt install libevent-dev
$ sudo apt install libuv1 libuv1-dev
```