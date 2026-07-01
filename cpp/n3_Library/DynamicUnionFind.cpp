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
#include <cassert>
#include <functional>


using namespace std;
#define ll long long
#define LL __int128
#define MOD 998244353
#define ld long double
#define INF 2251799813685248
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

// =============================================================

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
#include <cassert>
#include <functional>


using namespace std;
#define ll long long
#define LL __int128
#define MOD 998244353
#define ld long double
#define INF 2251799813685248
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

// ======================================================================================================

// https://judge.yosupo.jp/submission/334110
// https://atcoder.jp/contests/atc001/submissions/70817243
// https://atcoder.jp/contests/abc364/submissions/71087452

struct UnionFind{
    vector<int> parent;
    int c;

    UnionFind(int n) : parent(n,-1),c(n){ }

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

    bool marge(int x,int y){
        int xr = root(x),yr = root(y);
        if (xr == yr){
            return false;
        }
        if (parent[xr] <= parent[yr]){
            parent[xr] += parent[yr];
            parent[yr] = xr;
        } else {
            parent[yr] += parent[xr];
            parent[xr] = yr;
        }
        c -= 1;
        return true;
    }

    int group_count(){
        return c;
    }

    vector<int> label(){
        vector<int> res(parent.size());
        for (int i=0;i<parent.size();i++){
            res[i] = root(i);
        }
        return res;
    }

    vector<int> comped_label(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<int> res(parent.size());
        for (int i = 0; i < parent.size(); i++) {
            res[i] = lower_bound(B.begin(), B.end(), A[i]) - B.begin();
        }
        return res;
    }

    vector<vector<int>> groups(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<vector<int>> res(c);
        for (int i = 0; i < parent.size(); i++) {
            res[lower_bound(B.begin(), B.end(), A[i]) - B.begin()].push_back(i);
        }
        return res;
    }

    vector<int> group_sizes(){
        vector<int> A = label();
        vector<int> B = A;
        sort(B.begin(), B.end());
        B.erase(unique(B.begin(), B.end()), B.end());

        vector<int> res(c,0);
        for (int i = 0; i < parent.size(); i++) {
            res[lower_bound(B.begin(), B.end(), A[i]) - B.begin()] += 1;
        }
        return res;
    }

    int extend(){
        parent.push_back(-1);
        c += 1;
        return parent.size()-1;
    }

    void print(){
      cout << "{";
        for (int i = 0; i < parent.size(); i++){
            cout << parent[i] << ", ";
        }
      cout << "}" << "\n";
    }
};


template <typename T>
struct DynamicUnionFind{
    unordered_map<T,int> pointer;
    vector<T> obj;
    UnionFind UF;

    DynamicUnionFind<T>() : UF(0) { };

    int get_pointer(T x){
        int px;
        if (pointer.count(x) == 0){
            obj.push_back(x);
            px = UF.extend();
            pointer[x] = px;
        } else {
            px = pointer[x];
        }
        return px;
    }
    
    T root(T x){
        int px,pr;
        px = get_pointer(x);
        pr = UF.root(px);
        return obj[pr];
    }

    bool same(T x, T y){
        int px,py;
        px = get_pointer(x);
        py = get_pointer(y);
        return UF.same(px,py);
    }

    int size(T x){
        int px;
        px = get_pointer(x);
        return UF.size(px);
    }

    bool marge(T x, T y){
        int px,py;
        px = get_pointer(x);
        py = get_pointer(y);
        return UF.marge(px,py);
    }

    int group_count(){
        return UF.group_count();
    }

    unordered_map<T,int> label(){
        unordered_map<T,int> res = {};
        auto poiner_label = UF.label();
        for (int i=0;i < poiner_label.size();i++){
            res[obj[i]] = poiner_label[i];
        }
        return res;
    }

    unordered_map<T,int> comped_label(){
        unordered_map<T,int> res = {};
        auto poiner_comped_label = UF.comped_label();
        for (int i=0;i < poiner_comped_label.size();i++){
            res[obj[i]] = poiner_comped_label[i];
        }
        return res;
    }

    vector<vector<T>> groups(){
        auto poiner_groups = UF.groups();
        vector<vector<T>> res(poiner_groups.size());
        for (int i=0; i < poiner_groups.size(); i++) {
            for (int j=0; j < poiner_groups[i].size();j++){
                res[i].push_back(obj[poiner_groups[i][j]]);
            }
        }
        return res;
    }

    vector<int> group_sizes(){
        return UF.group_sizes();
    }
};

int main(){
  int N,Q;
  int t;
  string u,v;
  cin >> N >> Q;
  auto UF = DynamicUnionFind<string>();
  for (int i=0;i<Q;i++){
      cin >> t >> u >> v;
      if (t == 0){
          UF.marge(u,v);
      } else {
          if (UF.same(u,v)){
            cout << "1" << "\n";
          } else {
            cout << "0" << "\n";
          }
      }
  }

  cout << "\n";
  auto X = UF.group_sizes();
  for (int i=0;i<X.size();i++){
    cout << X[i] << " ";
  }
  cout << "\n";

  cout << "\n";
  auto Y = UF.groups();
  for (int i=0;i<Y.size();i++){
    for (int j=0;j<Y[i].size();j++){
      cout << Y[i][j] << " ";
    }
    cout << "\n";
  }
}

/*
sample input:
3 7
0 Alice Bob
0 Bob Charlie
1 Alice Charlie
0 Dave Ellen
1 Dave Bob
0 Alice Charlie
1 Dave Ellen


*/