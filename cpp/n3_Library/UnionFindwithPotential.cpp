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

struct UnionFindwithPotential{
    
};




