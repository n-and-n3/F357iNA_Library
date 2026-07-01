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
#include <random>
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

struct FullyIndexableDictionary_{
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    size_t sz,count;  // szはビット数、countはチャンク数
    bool is_builded;

    vector<u64> data;  // 64bit区切りでデータを持っておく
    vector<u32> CT;  // チャンクテーブル、各チャンクの先頭の rank を前計算する。
    vector<u64> BT;  // ブロックテーブル、各ブロックが所属するチャンクの始点からの rank を前計算する。u9 * 7 を u64を使い違法圧縮している。展開用関数を作らねば...
    int popcount;

    FullyIndexableDictionary_(size_t sz):is_builded(false), sz(sz), count(sz/512+1), data((sz/512+1)*8,0), CT((sz/512+1),0), BT((sz/512+1),0){};

    void set(int i, bool b){
        assert(!is_builded);
        assert(0 <= i && i < sz);
        if (b){ write1(i);} else { write0(i);}
    }

    void set(int i, char b){
        assert(!is_builded);
        assert(0 <= i && i < sz);
        if (b=='0'){ write0(i);} else if (b == '1'){ write1(i);} else {assert(false);}
    }

    void set(int i, int b){
        assert(!is_builded);
        assert(0 <= i && i < sz);
        if (b==0){ write0(i);} else if (b == 1){ write1(i);} else {assert(false);}
    }

    void set(int i, ll b){
        assert(!is_builded);
        assert(0 <= i && i < sz);
        if (b==0){ write0(i);} else if (b == 1){ write1(i);} else {assert(false);}
    }

    bool test(int i){
        assert(0 <= i && i < sz);
        return ((data[i/64]>>(i&63))&1)==1;
    }

    void build(){
        assert(!is_builded);
        is_builded = true;

        int rank_all = 0;
        int rank_part = 0;
        for (int i=0; i<count ;i++){
            CT[i] = rank_all;
            rank_part = 0;
            for (int j=0;j<8;j++){
                write64(BT[i], rank_part, j);
                rank_part += __builtin_popcountll(data[(i<<3)+j]);
            }
            rank_all += rank_part;
        }
        popcount = rank_all;
    }

    int rank(int n,int b){  // [0,n) までに含まれる b の個数
        if (n == sz){return rank_all(b);}
        assert(0 <= n && n < sz);
        if (b == 0){
            return n - rank1(n);
        } else if (b == 1){
            return rank1(n);
        } else {
            assert(false);
        }
    }

    int select(int n,int b){  // rank(k) = n となる最小の k の値
        assert(0 <= n && n < sz);
        if (b == 0){
            if (n < sz - popcount){
                return select0(n);
            } else {
                return -1;
            }
        } else if (b == 1){
            if (n < popcount){
                return select1(n);
            } else {
                return -1;
            }
        } else {
            assert(false);
        }
    }

    int rank_all(int b){
        if (b == 0){
            return sz - popcount;
        } else if (b == 1){
            return popcount;
        } else {
            assert(false);
        }
    }

    inline int operator[](int i) {
        return test(i);
    }

    private:
    inline void write1(int i){
        data[i>>6] |= ((u64)1)<<(i&((1<<6)-1));
    }

    inline void write0(int i){
        data[i>>6] &= ~(((u64)1)<<(i&((1<<6)-1)));
    }

    inline int read64(u64 bits, int i){
        // assert(0 <= i < 8)
        if (i == 0){
            return 0;
        } else {
            return (bits>>((i-1)*9))&((1<<9)-1);
        }
    }

    inline void write64(u64& bits, int num, int i){
        // assert(0 <= i < 8)
        if (i != 0){
            bits |= ((u64)num)<<((i-1)*9);
        }
    }

    inline int rank1(int n){
        return CT[n/512] + read64(BT[n/512],(n&511)>>6) + __builtin_popcountll((data[n/64] & ((((u64)1)<<(n&63))-((u64)1))));
    }

    int select1(int n){  // 二分探索、rank(k,1) <= n となる最大の k を返す
        int ok_c = 0;
        int ng_c = count;
        int mid_c;
        while (ng_c - ok_c > 1){
            mid_c = (ok_c+ng_c)/2;
            if (CT[mid_c] <= n){
                ok_c = mid_c;
            } else {
                ng_c = mid_c;
            }
        }

        int ok_b = 0;
        int ng_b = 8;
        int mid_b;
        while (ng_b - ok_b > 1){
            mid_b = (ok_b+ng_b)/2;
            if (CT[ok_c] + read64(BT[ok_c],mid_b) <= n){
                ok_b = mid_b;
            } else {
                ng_b = mid_b;
            }
        }

        int ok_d = 0;
        int ng_d = 64;
        int mid_d;
        while (ng_d - ok_d > 1){
            mid_d = (ok_d+ng_d)/2;
            auto res = (data[8*ok_c+ok_b] & ((((u64)1)<<(mid_d))-((u64)1)));
            if (CT[ok_c] + read64(BT[ok_c],ok_b) + __builtin_popcountll((data[8*ok_c+ok_b] & ((((u64)1)<<(mid_d))-((u64)1)))) <= n){
                ok_d = mid_d;
            } else {
                ng_d = mid_d;
            }
        }

        return 512*ok_c + 64*ok_b + ok_d;
    }

    int select0(int n){  // 二分探索、rank(k,0) <= n となる最大の k を返す
        int ok_c = 0;
        int ng_c = count;
        int mid_c;
        while (ng_c - ok_c > 1){
            mid_c = (ok_c+ng_c)/2;
            if ((512*mid_c) - (CT[mid_c]) <= n){
                ok_c = mid_c;
            } else {
                ng_c = mid_c;
            }
        }

        int ok_b = 0;
        int ng_b = 8;
        int mid_b;
        while (ng_b - ok_b > 1){
            mid_b = (ok_b+ng_b)/2;
            if ((512*ok_c + 64*mid_b) - (CT[ok_c] + read64(BT[ok_c],mid_b)) <= n){
                ok_b = mid_b;
            } else {
                ng_b = mid_b;
            }
        }

        int ok_d = 0;
        int ng_d = 64;
        int mid_d;
        while (ng_d - ok_d > 1){
            mid_d = (ok_d+ng_d)/2;
            auto res = (data[8*ok_c+ok_b] & ((((u64)1)<<(mid_d))-((u64)1)));
            if ((512*ok_c + 64*ok_b + mid_d) - (CT[ok_c] + read64(BT[ok_c],ok_b) + __builtin_popcountll((data[8*ok_c+ok_b] & ((((u64)1)<<(mid_d))-((u64)1))))) <= n){
                ok_d = mid_d;
            } else {
                ng_d = mid_d;
            }
        }

        return 512*ok_c + 64*ok_b + ok_d;
    }

};

// ===============================================================================

int main(){
 
    FullyIndexableDictionary_ A(1000000000);

    mt19937 gen(3);
    uniform_int_distribution<int> ui(0, 1);
    uniform_int_distribution<int> ui2(0, 5000000);
    for (ll i = 0; i < 100000000; i++){
        if (ui(gen)) {A.set(i,1);} else {A.set(i,0);}
    }
    A.build();
 
 }

/*
int main(){

    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T = 550;

    FullyIndexableDictionary bit_vector(T);

    rep(i,T){
        bit_vector.set(i,i&1);
    }

    bit_vector.build();

    rep(i,T+1){
        cout << bit_vector.rank(i,0) << " \n"[i == T];
    }
    cout << endl;
    rep(i,T+1){
        cout << bit_vector.rank(i,1) << " \n"[i == T];
    }
    cout << endl;
    rep(i,T){
        cout << bit_vector.select(i,0) << " \n"[i == T];
    }
    cout << endl;
    rep(i,T){
        cout << bit_vector.select(i,1) << " \n"[i == T];
    }
    cout << endl;


    FullyIndexableDictionary bit_vector2(10);

    bit_vector2.set(0, 0);
    bit_vector2.set(1, 0);
    bit_vector2.set(2, 1);
    bit_vector2.set(3, 1);
    bit_vector2.set(4, 0);
    bit_vector2.set(5, 0);
    bit_vector2.set(6, 1);
    bit_vector2.set(7, 1);
    bit_vector2.set(8, 0);
    bit_vector2.set(9, 1);

    bit_vector2.build();

    cout << bit_vector2.test(0) << endl;
    cout << bit_vector2.test(1) << endl;
    cout << bit_vector2.test(2) << endl;
    cout << bit_vector2.test(3) << endl;
    cout << bit_vector2.test(4) << endl;
    cout << bit_vector2.test(5) << endl;
    cout << bit_vector2.test(6) << endl;
    cout << bit_vector2.test(7) << endl;
    cout << bit_vector2.test(8) << endl;
    cout << bit_vector2.test(9) << endl;
    cout << endl;

    cout << bit_vector2.rank(0,1) << endl;
    cout << bit_vector2.rank(1,1) << endl;
    cout << bit_vector2.rank(2,1) << endl;
    cout << bit_vector2.rank(3,1) << endl;
    cout << bit_vector2.rank(4,1) << endl;
    cout << bit_vector2.rank(5,1) << endl;
    cout << bit_vector2.rank(6,1) << endl;
    cout << bit_vector2.rank(7,1) << endl;
    cout << bit_vector2.rank(8,1) << endl;
    cout << bit_vector2.rank(9,1) << endl;
    cout << bit_vector2.rank(10,1) << endl;
    cout << endl;

    cout << bit_vector2.select(0,1) << endl;
    cout << bit_vector2.select(1,1) << endl;
    cout << bit_vector2.select(2,1) << endl;
    cout << bit_vector2.select(3,1) << endl;
    cout << bit_vector2.select(4,1) << endl;
    cout << bit_vector2.select(5,1) << endl;
    cout << endl;

    cout << bit_vector2.select(0,0) << endl;
    cout << bit_vector2.select(1,0) << endl;
    cout << bit_vector2.select(2,0) << endl;
    cout << bit_vector2.select(3,0) << endl;
    cout << bit_vector2.select(4,0) << endl;
    cout << bit_vector2.select(5,0) << endl;
    cout << endl;

    cout << "end" << endl;
    
}
*/