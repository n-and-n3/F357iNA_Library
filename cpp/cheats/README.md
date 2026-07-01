# cheats

コンテスト中に必要になったときだけ使う、高機能寄りの C++ ライブラリをまとめる。
以下の使用例は `using namespace std;` 前提。

## fast_factorization
Miller-Rabin + Pollard-Rho(Brent) による、64bit 整数の高速素数判定・素因数分解・約数列挙。

### 使用方法
1. `fast_factorization.hpp` の中身をメイン関数の上にコピーする。
2. `using namespace fast_factorization;` を書く。

### 関数一覧

#### fast_isprime(n)
- 入力: `long long n` (`0 <= n <= LLONG_MAX`)
- 出力: `n` が素数なら `true`、そうでなければ `false`。
- 64bit 範囲で決定的 Miller-Rabin。

使用例
```cpp
if (fast_isprime(x)) {
    // x is prime
}
```

#### fast_factorize(n)
- 入力: `long long n` (`1 <= n <= LLONG_MAX`)
- 出力: `vector<pair<long long, long long>>`
- 形式: `{素因数, 指数}` の組。素因数の昇順。
- `n <= 1` なら空 vector。

使用例
```cpp
auto f = fast_factorize(360);
// {{2, 3}, {3, 2}, {5, 1}}
```

#### fast_divisor(n)
- 入力: `long long n`
- 出力: `n` の正の約数を `vector<long long>` で昇順に返す。
- `n <= 0` なら空 vector。

使用例
```cpp
auto ds = fast_divisor(12);
// {1, 2, 3, 4, 6, 12}
```

#### fast_divisor(factorization)
- 入力: `fast_factorize(n)` の返り値。
- 出力: 正の約数を昇順に返す。

使用例
```cpp
auto f = fast_factorize(n);
auto ds = fast_divisor(f);
```

### メモ
- 実行時の SPF / 素数表の前計算なし。
- `set_seed(seed)` で Pollard-Rho の乱数を固定できる。
- 基本的な対象は正の `long long`。

---

## indexed_avl_set / indexed_avl_multiset / indexed_avl_map
AVL 木に部分木サイズを持たせた、インデックスアクセス可能な `set` / `multiset` / `map`。

### 使用方法
1. `indexed_avl_set.hpp` の中身をメイン関数の上にコピーする。
2. `indexed_map` も使うなら `indexed_avl_map.hpp` もコピーする。
3. `using namespace avl;` を書く。

```cpp
indexed_set<int> s;
indexed_multiset<int> ms;
indexed_map<int, long long> mp;
```

### 共通の主要機能
標準の `set` / `multiset` / `map` に近い操作が使える。

```cpp
insert, emplace, erase, find, count, contains
lower_bound, upper_bound, equal_range
begin, end, size, empty, clear
```

イテレータは双方向イテレータ。`std::distance(it1, it2)` は区間長に線形。
順位差を速く欲しいときは `index_of` を使う。

### インデックス系 共通
ソート順で 0-indexed。

#### nth(k) / find_by_order(k)
- `k` 番目のイテレータを返す。
- 範囲外なら `end()`。
- 計算量: `O(log N)`

```cpp
auto it = s.nth(2);
if (it != s.end()) cout << *it << '\n';
```

#### order_of_key(x)
- `x` 未満の要素数を返す。
- 計算量: `O(log N)`

```cpp
long long cnt = s.order_of_key(x);
```

#### index_of(it)
- イテレータの順位を返す。
- `end()` なら `size()`。
- 計算量: `O(log N)`

```cpp
auto it = s.find(x);
if (it != s.end()) cout << s.index_of(it) << '\n';
```

### indexed_set
重複なし。

```cpp
indexed_set<int> s;
s.insert(10);
s.insert(20);
s.insert(10);       // 増えない

cout << s.at(0);    // 範囲外なら例外
cout << s[1];       // 範囲外は未定義
```

### indexed_multiset
重複あり。

```cpp
indexed_multiset<int> ms;
ms.insert(5);
ms.insert(5);
ms.insert(7);

cout << ms.count(5);  // 2
```

#### erase(key)
標準 `multiset` 互換。`key` と等しい要素をすべて消す。

```cpp
ms.erase(5); // 5 を全部消す
```

#### erase1(key)
`key` と等しい要素を 1 個だけ消す。消したら `1`、なければ `0`。

```cpp
ms.erase1(5);
```

### indexed_map
`std::map` 相当。キー重複なし。`multimap` はなし。

```cpp
indexed_map<int, long long> mp;
mp[10] = 100;
mp.insert({20, 200});
```

#### at_index(k)
`k` 番目の `{key, value}` への参照。範囲外なら例外。

```cpp
auto [key, val] = mp.at_index(0); // コピー
auto& p = mp.at_index(0);         // 参照
```

#### key_at_index(k) / mapped_at_index(k)
`k` 番目の key / value にアクセスする。

```cpp
cout << mp.key_at_index(0) << '\n';
mp.mapped_at_index(0) += 1;
```

#### index_distance(first, last)
`index_of(last) - index_of(first)` を返す。`std::distance` より順位差を速く取れる。

```cpp
auto l = mp.lower_bound(L);
auto r = mp.lower_bound(R);
cout << mp.index_distance(l, r) << '\n';
```