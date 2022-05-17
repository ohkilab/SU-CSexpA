# ファイル入出力プログラミング

## はじめに

この課題の目的はネットワークプログラミングを実施する前に，入出力プログラミングの典型例としてファイル入出力プログラミングを確認・復習することです．
入出力プログラミングで用いる基本的な概念や関数について確認しながら課題を進めてください．

基本的に任意の入出力に関する振る舞いが，指定したデバイスに対する入出力関数（open, read, writeなど）として抽象化されており，その上で，それらの関数をラッピングする形で特定のデバイス用の入出力関数（fopen, fread, fwrite）が設計されています．

### 参考情報

-   [システムコールと標準ライブラリ関数の違いを知る](http://www.atmarkit.co.jp/ait/articles/1112/13/news117.html)
-   [デバッグのコツ](http://www.slideshare.net/kaityo256/130613debug)

## 事前準備

-   1\. RaspberryPiにログインする
-   2\. ホームディレクトリに以下のサンプルコードファイルをダウンロードする．
    -   [https://exp1.inf.shizuoka.ac.jp/shizudai-only/day2/part1-2B.tgz](https://exp1.inf.shizuoka.ac.jp/shizudai-only/day2/part1-2B.tgz)（静大のネットワークからのみアクセス可能）
    -   `\\fs.inf.in.shizuoka.ac.jp\share\class\情報科学実験A\第一部サンプルコード\part1-2B.tgz`（静大のネットワーク内からはWindowsファイル共有にて．外部からはVPNを利用してアクセス可能）

-   3\. ダウンロードしたサンプルコードファイルを展開する

```shell
 $ tar zxvf part1-2B.tgz
 $ ls part1-2B
 001  003  005  007client  008client  009client  010client  exp1.h     exp1lib.h
 002  004  006  007server  008server  009server  010server  exp1lib.c
```

## exp1.h

-   各サンプルコードで必要なヘッダファイルが類似しているのでまとめてincludeするヘッダファイルを作成してあります．各サンプルコードで読み込むようにしてください．

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <float.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
```

## ビルド方法

各サンプルコードディレクトリ（e.g. `001/`）以下にはビルドのためのMakefileを作成してあります．

```shell
 $ ls
 001.c  Makefile
```

このMakefileを利用して，以下のようにmakeコマンドでソースコードをビルドします．

```shell
 $ make
```

Makefileの内容はサンプルコードによって異なりますが，典型的には以下のような記述になっています．

```makefile
 CC=gcc
 C++=g++
 LD=g++
 CFLAGS=-c -Wall -pedantic-errors -O3 -std=gnu11 -I../
 LDFLAGS=
 OBJECTS=001.o
 EXECUTABLE=001

 all: $(EXECUTABLE)

 $(EXECUTABLE): $(OBJECTS)
         $(LD) $(LDFLAGS) $(OBJECTS) -o $@

 .cpp.o:
         $(C++) $(CFLAGS) $< -o $@

 .c.o:
         $(CC) $(CFLAGS) $< -o $@

 clean:
         -rm -f ${EXECUTABLE} ${EXECUTABLE}.exe ${OBJECTS} core
```

`-pedantic-errors`は，厳密な文法チェックを行うためのオプションです．
警告が残っていてもコンパイルが完了する場合がありますがバグの原因になりますので，必ず警告も全て出ないように解決させておいてください．
警告が残っているプログラムはチェック対象としませんのでご注意ください．

`-O3`は，最適化オプションです．

`-I../` は，include ファイルの探索パスに1つ上のディレクトリを追加する指定です．
この指定によって，ソースコード中では `#include "exp1.h"` と記述した場合に `../exp1.h` を読み込むことができるようになります．

```shell
 CFLAGS=-c -Wall -pedantic-errors -O3 -std=c11 -I../
```

## 実行方法

ビルドが成功すると Makefile の以下の箇所で指定されたファイル名（ただし，Windows上のCygwinでビルドした場合は拡張子.exeが付与される）の実行ファイルが作成されます．

```shell
 EXECUTABLE=001
```

以下のようにコマンドライン上で実行ファイルのファイル名を指定して実行します（実行に必要な引数などはサンプルコードによって異なります）．

```shell
 $ ./001
 The number of arguments is 1
 argument 0 is ./001
```

### 参考情報

-   [Man page of GCC](http://linuxjm.sourceforge.jp/html/GNU_gcc/man1/gcc.1.html)

## \[必須課題1\] コマンドライン引数

### ソースコード: 001.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  printf("The number of arguments is %d\n", argc);
  for(int i=0 ; i<argc ; i++) {
    printf("argument %d is %s\n", i, argv[i]);
  }
  return 0;
}

```

-   参考: [Man page of printf](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/printf.3.html)

### 実行方法

コマンドラインから引数を付けて実行します．
コマンドライン引数をいろいろ変えてみてソースコードの中身をしっかりと理解してください．

```shell
$ ./001 hello world
The number of arguments is 3
argument 0 is ./001
argument 1 is hello
argument 2 is world
```

## \[必須課題2\] システムコールで標準入出力

### ソースコード: 002.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  char buf[4];
  int ret;

  ret = read(0, buf, 4);
  printf("read %d bytes\n", ret);

  ret = write(1, buf, ret);
  printf("write %d bytes\n", ret);

  return 0;
}
```

readとwriteは，入出力を行うシステムコールです．

-   [Man page of read](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/read.2.html)
-   [Man page of write](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/write.2.html)

readとwriteで与えられている引数のファイル記述子(fd: file descriptor)は，0が標準入力，1が標準出力を意味しています．

-   参考: [ファイル記述子](http://ja.wikipedia.org/wiki/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E8%A8%98%E8%BF%B0%E5%AD%90)

### 実行方法

実行するとキーボードの入力を待ち受けます．

以下では「aa」と入力してEnterを押しています．

読み込みが3 bytesになっているのは改行コード(\\n)が含まれているからです．

入力をいろいろと変えながら（例えば "aaaaaa" と入力したらどうなるでしょう）ソースコードの内容と挙動を理解してください．

```shell
$ ./002
aa
read 3 bytes
aa
write 3 bytes
```

## \[必須課題3\] システムコールでファイル入出力

### ソースコード: 003.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  int fd;
  int ret;
  char buf[4];

  if (argc != 2) {
    printf("usage: %s [filename]\n", argv[0]);
    exit(-1);
  }

  fd = open(argv[1], O_RDONLY);
  printf("file descriptor = %d\n", fd);
  ret = read(fd, buf, 4);
  printf("read %d bytes\n", ret);
  write(1, buf, ret);
  close(fd);

  return 0;
}
```


必須課題2に加えてopenとcloseというシステムコールを使います．

-   参考: [Man page of open](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/open.2.html)
-   参考: [Man page of close](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/close.2.html)

### 実行方法

ファイルパスをコマンド引数に指定して実行します．

コマンド引数をいろいろと変えながらソースコードの内容と挙動を理解してください．

プログラム実行時のファイル識別子の値と，その値になっている理由を確認してください．

```shell
$ ./003 ../001/001.c
file descriptor = 3
read 4 bytes
```

## \[必須課題4\] 一般的なファイル入出力

### ソースコード: 004.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  FILE* fp;
  int ret;
  char buf[1024];

  if(argc != 2) {
    printf("usage: %s [filename]\n", argv[0]);
    exit(-1);
  }

  fp = fopen(argv[1], "r");
  ret = fread(buf, sizeof(char), 1024, fp);
  fwrite(buf, sizeof(char), ret, stdout);
  fclose(fp);

  return 0;
}
```

fopen，fread，fwrite，fcloseは，ファイルの入出力を行うopen，read，write，closeのラッパーです．

-   [Man page of fopen](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/fopen.3.html)
-   [Man page of fread](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/fread.3.html)
-   [Man page of fwrite](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/fwrite.3.html)
-   [Man page of fclose](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/fclose.3.html)

### 実行方法

ファイルパスをコマンド引数に指定して実行します．

コマンド引数をいろいろと変えながらソースコードの内容と挙動を理解してください．

fwriteのstdoutは，標準出力に対応付けられたファイル構造体へのポインタです．

プログラム実行時に1024バイトを超えるファイルを指定した場合の挙動とその原因を確認してください．

```shell
$ ./004 ../001/001.c
#include <stdio.h>

int main(int argc, char** argv)
{
  int i;

  printf("The number of arguments is %d\n", argc);
  for(i = 0; i < argc; i++){
    printf("argument %d is %s\n", i, argv[i]);
  }
}
```

## \[必須課題5\] 文字列を対象としたファイル入出力

### ソースコード: 005.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  FILE* fp;
  char* p;
  char buf[1024];

  if(argc != 2) {
    printf("usage: %s [filename]\n", argv[0]);
    exit(-1);
  }

  fp = fopen(argv[1], "r");
  p = fgets(buf, 1024, fp);
  fputs(p, stdout);
  fclose(fp);

  return 0;
}
```

fgets，fputsは，文字列の入出力を行うread，writeのラッパーです．

-   [Man page of puts](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/putc.3.html)
-   [Man page of fgets](http://linuxjm.sourceforge.jp/html/LDP_man-pages/man3/getc.3.html)

### 実行方法

ファイルパスをコマンド引数に指定して実行します．

コマンド引数をいろいろと変えながらソースコードの内容と挙動を理解してください．

```shell
$ ./005 ../001/001.c
#include <stdio.h>
```

## \[必須課題6\] ファイルのコピー

### ソースコード: 006.c

```c
#include "exp1.h"

int main(int argc, char** argv) {
  FILE* fpr;
  FILE* fpw;
  int ret;
  char buf[4];

  if(argc != 2) {
    printf("usage: %s [filename]\n", argv[0]);
    exit(-1);
  }

  fpr = fopen(argv[1], "r");
  fpw = fopen("tmp.txt", "w");

  ret = fread(buf, sizeof(char), 4, fpr);
  while (ret > 0) {
    fwrite(buf, sizeof(char), ret, fpw);
    ret = fread(buf, sizeof(char), 4, fpr);
  }
  fclose(fpr);
  fclose(fpw);

  return 0;
}

```

whileで繰り返し処理することでバッファサイズよりも大きなデータを扱うことができます．

上記の例では分かりやすくするためにバッファサイズを4にしています．

### 実行方法

ファイルパスをコマンド引数に指定して実行します．

コマンド引数をいろいろと変えながらソースコードの内容と挙動を理解してください．

```shell
$ cat tmp.txt
cat: tmp.txt: No such file or directory
$ ./006 ../001/001.c
$ cat tmp.txt
#include <stdio.h>

int main(int argc, char** argv)
{
  int i;

  printf("The number of arguments is %d\n", argc);
  for(i = 0; i < argc; i++){
    printf("argument %d is %s\n", i, argv[i]);
  }
}
```
