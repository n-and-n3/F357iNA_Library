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


struct RollingHash{
    using ulong = unsigned long long;

    vector<ulong> data;
    vector<ulong> pows;
    static const ulong MOD = (1ULL << 61) - 1;
    static const ulong base;
    size_t sz;

    RollingHash(const string& s): data(s.size()+1, 0), pows(s.size()+1, 1), sz(s.size()){
        for (size_t i = 0; i < s.size(); i++){
            data[i+1] = CalcMod(Mul(data[i], base) + s[i]);
            pows[i+1] = Mul(pows[i], base);
        }
    }

    ulong get(int l, int r){
        assert(0 <= l && l <= r && r <= sz);
        return CalcMod(data[r] + MOD - Mul(data[l], pows[r-l]));
    }

    ulong get_all(){
        return data[sz];
    }

    ulong concat(int a, int b, int c, int d){
        assert(0 <= a && a <= b && b <= sz);
        assert(0 <= c && c <= d && d <= sz);
        ulong tmp1 = CalcMod(data[b] + MOD - Mul(data[a], pows[b-a]));
        ulong tmp2 = CalcMod(data[d] + MOD - Mul(data[c], pows[d-c]));
        return CalcMod(Mul(tmp1,pows[d-c]) + tmp2);
    }

    size_t size(){
        return sz;
    }

    private:

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

    inline ulong CalcMod(ulong x)
    {
        ulong xu = x >> 61;
        ulong xd = x & ((1ULL << 61) - 1);
        ulong res = xu + xd;
        if (res >= ((1ULL << 61) - 1)) res -= ((1ULL << 61) - 1);
        return res;
    }

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

};

const RollingHash::ulong RollingHash::base = RollingHashEngine().base;

// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    cin >> N;

    string S;
    cin >> S;

    RollingHashEngine engine;
    RollingHash rh(S);

    for (int len = N; len > 0; len--){
        unordered_map<ulong, int> S;
        for (int i = 0; i + len <= N; i++){
            auto tmp = rh.get(i, i+len);
            if (S.find(tmp) != S.end()){
                int j = S[tmp];
                if (j + len <= i){ // 重なりがないか
                    cout << len << "\n";
                    return 0;
                }
            } else {
                S[tmp] = i;
            }
        }
    }

    cout << 0 << "\n";
}