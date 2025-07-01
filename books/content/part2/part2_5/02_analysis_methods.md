# ボトルネック分析手法

## 測定・分析の重要性

「測定できないものは改善できない」という原則は、性能最適化において特に重要です。適切な測定と分析なしに行われる最適化は、しばしば間違った方向に向かい、貴重な時間と労力を無駄にしてしまいます。

### 分析手法の全体像

ボトルネック分析は以下の4つの段階で段階的に進めます：

1. **システム全体の性能測定** - Webアプリケーション全体のレスポンス時間とスループットを測定
2. **各層での詳細分析** - フロントエンド、アプリケーション、データベース層ごとの問題特定
3. **システムリソース分析** - CPU、メモリ、ディスクI/Oの使用状況を監視
4. **ボトルネック判別** - 制約の種類（CPU/メモリ/I/O制約型）を特定し、最適化戦略を決定

## システム全体の性能測定

### abコマンドによる性能測定

Apache Benchマーク（ab）は、Webサーバの性能を測定する標準的なツールです。

#### 基本的な使用法

```bash
# 基本的な計測（100リクエスト、同時実行数10）
ab -n 100 -c 10 http://localhost/progA.php?tag=beach

# 詳細結果の出力
ab -n 1000 -c 50 -v 2 http://10.70.174.167/~pi/progA.php?tag=dog
```

#### 重要な測定指標

**Time per request（平均応答時間）**
```
Time per request:       2847.123 [ms] (mean)
```

**Requests per second（スループット）**
```
Requests per second:    0.35 [#/sec] (mean)
```

**Transfer rate（転送速度）**
```
Transfer rate:          45.23 [Kbytes/sec] received
```

#### 負荷レベルの設定

負荷レベルやタグはサンプルですが，開発・デバッグ用や性能比較用，高負荷テスト用などに分けて実験をすると良いでしょう．

```bash
# 軽量テスト（開発・デバッグ用）
ab -n 10 -c 1 http://your-server/progA.php?tag=test

# 標準テスト（性能比較用）
ab -n 100 -c 10 http://your-server/progA.php?tag=test

# 高負荷テスト（最終評価用）
ab -n 1000 -c 50 http://your-server/progA.php?tag=test
```

### ベンチマークサーバでの計測

ベンチマークサーバは予選と本選で異なる設定となっているので注意が必要です．また，返答される計測結果から得られる情報も異なるので以下を参考にサーバの改善を行うようにしてください．

#### 予選期間の特徴
- 計測回数制限なし
- 任意のタグで測定可能
- 開発・デバッグに最適

#### 本選期間の特徴
- **最大10回まで**の計測制限
- **異なるタグ**での測定（グループ内で重複不可）
- **未知のタグ**での検索が前提

#### 計測結果の解釈

**Success（成功）**
- 正常に計測完了
- Request Per Second（RPS）が評価対象

**Connection Failed（接続失敗）**
- IPアドレス確認
- Webサーバ起動状態確認
- ファイアウォール設定確認

**Validation Error（検証エラー）**
- JSON形式の確認
- Content-Type: application/json
- 必須フィールドの確認

**Timeout（タイムアウト）**
- 処理時間の最適化が必要
- 制限時間内での応答が必要

## 各層での分析ポイント

システム全体の性能測定で問題を特定したら、次は各層での詳細分析を行います。Webアプリケーションは複数の層で構成されており、それぞれ異なる特性とボトルネックを持ちます。

### フロントエンド層（ブラウザ）

フロントエンド層は、ユーザーが直接操作するWebブラウザ上での処理を担当します。HTMLレンダリング、JavaScriptの実行、CSS適用、Ajax通信などが主な処理となり、ユーザーの体感性能に直結します。特にネットワーク通信の遅延やJavaScript処理の重さが、ユーザーエクスペリエンスに大きく影響するため、適切な測定と分析が重要です。

#### 分析対象
- HTMLレンダリング時間
- JavaScriptの実行時間
- CSSの適用時間
- Ajax通信の待機時間

#### 測定方法
```javascript
// JavaScript実行時間測定
console.time('search-execution');
// 検索処理
console.timeEnd('search-execution');

// ネットワーク通信時間測定
const startTime = performance.now();
fetch('/progA.php?tag=beach')
  .then(response => {
    const endTime = performance.now();
    console.log(`通信時間: ${endTime - startTime}ms`);
  });
```

### アプリケーション層（Webサーバ）

アプリケーション層は、Webサーバ上でビジネスロジックを実行する中核的な処理層です。PHP等のスクリプトによるデータ処理、ファイルI/O、検索・ソート処理などが行われ、アプリケーション全体の性能を大きく左右します。Apache設定、PHP実行時間、メモリ使用量の最適化により、大幅な性能向上が期待できる層でもあります。

#### Apache設定の影響
```apache
# プロセス数の確認
ps aux | grep apache

# 接続数の確認
netstat -an | grep :80 | wc -l

# エラーログの確認
tail -f /var/log/apache2/error.log
```

#### PHP実行時間の測定

**基本的な実行時間測定**

関数名などはサンプルなので各自で置き換えてください．また，言語はPHPに限りません．
```php
<?php
// PHP実行時間測定
$start_time = microtime(true);

// 検索処理
$results = searchPhotos($_REQUEST['tag']);

$end_time = microtime(true);
$execution_time = ($end_time - $start_time) * 1000; // ミリ秒
error_log("実行時間: {$execution_time}ms");
?>
```

**詳細なプロファイリング**
```php
<?php
// 処理段階別の実行時間測定
$profile = [];
$profile['start'] = microtime(true);

// ファイル読み込み時間
$profile['file_read_start'] = microtime(true);
$data = file_get_contents('/data/geotag.csv');
$profile['file_read_end'] = microtime(true);

// 検索処理時間
$profile['search_start'] = microtime(true);
$results = performSearch($data, $_REQUEST['tag']);
$profile['search_end'] = microtime(true);

// ソート処理時間
$profile['sort_start'] = microtime(true);
usort($results, 'sortByDate');
$profile['sort_end'] = microtime(true);

$profile['end'] = microtime(true);

// 結果出力
$file_read_time = ($profile['file_read_end'] - $profile['file_read_start']) * 1000;
$search_time = ($profile['search_end'] - $profile['search_start']) * 1000;
$sort_time = ($profile['sort_end'] - $profile['sort_start']) * 1000;
$total_time = ($profile['end'] - $profile['start']) * 1000;

error_log("ファイル読み込み: {$file_read_time}ms");
error_log("検索処理: {$search_time}ms");
error_log("ソート処理: {$sort_time}ms");
error_log("合計実行時間: {$total_time}ms");
?>
```

### データ層（データベース）

データ層は、データの永続化とクエリ処理を担当する基盤層です。大量のデータから必要な情報を効率的に検索・取得する責任があり、インデックスの有無やクエリの最適化によって性能が大きく変動します。特にフルテーブルスキャンやN+1問題などは、アプリケーション全体のボトルネックとなりやすいため、綿密な分析が必要です。なお、本最終課題は主にCSVファイルの検索が課題となっていますので、データベースの改善については対象外（任意）となっています。

#### クエリ実行時間の測定
```sql
-- クエリ実行時間の有効化
SET profiling = 1;

-- 検索クエリの実行
SELECT * FROM tag WHERE tag LIKE '%beach%';

-- 実行時間の確認
SHOW PROFILES;

-- 詳細なプロファイル情報
SHOW PROFILE FOR QUERY 1;
```

#### インデックス使用状況の確認
```sql
-- インデックス使用状況の確認
EXPLAIN SELECT * FROM tag WHERE tag = 'beach';

-- インデックス効果の比較
EXPLAIN SELECT * FROM tag IGNORE INDEX(i_tag) WHERE tag = 'beach';
```

### システムリソース監視

**注意**: `iostat`コマンドがない場合は、以下のコマンドでインストールしてください：
```bash
sudo apt update
sudo apt install sysstat
```

#### CPU使用率の監視
```bash
# リアルタイム監視
top

# Apache特定プロセスの確認
ps aux | grep apache2

# 統計情報の取得
iostat -c 1 10
```

#### メモリ使用量の監視
```bash
# メモリ使用状況
free -h

# プロセス別メモリ使用量
ps aux --sort=-%mem | head -10
```

#### ディスクI/O監視
```bash
# ディスクI/O統計
iostat -x 1 10

# プロセス別I/O統計（iotopがある場合）
iotop -o
```

## ボトルネック判別手法

### 制約型の特定

収集したデータから、システムの制約がCPU、メモリ、I/Oのどこにあるかを判別します。

#### CPU制約型の判別
```bash
# CPU使用率が高い（80%以上）状態を確認
top
iostat -c 1 5

# 判別基準：
# - CPU使用率: 80%以上継続
# - Load Average: CPUコア数を上回る
# - iowait: 低い（10%以下）
```

**特徴:**
- 計算集約的な処理（検索、ソート、文字列操作）
- CPUコア数を増やすか、アルゴリズム最適化が有効

#### メモリ制約型の判別
```bash
# メモリ使用状況とスワップの確認
free -h
cat /proc/meminfo | grep -E '(MemAvailable|SwapTotal|SwapFree)'

# 判別基準：
# - 利用可能メモリ: 物理メモリの10%以下
# - スワップ使用: 発生している
# - ページフォルト: 頻発
```

**特徴:**
- 大容量データの処理時にパフォーマンス急激な低下
- スワップ発生によるディスクI/O増加
- メモリ増設または効率的なデータ構造が必要

#### I/O制約型の判別
```bash
# ディスクI/O状況の確認
iostat -x 1 5

# 判別基準：
# - %util: 80%以上
# - await: 応答時間が長い（10ms以上）
# - iowait: CPUのI/O待機時間が高い（20%以上）
```

**特徴:**
- ファイル読み込み、データベースアクセスでの遅延
- キャッシュ、インデックス、SSD化が有効

### 制約型別の最適化戦略

| 制約型 | 主な症状 | 最適化手法 |
|--------|----------|------------|
| CPU制約型 | CPU使用率高、計算処理遅延 | アルゴリズム最適化、並列処理、キャッシュ |
| メモリ制約型 | スワップ発生、メモリ不足 | データ構造効率化、ストリーミング処理 |
| I/O制約型 | ディスクアクセス遅延 | インデックス、キャッシュ、SSD化 |

## 分析結果の可視化

### 性能データのグラフ化

測定データを可視化することで、ボトルネックや改善効果を明確に示すことができます。

#### 時系列グラフ
- 実装A/B/Cの性能推移
- 最適化前後の比較

#### 箱ひげ図
- 複数回測定結果のばらつき
- 異常値の検出

#### 積み上げグラフ
- 処理時間の内訳
- 各段階の処理時間

## 分析時の注意点

### 1. キャッシュ効果の考慮
```bash
# MySQLキャッシュのクリア
mysql> RESET QUERY CACHE;

# ファイルシステムキャッシュのクリア
sudo sync && sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'
```

### 2. 負荷状況の統一
- CPU温度による性能変動
- 他プロセスの影響
- ネットワーク状況

### 3. 測定の再現性
- 同一条件での複数回測定
- 統計的な有意性の確認
- 環境要因の記録

## 次のステップ

ボトルネック分析手法を習得したら、次は具体的な実装パターンごとの性能特性を理解しましょう。以降では、実装Aのアーキテクチャ特性と主要なボトルネックを詳しく分析し、改善方法の案について解説していきます．

各実装の特性を理解することで、ここで学んだ分析手法をより効果的に活用できるようになります。