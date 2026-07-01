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

// https://atcoder.jp/contests/atc001/submissions/70817243
// https://atcoder.jp/contests/abc364/submissions/71087452

struct UnionFind{
    vector<int> parent;
    vector<int> next_v;
    int c;

    UnionFind(int n) : parent(n,-1),next_v(n),c(n){
        for(int i=0; i<n; i++){
            next_v[i] = i;
        }
    }

    int root(int x){
        int tmp = x;
        while (parent[tmp] >= 0){
            tmp = parent[tmp];
        }

        while (parent[x] >= 0){
            parent[x] = tmp;
            x = parent[x];
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
        int xr = root(x),yr = root(y);
        if (xr == yr){
            return false;
        }
        if (parent[xr] > parent[yr]){
            swap(xr,yr);
        }
        parent[xr] += parent[yr];
        parent[yr] = xr;
        swap(next_v[xr], next_v[yr]);
        c -= 1;
        return true;
    }

    int group_count(){
        return c;
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

    vector<int> label(){
        vector<int> res(parent.size(),-1);
        int cnt = 0;
        rep(v,parent.size()){
            if (res[v] != -1){continue;}
            int tmp = v;
            while (res[tmp] == -1){
                res[tmp] = cnt;
                tmp = next_v[tmp];
            }
            cnt++;
        }
        return res;
    }

    vector<vector<int>> groups(){
        vector<vector<int>> res(c);
        vector<int> L = label();
        rep(v,parent.size()){
            res[L[v]].push_back(v);
        }
        return res;
    }

    vector<int> group_sizes(){
        vector<int> res(c);
        vector<int> L = label();
        rep(v,parent.size()){
            res[L[v]] += 1;
        }
        return res;
    }

    void push(){
        parent.push_back(-1);
        next_v.push_back(next_v.size());
        c += 1;
    }

    void print(){
      cout << "{";
        for (int i = 0; i < parent.size(); i++){
            cout << parent[i] << ", ";
        }
      cout << "}" << "\n";
    }
};


void print(vector<int> A){
    cout << "{";
    for (int i = 0; i < A.size(); i++){
        cout << A[i] << ", ";
    }
    cout << "}" << "\n";
}
void print(vector<vector<int>> A){
    cout << "{";
    for (int i = 0; i < A.size(); i++){
        print(A[i]);
        cout << "," << "\n";
    }
    cout << "}" << "\n";
}

//======================================================================

// https://atcoder.jp/contests/past202203-open/submissions/72463147
int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N,Q;
    cin >> N >> Q;

    UnionFind UF(N); 

    int q,u,v;
    rep(i,Q){
        cin >> q;
        if (q == 1){
            cin >> u >> v;
            u--;v--;
            UF.merge(u,v);
        } else {
            cin >> u;
            u--;
            auto ans = UF.component(u);
            sort(vall(ans));
        }
    }
    //cout << UF.edges(0) << "\n";
    //cout << UF.edges(1) << "\n";
    //cout << UF.edges(2) << "\n";
    //cout << UF.edges(3) << "\n";
    //cout << UF.edges(4) << "\n";
}