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
#define slice(A, l, r) next((A).begin(), (l)), next((A).begin(), (r))
#define vin(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cin >> A[iiii];}
#define vout(A) for (ll iiii = 0, szszszsz = A.size(); iiii < szszszsz; iiii++){cout << A[iiii] << " \n"[iiii == szszszsz-1];}
#define vin2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cin >> A[iiii][jjjj];}}
#define vout2d(A) for (ll iiii = 0; iiii < A.size(); iiii++){for (ll jjjj = 0; jjjj < A[iiii].size(); jjjj++){cout << A[iiii][jjjj] << " \n"[jjjj==A[iiii].size()-1];}}
#define encode(i,j) ((i)<<32)+j
#define decode(v,w) (w ? (v)%4294967296 : (v)>>32)
#define vinc(A) for (auto &vvvv : A){vvvv++;}
#define vdec(A) for (auto &vvvv : A){vvvv--;}

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
    return ans;
}



// ======================================================================================


struct Permutation
{
    int N;
    vector<int> array;
    vector<int> array_inv;

    Permutation(int N):N(N),array(N,0),array_inv(N,0){
        for (int i = 0;i<N;i++){
            array[i] = i;
            array_inv[i] = i;
        }
    }

    Permutation(vector<int> data):N(data.size()),array(N,-1),array_inv(N,-1){
        for (int i = 0;i<N;i++){
            if (0 <= data[i] && data[i] < N && array[i] == -1){
                array[i] = data[i];
                array_inv[array[i]] = i;
            } else {
                throw invalid_argument("array is not a Permutation.");
            }
        }
    }

    void pre_swap(int i, int j){
        assert(0 <= i && i < N);
        assert(0 <= j && j < N);
        swap(this->array_inv[this->array[i]],this->array_inv[this->array[j]]);
        swap(this->array[i],this->array[j]);
    }

    void post_swap(int i, int j){
        assert(0 <= i && i < N);
        assert(0 <= j && j < N);
        swap(this->array[this->array_inv[i]],this->array[this->array_inv[j]]);
        swap(this->array_inv[i],this->array_inv[j]);
    }

    void inverse(){
        swap(this->array, this->array_inv);
    }

    void next(){
        next_permutation(vall(this->array));
        for (int i=0;i<N;i++){
            array_inv[array[i]] = i;
        }
    }

    void prev(){
        prev_permutation(vall(this->array));
        for (int i=0;i<N;i++){
            array_inv[array[i]] = i;
        }
    }

    vector<vector<int>> divided_cycles(){
        vector<bool> visited(N,false);
        int j = -1;
        vector<vector<int>> ans;
        for (int i=0;i<N;i++){
            if (visited[i]){
                continue;
            }
            vector<int> res;
            visited[i] = true;
            j = i;
            res.push_back(i);
            while (!visited[this->array[j]]){
                j = this->array[j];
                visited[j] = true;
                res.push_back(j);
            }
            ans.push_back(res);
        }
        return ans;
    }

    vector<int> p2vec(){
        return this->array;
    }

    const int& operator[](int ind) const {
        return this->array[ind];
    }

    void print(){
        for (int i=0;i<N;i++){
            cout << this->array[i] << " \n"[i == N-1];
        }
    }

    int size(){
        return N;
    }
};

Permutation pow(Permutation P,ll K){
    vector<int> ans(P.size(),-1);
    vector<vector<int>> cycles = P.divided_cycles();
    ll L;
    for (ll i=0;i<cycles.size();i++){
        L = cycles[i].size();
        for (ll j=0;j<L;j++){
            ans[cycles[i][j]] = cycles[i][(j+K)%L];
        }
    }
    return Permutation(ans);
}

Permutation powpow(Permutation P,ll c,ll K){
    vector<int> ans(P.size(),-1);
    vector<vector<int>> cycles = P.divided_cycles();
    ll L;
    for (ll i=0;i<cycles.size();i++){
        L = cycles[i].size();
        for (ll j=0;j<L;j++){
            ans[cycles[i][j]] = cycles[i][(j+powll(c,K,L))%L];
        }
    }
    return Permutation(ans);
}

Permutation product(Permutation P, Permutation Q){
    assert(P.size() == Q.size());
    int N = P.size();
    vector<int> R(N,-1);
    for (int i=0;i<N;i++){
        R[i] = P.array[Q.array[i]];
    }
    return Permutation(R);
}

// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    ll N;
    cin >> N;
    vector<int> A(N);
    vin(A);
    vdec(A);
    Permutation P(A);

    ll ans = 0;
    ll g;
    for (auto cyc:P.divided_cycles()){
        g = cyc.size();
        ans += g*(g-1)/2;
    }
    cout << ans << endl;    
}
