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
#include <initializer_list>
#include <bit>
#include <array>
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
vector<ll> pow3ll{1,3,9,};
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

// ==========================================================================

// @brief TernaryIndexedTree
template <typename T>
struct TIT {
    vector<T> array;
    size_t sz;
    TIT(initializer_list<T> init_list){
        init(init_list.begin(), init_list.end());
    }

    TIT(const vector<T>& init_vec){
        init(init_vec.begin(), init_vec.end());
    }

private:
    template <class It>
    void init(It first, It last){
        // 冒頭に0を挿入したうえで初期化
        array.clear();
        array.push_back(0);
        array.insert(array.end(), first, last); // 元の配列を代入

        // サイズを (3^k-1)/2 型 にする。3の累乗でもいいけど、平衡三進数的にはこれが自然。
        int tmp = 1;
        while (tmp < (int)array.size()) {
            tmp = 3*tmp+1;
        }
        array.resize(tmp+1, 0); // 1-indexed なので +1 する
        sz = array.size() - 1;

        // テーブルの構築
        int pos = 2;
        int p = 1;
        while (pos < (int)array.size()) {
            for (int i = pos; i <= (int)sz; i+=3*p) {
                array[i] += array[i - p];
                array[i] += array[i + p];
            }
            pos = 3 * pos - 1;
            p *= 3;
        }
    }

public:

    T cum(int ind){
        T res = 0;
        int p = 1;
        while (ind > 0){
            if (ind % 3 == 1){
                res += array[ind*p - (p-1)/2];
                ind -= 1;
            } else if (ind % 3 == 2){
                ind += 1;
                res -= array[ind*p - (p-1)/2];
            }
            ind /= 3;
            p *= 3;
        }
        return res;
    }

    T get(int ind){
        return cum(ind) - cum(ind-1);
    }

    void add(int ind, T x){
        int p = 1;
        while (ind*p <= sz){
            if (ind % 3 != 2){
                array[ind*p - (p-1)/2] += x;
            }
            ind = (ind-1)/3+1;
            p *= 3;
        }
    }

    void write(int ind, T x){
        add(ind, x - get(ind));
    }

    size_t size(){
        return sz;
    }


};


// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N,Q;
    cin >> N >> Q;
    vector<ll> A(N);
    vin(A);
    TIT<ll> tit(A);

    int t,x,y;
    rep(i,Q){
        cin >> t >> x >> y;
        if (t == 0){
            tit.add(x+1,y);
        } else {
            cout << tit.cum(y) - tit.cum(x) << endl;
        }
    }
}