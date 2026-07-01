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

using namespace std;

#define ll long long
#define LL __int128
#define MOD 998244353
#define ld long double
#define INF 2251799813685248
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define reps(i, l, r) for(ll i = (l); i < (r); ++i)
#define vall(A) A.begin(),A.end()
#define slice(A, l, r) next((A).begin(), (l)), next((A).begin(), (r))
#define gridinput(vv,H,W) for (ll i = 0; i < H; i++){string T; cin >> T; for(ll j = 0; j < W; j++){vv[i][j] = {T[j]};}}
#define adjustedgridinput(vv,H,W) for (ll i = 1; i <= H; i++){string T; cin >> T; for(ll j = 1; j <= W; j++){vv[i][j] = {T[j-1]};}}
#define vin(A) for (ll i = 0, sz = A.size(); i < sz; i++){cin >> A[i];}
#define vout(A) for(ll i = 0, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}
#define adjustedvin(A) for (ll i = 1, sz = A.size(); i < sz; i++){cin >> A[i];}
#define adjustedvout(A) for(ll i = 1, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}
#define vout2d(A,H,W) for (ll i = 0; i < H; i++){for (ll j = 0; j < W; j++){cout << A[i][j] << " \n"[j==W-1];}}
#define encode(i,j) ((i)<<32)+j
#define decode(v,w) (w ? (v)%4294967296 : (v)>>32)

vector<ll> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296};
vector<ll> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000};
vector<ll> di{0,1,0,-1};
vector<ll> dj{1,0,-1,0};

template <typename T>
bool chmax(T &a, const T& b) { return a < b ? a = b, true : false; }
template <typename T>
bool chmin(T &a, const T& b) { return a > b ? a = b, true : false; }


// ===========================================================================================================================

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

void print(vector<ll> A){
    rep(i,A.size()){
        cout << A[i] << " \n"[i == A.size()-1];
    }
}

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
    vin(array);
    
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
