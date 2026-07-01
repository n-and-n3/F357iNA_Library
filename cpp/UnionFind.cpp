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
            vinc(ans);
            vout(ans);
        }
    }
    //cout << UF.edges(0) << "\n";
    //cout << UF.edges(1) << "\n";
    //cout << UF.edges(2) << "\n";
    //cout << UF.edges(3) << "\n";
    //cout << UF.edges(4) << "\n";
}