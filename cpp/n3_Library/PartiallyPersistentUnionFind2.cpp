#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <cassert>
#include <bit>
#include <atcoder/all>

using namespace std;
using namespace atcoder;
using mint = modint998244353;

#define ll long long
#define LL __int128
#define MOD 998244353
#define ld long double
#define INF 2251799813685248
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define reps(i, l, r) for(ll i = (l); i < (r); ++i)
#define foreach(c, A) for(auto c:(A))
#define vall(A) A.begin(),A.end()
#define vrall(A) A.rbegin(),A.rend()
#define slice(A, l, r) next((A).begin(), (l)), next((A).begin(), (r))
#define vin(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cin >> A[iiii];}
#define vout(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cout << A[iiii] << " \n"[iiii == szszszsz-1];}
#define vin2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cin >> A[iiii][jjjj];}}
#define vout2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cout << A[iiii][jjjj] << " \n"[jjjj==A[iiii].size()-1];}}
#define encode(i,j) ((i)<<32)+j
#define decode(v,w) (w ? (v)%4294967296 : (v)>>32)
#define vinc(A) for (auto &vvvv : A){vvvv++;}
#define vdec(A) for (auto &vvvv : A){vvvv--;}
#define graphin0(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; C[aaaa].push_back(bbbb); C[bbbb].push_back(aaaa);}
#define graphin1(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; C[aaaa-1].push_back(bbbb-1); C[bbbb-1].push_back(aaaa-1);}

vector<ll> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904};
vector<ll> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000};
vector<ll> di{0,1,0,-1};
vector<ll> dj{1,0,-1,0};

istream &operator>>(istream &is, mint &i){long long t; is >> t; i = t; return is; }
ostream &operator<<(ostream &os, const mint &i){ os << i.val(); return os;}

template <typename T>
bool chmax(T &a, const T& b) { return a < b ? a = b, true : false; }
template <typename T>
bool chmin(T &a, const T& b) { return a > b ? a = b, true : false; }

unsigned int bit_length(ll n){ return n > 0 ? 64 - __builtin_clzll(n) : 0;}

ll powll(ll a, ll n, ll m){
    if (n == 0){return 1;}
    if (n == 1){return a % m;}
    LL ans = 1;
    LL p = a;
    while(n > 0){
        if ((n & 1) == 1){
            ans *= p;
            ans %= m;
        }
        n >>= 1;
        p *= p;
        p %= m;
    }
    return (ll)ans;
}

//===================================================================================

template <typename T>
struct PartiallyPersistentArray {
    struct Data{
        vector<vector<pair<int,T>>> historys;
        Data(vector<T> array){
            int N = array.size();
            historys.resize(N); 
            for (int i=0; i<N; i++){
                this->historys[i] = {{0,array[i]},};
            }
        }


        T get(int v, int ind){
            int ok=0;
            int ng=historys[ind].size();
            int mid;
            while (ng - ok > 1){
                mid = (ok + ng)/2;
                if (this->historys[ind][mid].first <= v){
                    ok = mid;
                } else {
                    ng = mid;
                }
            }
            return this->historys[ind][ok].second;
        }

        void write(int v, int ind, T x){
            assert(this->historys[ind].back().first < v); // 最新版より大きい数値でないと、バージョンとして認めない。要するに二分探索がバグらないように単調性を要請する
            if (this->historys[ind].back().second != x){
                this->historys[ind].push_back({v,x});
            }
        }

        T current(int ind){
            return this->historys[ind].back().second;
        }
    };

    struct NodeRef {
        PartiallyPersistentArray<T>* st;
        int idx;

        NodeRef(PartiallyPersistentArray<T>* st, int idx) : st(st), idx(idx) {}

        NodeRef& operator=(const T& val) {st->write(idx, val); return *this;}
        NodeRef& operator=(const NodeRef& other) {st->write(idx, (T)other);return *this;}
        
        #define DEFINE_COMPOUND_OP(OP, BI_OP) \
        NodeRef& operator OP (const T& val) {st->write(idx, st->get(idx) BI_OP val); return *this;} \
        NodeRef& operator OP (const NodeRef& other) {st->write(idx, st->get(idx) BI_OP (T)other); return *this;} 

        DEFINE_COMPOUND_OP(+=, +)
        DEFINE_COMPOUND_OP(-=, -)
        DEFINE_COMPOUND_OP(*=, *)
        DEFINE_COMPOUND_OP(/=, /)
        DEFINE_COMPOUND_OP(%=, %)
        DEFINE_COMPOUND_OP(|=, |)
        DEFINE_COMPOUND_OP(&=, &)
        DEFINE_COMPOUND_OP(^=, ^)
        DEFINE_COMPOUND_OP(<<=, <<)
        DEFINE_COMPOUND_OP(>>=, >>)

        #undef DEFINE_COMPOUND_OP
        
        NodeRef& operator++() {st->write(idx, st->get(idx) + 1);return *this;}
        T operator++(int) {T old = st->get(idx);st->write(idx, old + 1);return old;}
        NodeRef& operator--() {st->write(idx, st->get(idx) - 1);return *this;}
        T operator--(int) {T old = st->get(idx);st->write(idx, old - 1);return old;}

        operator T() const {return st->get(idx);}
    };

    NodeRef operator[](int i) {
        return NodeRef(this, i);
    }

// --- 内部構造体 iterator の定義 ---
    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = NodeRef; // 代入を可能にするため NodeRef を返す

        PartiallyPersistentArray<T>* ppa;
        int idx;

        iterator(PartiallyPersistentArray<T>* ppa, int idx) : ppa(ppa), idx(idx) {}

        // 間接参照
        NodeRef operator*() const { return (*ppa)[idx]; }
        // ※ポインタアクセス(->)は T が構造体の場合に複雑になるため、必要に応じて定義
        
        // 算術演算
        iterator& operator++() { idx++; return *this; }
        iterator operator++(int) { iterator temp = *this; idx++; return temp; }
        iterator& operator--() { idx--; return *this; }
        iterator operator--(int) { iterator temp = *this; idx--; return temp; }
        iterator& operator+=(difference_type n) { idx += n; return *this; }
        iterator& operator-=(difference_type n) { idx -= n; return *this; }
        iterator operator+(difference_type n) const { return iterator(ppa, idx + n); }
        iterator operator-(difference_type n) const { return iterator(ppa, idx - n); }
        difference_type operator-(const iterator& other) const { return idx - other.idx; }

        // 比較演算
        bool operator==(const iterator& other) const { return idx == other.idx; }
        bool operator!=(const iterator& other) const { return idx != other.idx; }
        bool operator<(const iterator& other) const { return idx < other.idx; }
        bool operator<=(const iterator& other) const { return idx <= other.idx; }
        bool operator>(const iterator& other) const { return idx > other.idx; }
        bool operator>=(const iterator& other) const { return idx >= other.idx; }

        // インデックスアクセス
        NodeRef operator[](difference_type n) const { return (*ppa)[idx + n]; }
    };

    // --- begin / end メソッド ---
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, sz); }


    Data* data;
    unsigned int version;
    bool is_original;
    size_t sz;

    // 通常定義用 1
    PartiallyPersistentArray<T>(vector<T> array){
        Data* new_data = new Data{array};
        this->data = new_data;
        this->is_original = true;
        this->version = 0;
        this->sz = this->data->historys.size();
    }

    // 通常定義用 2
    PartiallyPersistentArray<T>(int N){
        Data* new_data = new Data{vector<T>(N)};
        this->data = new_data;
        this->is_original = true;
        this->version = 0;
        this->sz = this->data->historys.size();
    }

    // 通常定義用 3
    PartiallyPersistentArray<T>(int N, T initial){
        Data* new_data = new Data{vector<T>(N,initial)};
        this->data = new_data;
        this->is_original = true;
        this->version = 0;
        this->sz = this->data->historys.size();
    }

    // リスト初期化用
    PartiallyPersistentArray<T>(initializer_list<T> init): PartiallyPersistentArray<T>(vector<T>(init)) {}



    T get(int ind){
        assert(0 <= ind && ind < sz);
        if (this->is_original){
            return data->current(ind);
        } else {
            return data->get(this->version,ind);
        }
    }

    void write(int ind, T x){
        assert(0 <= ind && ind < sz);
        assert(this->is_original);
        this->version++;
        data->write(this->version,ind,x);
    }

    size_t size(){
        return sz;
    }

    PartiallyPersistentArray<T> copy(){
        return PartiallyPersistentArray<T>(this);
    }

    operator vector<T>(){
        vector<T> ans(sz);
        for (int i=0; i<sz;i++){
            ans[i] = get(i);
        }
        return ans;
    }

    // コピー用
    private:
    PartiallyPersistentArray<T>(const PartiallyPersistentArray<T>* ppa){
        this->data = ppa->data;
        this->is_original = false;
        this->version = ppa->version;
        this->sz = ppa->data->historys.size();
    }
};


// ===============================================================================


struct PartiallyPersistentUnionFind{
    PartiallyPersistentArray<int> parent;
    int c;
    bool is_original;

    PartiallyPersistentUnionFind(int n) : parent(n,-1),c(n),is_original(true) {}
    PartiallyPersistentUnionFind(PartiallyPersistentArray<int>& parent, int c) : parent(0), c(c), is_original(false){
        this->parent = parent.copy();
    }


    int root(int x){
        int tmp = x;
        while (parent[tmp] >= 0){
            tmp = parent[tmp];
        }

        if (is_original){
            while (parent[x] >= 0){
                parent[x] = tmp;
                x = parent[x];
            }
        }

        return tmp;
    }

    bool same(int x,int y){
        return root(x) == root(y);
    }

    int size(int x){
        return -parent[root(x)];
    }

    bool merge(int x,int y){
        assert(is_original);
        int xr = root(x),yr = root(y);
        if (xr == yr){
            return false;
        }
        if (parent[xr] <= parent[yr]){
            parent[xr] += parent[yr];
            parent[yr] = xr;
        } else {
            parent[yr] += parent[xr];
            parent[xr] = yr;
        }
        c -= 1;
        return true;
    }

    int group_count(){
        return c;
    }

    PartiallyPersistentUnionFind copy(){
        return PartiallyPersistentUnionFind(parent, c);
    }



    vector<int> label(){
        vector<int> res(parent.size());
        for (int i=0;i<parent.size();i++){
            res[i] = root(i);
        }
        return res;
    }

    vector<int> comped_label(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<int> res(parent.size());
        for (int i = 0; i < parent.size(); i++) {
            res[i] = lower_bound(B.begin(), B.end(), A[i]) - B.begin();
        }
        return res;
    }

    vector<vector<int>> groups(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<vector<int>> res(c);
        for (int i = 0; i < parent.size(); i++) {
            res[lower_bound(B.begin(), B.end(), A[i]) - B.begin()].push_back(i);
        }
        return res;
    }

    vector<int> group_sizes(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<int> res(c,0);
        for (int i = 0; i < parent.size(); i++) {
            res[lower_bound(B.begin(), B.end(), A[i]) - B.begin()] += 1;
        }
        return res;
    }

    void print(){
      cout << "{";
        for (int i = 0; i < parent.size(); i++){
            cout << parent[i] << ", ";
        }
      cout << "}" << "\n";
    }
};


// ==============================================================================


// https://atcoder.jp/contests/agc002/submissions/72462718
int main(){
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N,M;
  cin >> N >> M;
  PartiallyPersistentUnionFind UF(N);

  vector<PartiallyPersistentUnionFind> history;
  history.push_back(UF.copy());
  
  int a,b;
  rep(_,M){
    cin >> a >> b;
    a--;b--;
    UF.merge(a,b);
    history.push_back(UF.copy());
  }

  int Q;
  int x,y,z;
  int ok,ng,mid;
  int sz;


  cin >> Q;
  rep(_,Q){
    cin >> x >> y >> z;
    x--;y--;
    if (UF.same(x,y)){
        ng=0;
        ok=M+1;
        while (ok-ng > 1){
            mid = (ok+ng)/2;
            if (history[mid].same(x,y)){
                sz = history[mid].size(x);
            } else {
                sz = history[mid].size(x) + history[mid].size(y);
            }
            if (sz >= z){
                ok = mid;
            } else {
                ng = mid;
            }
        }
        cout << ok << "\n";
    } else {
        cout << -1 << "\n";
    }
  }

}