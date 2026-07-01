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
#include <array>
#include <random>

using namespace std;

#define ll long long
#define LL __int128
#define ld long double
#define INF 2251799813685248
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define reps(i, l, r) for(ll i = (l); i < (r); ++i)
#define foreach(c, A) for(auto c:(A))
#define vall(A) (A).begin(),(A).end()
#define vrall(A) (A).rbegin(),(A).rend()
#define slice(A, l, r) next((A).begin(), (l)), next((A).begin(), (r))
#define vin(A) for (ll iiii = 0, szszszsz = (A).size(); iiii < szszszsz; iiii++){cin >> (A)[iiii];}
#define vout(A) for (ll iiii = 0, szszszsz = (A).size(); iiii < szszszsz; iiii++){cout << (A)[iiii] << " \n"[iiii == szszszsz-1];}
#define vin2d(A) for (ll iiii = 0; iiii < (A).size(); iiii++){for (ll jjjj = 0; jjjj < (A)[iiii].size(); jjjj++){cin >> (A)[iiii][jjjj];}}
#define vout2d(A) for (ll iiii = 0; iiii < (A).size(); iiii++){for (ll jjjj = 0; jjjj < (A)[iiii].size(); jjjj++){cout << (A)[iiii][jjjj] << " \n"[jjjj==(A)[iiii].size()-1];}}
#define encode(i,j) (((i))<<32)+(j)
#define decode(v,w) ((w) ? (v)%4294967296 : (v)>>32)
#define vinc(A) for (auto &vvvv : (A)){vvvv++;}
#define vdec(A) for (auto &vvvv : (A)){vvvv--;}
#define graphin0(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; (C)[aaaa].push_back(bbbb); (C)[bbbb].push_back(aaaa);}
#define graphin1(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; (C)[aaaa-1].push_back(bbbb-1); (C)[bbbb-1].push_back(aaaa-1);}
#define lsegtype(name) name::S, name::F
#define lsegarg(name) name::op, name::e,name::comp, name::mapping, name::id

vector<ll> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904};
vector<ll> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000};
vector<ll> di{0,1,0,-1};
vector<ll> dj{1,0,-1,0};

template <typename T>
bool chmax(T &a, const T& b) { return a < b ? a = b, true : false; }
template <typename T>
bool chmin(T &a, const T& b) { return a > b ? a = b, true : false; }

unsigned int bit_length(ll n){ return n > 0 ? 64 - __builtin_clzll(n) : 0;}

template <typename T>
T sum(vector<T> A){
    T res = 0;
    for (size_t i=0;i<A.size();i++){
        res += A[i];
    }
    return res;
}

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

// ===========================================================================================================================


template <typename T>
struct SegmentTree{
    int _size;
    int N;
    vector<T> array;
    function<T(T,T)> op;
    function<T()> e;

    SegmentTree() = default;
    SegmentTree(vector<T> _array,function<T(T,T)> _op, function<T()> _e){
        this->_size = _array.size();
        this->op = _op;
        this->e = _e;
        this->N = 1;
        while (this->N < this->_size) this->N <<= 1;
        this->array = vector<T>(2 * this->N, this->e());
        for (int i = 0;i<_size;i++){
            this->array[i+N] = _array[i];
        }
        for (int i = N - 1; i > 0; i--){
            this->array[i] = _op(this->array[2*i],this->array[2*i+1]);
        }
    }

    struct NodeRef {
        SegmentTree<T>* st;
        int idx;

        NodeRef(SegmentTree<T>* st, int idx) : st(st), idx(idx) {}

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

    T get(int i){
        return array[this->N + i];
    }

    void write(int i, T value){
        assert(0 <= i && i < _size);
        int ii = i + N;
        array[ii] = value;
        while (ii > 1){
            array[ii>>1] = op(array[ii&(-2)],array[ii|1]);
            ii >>= 1;
        }
    }

    void inplace_op(int i, T value){
        write(i, op(get(i) ,value));
    }

    T prod(int l,int r){
        assert(0 <= l && l <= r && r <= _size);

        T resL = e();
        T resR = e();
        int li = l + N;
        int ri = r + N;
        while (li < ri){
            if (li&1){
                resL = op(resL, array[li]);
                li += 1;
            }
            if (ri&1){
                ri -= 1;
                resR = op(array[ri], resR);
            }
            li >>= 1;
            ri >>= 1;
        }
        return op(resL, resR);
    }

    size_t size(){
        return this->_size;
    }
};

struct RollingHashEngine{
    using ulong = unsigned long long;

    // 必要な定数の定義
    const ulong root = 37; // (2^61-1の原始根)
    const ulong MOD = (1ULL << 61) - 1;
    const ulong base;


    // 必要な関数の定義
    // mod 2^61-1 系関数 (参考：https://qiita.com/keymoon/items/11fac5627672a6d6a9f6)
    //a*b mod 2^61-1を返す関数(最後にModを取る)
    inline ulong Mul(ulong a, ulong b)
    {
        ulong au = a >> 31;
        ulong ad = a & ((1ULL << 31) - 1);
        ulong bu = b >> 31;
        ulong bd = b & ((1ULL << 31) - 1);
        ulong mid = ad * bu + au * bd;
        ulong midu = mid >> 30;
        ulong midd = mid & ((1ULL << 30) - 1);
        return CalcMod(au * bu * 2 + midu + (midd << 31) + ad * bd);
    }

    //mod 2^61-1を計算する関数
    inline ulong CalcMod(ulong x)
    {
        ulong xu = x >> 61;
        ulong xd = x & ((1ULL << 61) - 1);
        ulong res = xu + xd;
        if (res >= ((1ULL << 61) - 1)) res -= ((1ULL << 61) - 1);
        return res;
    }

    // mod 2^61-1のべき乗を計算する関数
    inline ulong Pow(ulong a, ulong n){
        ulong res = 1;
        while (n > 0){
            if (n & 1){
                res = Mul(res, a);
            }
            a = Mul(a, a);
            n >>= 1;
        }
        return res;
    }

    // 基数を生成する関数 (参考：https://trap.jp/post/1036/)
    ulong get_base(ulong bound){
        // 乱数生成機の準備
        random_device rd;
        mt19937 engine(rd());
        uniform_int_distribution<ulong> dist(1ULL, ((1ULL << 61) - 1) - 2);

        while (true){
            ulong k = dist(engine);
            if (gcd(k, ((1ULL << 61) - 1) - 1) == 1){
                ulong b = Pow(root, k);
                if (b > bound){
                    return b;
                }
            }
        }
    }

    // コンストラクタ
    RollingHashEngine(): base(get_base(1ULL << 30)){}
    RollingHashEngine(ulong _bound): base(get_base(_bound)){}

};

// {a,b} → {a^-1 mod b,gcd(a,b)}
template <typename T>
pair<T,T> exgcd(T a, T b){
    T xs = 1, ys = 0, xt = 0, yt = 1, tmp;
    while (b != 0){
        tmp = a/b;
        a = a%b;
        xs -= tmp*xt;
        ys -= tmp*yt;
        swap(xs,xt);
        swap(ys,yt);
        swap(a,b);
    }
    return {xs,a};
}

template<int MOD>
struct static_modint
{
    int _val;

    static_modint() : _val(0){};
    static_modint(int __val) : _val(__val % MOD){if (_val < 0){_val += MOD;}};
    static_modint(ll __val) : _val(__val % (ll)MOD){if (_val < 0){_val += MOD;}}; 

    static_modint operator+(static_modint other) const {
        int tmp = _val + other._val;
        if (tmp >= MOD){
            tmp -= MOD;
        }
        return static_modint(tmp);
    }

    static_modint operator-(static_modint other) const {
        int tmp = _val - other._val;
        if (tmp < 0){
            tmp += MOD;
        }
        return static_modint(tmp);
    }

    static_modint operator*(static_modint other) const {
        return static_modint((ll)_val * (ll)other._val);
    }

    static_modint operator/(static_modint other) const {
        return static_modint(*this * other.inv());
    }

    static_modint inv() const {
        auto [x, g] = exgcd(_val, MOD);
        if (g != 1){
            throw invalid_argument("Inverse does not exist.");
        }
        return static_modint(x);
    }

    static_modint pow(ll n) const {
        ll res = 1;
        ll x = (ll)_val;
        while (n > 0){
            if (n & 1){
                res = (ll)(((__int128)res * x) % MOD);
            }
            x = (ll)(((__int128)x * x) % MOD);
            n >>= 1;
        }
        return static_modint(res);
    }

    static_modint operator++(){
        _val += 1;
        if (_val == MOD){
            _val = 0;
        }
        return *this;
    }

    static_modint operator--(){
        if (_val == 0){
            _val = MOD;
        }
        _val -= 1;
        return *this;
    }

    static_modint operator++( int ){
        static_modint tmp = *this;
        ++*this;
        return tmp;
    }

    static_modint operator--( int ){
        static_modint tmp = *this;
        --*this;
        return tmp;
    }

    static_modint& operator+=(static_modint other){
        _val += other._val;
        if (_val >= MOD){
            _val -= MOD;
        }
        return *this;
    }

    static_modint& operator-=(static_modint other){
        _val -= other._val;
        if (_val < 0){
            _val += MOD;
        }
        return *this;
    }

    static_modint& operator*=(static_modint other){
        _val = (ll)_val * (ll)other._val % MOD;
        return *this;
    }

    static_modint& operator/=(static_modint other){
        return *this *= other.inv();
    }

    static_modint operator-() const {
        if (_val == 0){
            return static_modint(0);
        } else {
            return static_modint(MOD - _val);
        }
    }

    static_modint operator+() const {
        return *this;
    }


    bool operator==(const static_modint& other) const{return _val == other._val;}
    bool operator!=(const static_modint& other) const{return _val != other._val;}
    bool operator<(const static_modint& other) const{return _val < other._val;}
    bool operator>(const static_modint& other) const{return _val > other._val;}
    bool operator<=(const static_modint& other) const{return _val <= other._val;}
    bool operator>=(const static_modint& other) const{return _val >= other._val;}

    operator int() const { return _val; }

    int val() const { return _val; }

    
};

using mint = static_modint<998244353>;
istream &operator>>(istream &is, mint &i){long long t; is >> t; i = t; return is; }
ostream &operator<<(ostream &os, const mint &i){ os << i.val(); return os;}


struct RangeParallelUnionFind{
    using pmm = pair<mint, mint>;
    using ulong = unsigned long long;
    using puu = pair<ulong, ulong>;
    using u128 = unsigned __int128;

    vector<int> parent;
    SegmentTree<puu> label;
    vector<int> next_v;
    vector<pmm> data; // {sum, square_sum}
    mint global_value;
    ulong base;
    ulong mod;
    ulong salt = 100;

    RangeParallelUnionFind(int n, vector<mint> v): parent(n,-1), data(n), global_value(0), next_v(n){
        RollingHashEngine rhe;
        base = rhe.base;
        mod = rhe.MOD;
        vector<puu> init_label(n);
        for(int i=0; i<n; i++){
            data[i] = {v[i], v[i] * v[i]};
            init_label[i] = {i + salt, base};
            next_v[i] = i;
        }

        auto op = [&](puu a, puu b) -> puu {
            auto [ah, ax] = a;
            auto [bh, bx] = b;
            u128 tmp = (u128)ah + (u128)bh * (u128)ax;
            ulong nh = (ulong)(tmp >> 61) + (ulong)(tmp & mod);
            if (nh >= mod){nh -= mod;}
            u128 tmp2 = (u128)ax * (u128)bx;
            ulong nx = (ulong)(tmp2 >> 61) + (ulong)(tmp2 & mod);
            if (nx >= mod){nx -= mod;}
            return {nh, nx};
        };

        label = SegmentTree<puu>(init_label, op, [](){return puu{0,1};});

    }

    int root(int x){
        int tmp = x;
        while (parent[tmp] >= 0){
            tmp = parent[tmp];
        }
        while (x != tmp){
            int next = parent[x];
            parent[x] = tmp;
            x = next;
        }
        return tmp;
    }

    bool merge(int x, int y){
        int xr = root(x);
        int yr = root(y);
        if (xr == yr){
            return false;
        }
        if (parent[xr] > parent[yr]){
            swap(xr,yr);
        }

        global_value += (((data[xr].first+data[yr].first)*(data[xr].first+data[yr].first) - (data[xr].second+data[yr].second)) - (data[xr].first * data[xr].first - data[xr].second) - (data[yr].first * data[yr].first - data[yr].second))/mint(2);
        
        data[xr].first += data[yr].first;
        data[xr].second += data[yr].second;
        data[yr] = {0,0};

        for(auto i: component(yr)){
            label.write(i,{xr + salt, base});
        }

        swap(next_v[xr], next_v[yr]);


        parent[xr] += parent[yr];
        parent[yr] = xr;
        return true;
    }

    bool same(int x, int y){
        return root(x) == root(y);
    }

    bool size(int x){
        return -parent[root(x)];
    }

    vector<int> component(int v){
        int tmp = v;
        vector<int> ans;
        ans.push_back(v);
        while (next_v[tmp] != v){
            tmp = next_v[tmp];
            ans.push_back(tmp);
        }
        return ans;
    }

    mint get_global_value(){
        return global_value;
    }

    void query(int k, int l, int r){
        int pre = 0;
        while(label.prod(l,l+k).first != label.prod(r,r+k).first){
            int ok = pre;
            int ng = k;
            while (ng - ok > 1){
                int mid = (ok + ng) / 2;
                if (label.prod(l,l+mid).first == label.prod(r,r+mid).first){
                    ok = mid;
                } else {
                    ng = mid;
                }
            }
            merge(l+ok, r+ok);
            pre = ok;
        }
    }
};





// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N,Q;
    cin >> N >> Q;

    vector<mint> v(N);
    vin(v);

    RangeParallelUnionFind UF(N, v);

    rep(_, Q){
        int k,l,r;
        cin >> k >> l >> r;
        UF.query(k,l,r);
        cout << UF.get_global_value() << "\n";
    }
    
}