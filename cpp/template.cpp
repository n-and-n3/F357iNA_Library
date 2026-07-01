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

#define vall(A) (A),begin(),(A).end()
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

//=========================================================================================

struct Eratosthenes{
    vector<bool> isprime;//素数であるか
    vector<int> minfactor;//最小素因数
    vector<int> mobius;//メビウス関数の列挙

    Eratosthenes(ll N):isprime(N+1,true),minfactor(N+1,-1),mobius(N+1,1){
        isprime[1]=false;
        minfactor[1]=1;
        for(ll i=2;i<=N;i++){
            if(!isprime[i]) continue;
            minfactor[i]=i;
            mobius[i]=-1;
            for(ll j=i*2;j<=N;j+=i){
                isprime[j]=false;
                if(minfactor[j]==-1) minfactor[j]=i;
                if((j/i)%i==0) mobius[j]=0;
                else mobius[j]=-mobius[j];
            }
        }
    }

    vector<pair<ll,ll>> factorize(ll n){
        vector<pair<ll,ll>> res;
        while(n>1){
            int p=minfactor[n];
            int exp=0;
            while(minfactor[n]==p){
                n/=p;
                ++exp;
            }
            res.push_back({p,exp});
        }
        return res;
    }

    vector<ll> divisors(ll n){
        vector<ll> res({1});
        auto pf=factorize(n);
        for(auto p:pf){
            ll s=res.size();
            for(ll i=0;i<s;i++){
                ll v=1;
                for(ll j=0;j<p.second;++j){
                    v*=p.first;
                    res.push_back(res[i]*v);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }
};

//約数系の高速ゼータ変換
//f(1)…f(N)から約数和を求める。
template<class T> void fast_zeta(vector<T> &f){
    ll N=f.size();
    Eratosthenes Er(N);
    rep(p,2,N){
        if(!Er.isprime[p]) continue;
        for(ll k=(N-1)/p;k>=1;--k){
            f[k]+=f[k*p];
        }
    }
}
//高速メビウス変換
template<class T> void fast_mobius(vector<T> &f){
    ll N=f.size();
    Eratosthenes Er(N);
    rep(p,2,N){
        if(!Er.isprime[p]) continue;
        for(ll k=1;k*p<N;++k){
            f[k]-=f[k*p];
        }
    }
}

//オイラー関数
vll Eular_func(ll N){
    vll res(N+1);
    rep(i,0,N+1) res[i]=i;
    rep(i,2,N+1){
        if(res[i]==i){
            for(ll j=i;j<=N;j+=i){
                res[j]=res[j]/i*(i-1);
            }
        }
    }
    return res;
}

//=========================================================================================


int main(){
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

}