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
#define vall(A) A.begin(),A.end()
#define vrall(A) A.rbegin(),A.rend()
#define slice(A, l, r) next((A).begin(), (l)), next((A).begin(), (r))
#define vin(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cin >> A[iiii];}
#define vout(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cout << A[iiii] << " \n"[iiii == szszszsz-1];}
#define vin2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cin >> A[iiii][jjjj];}}
#define vout2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cout << A[iiii][jjjj] << " \n"[jjjj==A[iiii].size()-1];}}
#define encode(i,j) ((i)<<32)+j
#define decode(v,w) (w ? (v)%4294967296 : (v)>>32)
#define vinc(A) for (auto &vvvv : A){vvvv++;}
#define vdec(A) for (auto &vvvv : A){vvvv--;}
#define graphin0(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; C[aaaa].push_back(bbbb); C[bbbb].push_back(aaaa);}
#define graphin1(C, M) int aaaa,bbbb;for (int iiii = 0; iiii < (M); iiii++){cin >> aaaa >> bbbb; C[aaaa-1].push_back(bbbb-1); C[bbbb-1].push_back(aaaa-1);}

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

ll powll(ll a, ll n, ll m){
    if (n == 0){return 1;}
    if (n == 1){return a % m;}
    LL ans = 1;
    LL p = a;
    while(n > 0){
        if (n & 1 == 1){
            ans *= p;
            ans %= m;
        }
        n >>= 1;
        p *= p;
        p %= m;
    }
    return (ll)ans;
}

//===================================================================================

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