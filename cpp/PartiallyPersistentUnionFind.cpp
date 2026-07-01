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

// 部分永続配列を使わないバージョン
struct PartiallyPersistentUnionFind{
    vector<int>* parent;
    vector<int>* merge_times;
    bool is_origial;
    int cur_time;
    int infinity = 1 << 30;
    PartiallyPersistentUnionFind* pointer; 

    PartiallyPersistentUnionFind(int n){
        this->parent = new vector<int>(n,-1);
        this->merge_times = new vector<int>(n,this->infinity);
        this->is_origial = true;
        this->cur_time = 0;
    }


    int root(int x){
        while ((*this->merge_times)[x] <= this->cur_time){
            x = (*this->parent)[x];
        }
        return x;
    }

    bool same(int x,int y){
        return root(x) == root(y);
    }

    bool merge(int x,int y){
        assert(this->is_origial);
        this->cur_time++;
        int xr = root(x),yr = root(y);
        if (xr == yr){
            return false;
        }
        if ((*this->parent)[xr] > (*this->parent)[yr]){
            swap(xr,yr);
        }
        (*this->parent)[xr] += (*this->parent)[yr];
        (*this->parent)[yr] = xr;
        (*this->merge_times)[yr] = this->cur_time;
        return true;
    }

    int size(int v){
        assert(this->is_origial);
        return -(*this->parent)[root(v)];
    }

    int connect_time(int x,int y){
        int xr = x;
        int yr = y;
        int t = min((*this->merge_times)[xr],(*this->merge_times)[yr]);

        while (xr != yr && t != infinity){
            if ((*this->merge_times)[xr] > (*this->merge_times)[yr]){
                swap(xr,yr);
            }
            t = (*this->merge_times)[xr];
            while ((*this->merge_times)[xr] <= t && (*this->merge_times)[xr] != infinity){
                xr = (*this->parent)[xr];
            }
        }
        return (t==infinity ? -1 : t);
    }

    PartiallyPersistentUnionFind copy(){
        return PartiallyPersistentUnionFind(this);
    }

    void print(){
      cout << "{";
        for (int i = 0; i < parent->size(); i++){
            cout << (*this->parent)[i] << ", ";
        }
      cout << "}" << "\n";
      cout << "{";
        for (int i = 0; i < merge_times->size(); i++){
            cout << (*this->merge_times)[i] << ", ";
        }
      cout << "}" << "\n";
    }

    PartiallyPersistentUnionFind(PartiallyPersistentUnionFind* ppuf){
        this->parent = ppuf->parent;
        this->merge_times = ppuf->merge_times;
        this->is_origial = false;
        this->cur_time = ppuf->cur_time;
    }

};

// =============================================================

// https://atcoder.jp/contests/code-thanks-festival-2017/submissions/72476915
int main(){
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N,M;
  cin >> N >> M;
  PartiallyPersistentUnionFind UF(N);
  
  int a,b;
  rep(_,M){
    cin >> a >> b;
    a--;b--;
    UF.merge(a,b);
  }

  int Q,x,y;
  cin >> Q;

  rep(_,Q){
    cin >> x >> y;
    x--;y--;
    cout << UF.connect_time(x,y) << "\n";
  }

}