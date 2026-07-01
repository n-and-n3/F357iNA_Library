#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using LL = __int128;
using ull = unsigned long long;
using ld = long double;

/*repマクロ*/
#define REP_CAT_INNER(a,b) a##b
#define REP_CAT(a,b) REP_CAT_INNER(a,b)
#define OVERLOAD_REP(a,b,c,d,name,...) name
#define rep(...) OVERLOAD_REP(__VA_ARGS__,REP3,REP2,REP1,REP0)(__VA_ARGS__)
#define REP0(x) for(auto REP_CAT(_rep_c_,__LINE__):std::views::iota(0LL,(long long)(x)))
#define REP1(i,x) for(auto i:std::views::iota(0LL,(long long)(x)))
#define REP2(i,l,r) for(auto i:std::views::iota((long long)(l),(long long)(r)))
#define REP3(i,l,r,c) for(long long i=(long long)(l),REP_CAT(_rep_e_,__LINE__)=(long long)(r),REP_CAT(_rep_s_,__LINE__)=(long long)(c);(REP_CAT(_rep_s_,__LINE__)>0?i<REP_CAT(_rep_e_,__LINE__):i>REP_CAT(_rep_e_,__LINE__));i+=REP_CAT(_rep_s_,__LINE__))
#define vall(A) (A).begin(),(A).end()
#define lsegtype(name) name::S, name::F
#define lsegarg(name) name::op, name::e,name::comp, name::mapping, name::id

vector<ll> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904};
vector<ll> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000};

template <typename T>
bool chmax(T &a, const T& b) { return a < b ? a = b, true : false; }
template <typename T>
bool chmin(T &a, const T& b) { return a > b ? a = b, true : false; }

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

long long isqrt(long long n) {
  if (n <= 0) return 0;
  long long x = sqrt(n);
  while ((x + 1) * (x + 1) <= n) x++;
  while (x * x > n) x--;
  return x;
}

template <typename T>
inline int bit_length(T n) {
    static_assert(std::is_integral_v<T>, "bit_length: unsupported type");
    if (n <= 0) return 0; 
    return std::bit_width(static_cast<std::make_unsigned_t<T>>(n));
}

template <typename T>
inline int bit_count(T n) {
    static_assert(std::is_integral_v<T>, "bit_count: unsupported type");
    return std::popcount(static_cast<std::make_unsigned_t<T>>(n));
}

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
// ===============================================================================

// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=11153823#1

template <typename T>
struct SegmentTree{
    int _size;
    int N;
    vector<T> array;
    function<T(T,T)> op;
    function<T()> e;

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


ll op(ll x,ll y){
  return x+y;
}

ll e(){
  return 0LL;
}

int main(){
    int n,q,t,l,r;
    ll res;

    cin >> n >> q;


    vector<ll> array(n);
    // vin(array);
    
    auto ST = SegmentTree<ll>(array, op, e);

    rep(i,q){
        cin >> t >> l >> r;
        if (t == 0){
            ST[l] += r; 
        } else {
            cout << ST.prod(l, r) << "\n";
        }
    }
}
