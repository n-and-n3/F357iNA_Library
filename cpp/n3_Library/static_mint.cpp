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

// ===============================================================================

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

// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    mint a = 10;
    mint b = 20;
    cout << a + b << endl;
    cout << a - b << endl;
    cout << a * b << endl;
    cout << a / b << endl;
    cout << a.pow(5) << endl;
    cout << a.pow(10) << endl;
    cout << a*a*a*a*a*a*a*a*a*a << endl;
    
}