# cpp

このフォルダ直下の C++ ライブラリを、関数名と意味だけで短く整理した README。

## 共通

- ほとんどの実装は 0-index 前提。
- 区間系は半開区間 [l, r) を使うものが多い。

## -template.cpp

競プロ用テンプレート。

- 汎用入力補助: 標準入力を扱うための補助群。
- 数論系ヘルパー: 素数判定やエラトステネス系の補助。
- modint 系: 典型的な modint の雛形。

## BinaryTrie.cpp

0/1 二分トライ。

- `insert(x)`: 要素を追加する。
- `erase(x)`: 要素を 1 個削除する。
- `count(x)`: `x` の個数を返す。
- `kth(k)`: 小さい方から `k` 番目の値を返す。
- `min()`: 最小値を返す。
- `max()`: 最大値を返す。
- `lower_bound(x)`: `x` 以上の最小値を返す。
- `upper_bound(x)`: `x` より大きい最小値を返す。
- `all_xor(v)`: 全要素に一括 XOR をかける。

## LazySegmentTree.cpp

汎用遅延セグ木。

- `set(k, x)`: 1 点を更新する。
- `get(k)`: 1 点を取得する。
- `prod(l, r)`: 区間 `[l, r)` の総積を返す。
- `apply(l, r, f)`: 区間 `[l, r)` に遅延作用を適用する。
- `all_prod()`: 全体の総積を返す。

## LinearSieve.cpp

線形篩。

- `build_lpf(n)`: `0..n` の最小素因数を作る。
- `build_mpf(n)`: `0..n` の最大素因数を作る。
- `factorize(x)`: 線形篩の結果を使って素因数分解する。

## PartiallyPersistentUnionFind.cpp

部分永続 Union-Find。

- `merge(x, y)`: 現在時点で 2 要素を併合する。
- `root(x, t)`: 時刻 `t` における代表元を返す。
- `same(x, y, t)`: 時刻 `t` に同一連結成分か判定する。
- `size(x, t)`: 時刻 `t` における成分サイズを返す。
- `connect_time(x, y)`: 2 要素が初めてつながる時刻を返す。

## RollingHash.cpp

2^61-1 ローリングハッシュ。

- `build(s)`: 文字列からハッシュ配列を作る。
- `get(l, r)`: 部分文字列 `[l, r)` のハッシュを返す。
- `concat(a, b)`: 2 つのハッシュを連結する。
- `lcp(i, j)`: 2 位置の最長共通接頭辞長を返す。

## SegmentTree.cpp

反復セグ木。

- `set(k, x)`: 1 点を更新する。
- `get(k)`: 1 点を取得する。
- `prod(l, r)`: 区間 `[l, r)` の総積を返す。
- `all_prod()`: 全体の総積を返す。
- `write(k, x)`: 1 点を代入する。
- `inplace_op(k, f)`: 1 点に対して演算を適用する。

## UnionFind.cpp

連結成分列挙付き DSU。

- `merge(x, y)`: 2 要素を併合する。
- `find(x)`: 代表元を返す。
- `same(x, y)`: 同一成分か判定する。
- `size(x)`: 成分サイズを返す。
- `groups()`: 各連結成分の要素列を返す。
- `component(x)`: `x` を含む成分の要素列を返す。

## WaveletMatrix.cpp

Wavelet Matrix。

- `access(k)`: 0-index の `k` 番目の値を返す。
- `rank(x, r)`: `[.., r)` にある `x` の個数を返す。
- `select(x, k)`: `x` の `k` 番目の出現位置を返す。
- `kth(l, r, k)`: 区間 `[l, r)` の `k` 番目の値を返す。
- `freq(l, r, x)`: 区間 `[l, r)` の `x` の個数を返す。
- `range_freq(l, r, a, b)`: 区間 `[l, r)` で値が `[a, b)` に入る個数を返す。

## cipolla.cpp

mod 平方根。

- `sqrt_mod(a, p)`: `a` の mod `p` における平方根を返す。
- `is_quadratic_residue(a, p)`: `a` が平方剰余か判定する。

## メモ

- ほとんどの構造は 0-index 前提。
- 区間系は半開区間 [l, r) を使う実装が多い。