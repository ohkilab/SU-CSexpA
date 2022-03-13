# HTTPサーバ機能拡張
## はじめに

昨今の一般的なWebサーバがサポートしている以下の機能をSimple HTTP Serverに実装してみてください． 選択課題となりますので，以下のいずれか（複数選択可）について実装を行ってみましょう（最低1つは実装してください）．

-   各種ステータスコードへの対応
-   各種認証機能の実装
-   各種メディアタイプへの対応
-   HTML5動画への対応
-   Ajaxへの対応
-   POSTへの対応
-   CGIへの対応
-   その他の機能拡張（Keep-Alive, WebSocket, HTTPS, HTTP/2など）

## 3XX系ステータスコードへの対応

HTTPのステータスコードは，[HTTPステータスコード - Wikipedia](http://ja.wikipedia.org/wiki/HTTP%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)にも大凡まとまっています．3XX系のステータスコードはリダイレクションするためのステータスコードになります．最低でも301，302，303の3種類は実装してみてください．

-   [リダイレクト時のステータスコードには何を使えばいいか？](http://www.symmetric.co.jp/blog/archives/123)
-   [新人コーダーに知っておいて欲しいリダイレクトの基本](http://html-coding.co.jp/knowhow/tips/000572/)

ブラウザのデベロッパーツールを使って適切に動作しているか確認してみてください．

## 4XX系ステータスコードへの対応

4xx系のステータスコードは，アクセス権限系のエラーを返す際に使います．よく見るのは「404 Not Found (ファイルが見つからなかった場合)」で，これは既に実装してあります．最低でも401，403，418を追加で実装してみてください．

401は，認証とセットで取り組む必要があります．

403は，指定されたファイルへのアクセス権限を確認すれば実装できると思います．

418は，何なのか相談しながら実装してみてください．

## Basic認証の実装

最も良く使用されている基本的な認証方式ですので実装してみてください．

ただし，[Basic認証 - Wikipedia](http://ja.wikipedia.org/wiki/Basic%E8%AA%8D%E8%A8%BC)は，ユーザ名とパスワードをBASE64でエンコードしているだけですので，[Base64のデコード - オンラインBase64のデコーダ](http://www.convertstring.com/ja/EncodeDecode/Base64Decode)などで簡単にユーザ名とパスワードをデコードできますのでご注意ください．

ご参考まで，Base64の関数はネット上に落ちてますし，C言語のsystem関数を使って[Linuxのbase64コマンド](http://linuxjm.osdn.jp/html/GNU_coreutils/man1/base64.1.html)を 実行するという方法もあります．以下は，C言語のsystem関数を使ってBase64の文字列をデコードするサンプルソースコードです．このサンプルでは，tmpnam()を用いて一時的に作成したファイルへデコード結果を書き出し，catコマンドで表示しています（ただし以下に示すようにtmpnam()の使用にも注意しましょう）．

```c
#include "exp1.h"

int main()
{
  char str_base64[] = "ZXhwMTpzaGl6dXB5X2hhX3Bha3VyaQo=";
  char tmpfile[L_tmpnam];
  char cmd[1024];

  tmpnam(tmpfile);
  printf("tmpfile is %s\n", tmpfile);

  sprintf(cmd, "echo %s | base64 -d > %s", str_base64, tmpfile);
  system(cmd);

  sprintf(cmd, "cat %s", tmpfile);
  system(cmd);
  remove(tmpfile);
}
```
-   [C言語 一時ファイル名(テンポラリファイル名)の作成 - stdio.h - \[ tmpnam](http://simd.jugem.jp/?eid=61) | 勇躍のゴミ箱\]
-   [IPA ISEC　セキュア・プログラミング講座：C/C++言語編　第7章 データ漏えい対策：テンポラリファイル（Unix の一時ファイル）](http://www.ipa.go.jp/security/awareness/vendor/programmingv2/contents/c603.html)
-   [C言語のtmpnam()って関数を知った](http://blog.clouder.jp/2008/10/29/ctmpnam/)

## Digest認証の実装

Basic認証は，httpsを使用していないと比較的簡単にデコードできてしまうという問題があります（といっても平文よりはマシですが）．

そこで，md5を用いた[Digest認証](http://ja.wikipedia.org/wiki/Digest%E8%AA%8D%E8%A8%BC)を実装してみてください．Basic認証をベースに実装できると思います．

## 各種メディアタイプへの対応

各種 [Internet media type](http://en.wikipedia.org/wiki/Internet_media_type) に対する処理を追加して，Webブラウザで表示できるようにしてください．

exp1\_check\_file()関数を拡張していくのが楽と思います．

## HTML5動画への対応

昨今のWebブラウザはほぼ対応済みのHTML5による動画再生へ対応させてください．

-   [videoタグで動画を埋め込む ★HTML5リファレンス](http://www.htmq.com/html5/004.shtml)

## Ajaxへの対応

Ajaxとは，Asynchronous Javascript+XMLのことで，Javascriptを使ってWebページとは非同期（Asynchronous）にXML形式（もしくはテキスト形式）のデータ通信を行う手法を指します．画面遷移を伴わずに動的なWebページを実現できるだけでなく，Webページを構成する部分画像など必要となった時にデータ通信を要求するためページ表示の高速化にもつながります．

jQueryのようなJavascriptライブラリを用いることで，WebブラウザによるJavascriptの実装の違いを吸収したAjaxやDOMプログラミングコードを簡潔に書くことができます．実装したら，FireBug等で実際にAjax通信が行われていることを確認してみてください．

## POSTへの対応

POSTメソッドへ対応させてください．

## CGIへの対応

system()システムコールを用いて，phpやcgiを呼び出すようにすれば実装できると思います．

POSTメソッドで受信されたデータにも対応できるようにしてみてください．

## その他の機能拡張

-   Comet (long polling)への対応
-   Keep-Aliveへの対応
-   SSE (Server Sent Events)への対応
-   WebSocketへの対応
-   WebWorkersへの対応
-   HTTPSへの対応
-   HTTP/2への対応
-   などなど

## おわりに

Webシステムの根幹をなすWebサーバとWebクライアント，それらのやりとりで使用されるHTTPについて十分理解し，実装や拡張できるようになりましたか？

Webに関係する技術の進化は早いですが，本質を理解し身に着けることができれば，どの業界に就職したとしても十分通用する知識や技術としてきっと役立つことと思います．