#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <functional>
#include <random>
#include <bitset>


using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = array<int,2>;

#define vall(A) A.begin(), A.end()
template<typename T> inline void vin(vector<T>& A){for (int i = 0, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void vout(const vector<T>& A){for (int i = 0, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void vout2d(const vector<vector<T>>& A){for (int i = 0, H = A.size(); i < H; i++){vout(A[i]);}}
template<typename T> inline void adjvin(vector<T>& A){for (int i = 1, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void adjvout(const vector<T>& A){for (int i = 1, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void adjvout2d(const vector<vector<T>>& A){for (int i = 1, H = A.size(); i < H; i++){adjvout(A[i]);}}
template<typename T> inline bool btest(T K, int i){return K&(1ull<<i);}
template<typename T> void print(T object){cout << (object) << "\n";}
template<typename T, typename U> void print(T object1, U object2){cout << (object1) << " " << (object2) << "\n";}
template<typename T, typename U, typename V> void print(T object1, U object2, V object3){cout << (object1) << " " << (object2) << " " << (object3) << "\n";}
template<typename T, typename U, typename V, typename W> void print(T object1, U object2, V object3, W object4){cout << (object1) << " " << (object2) << " " << (object3) << " " << (object4) << "\n";}
const vector<ull> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904, 9223372036854775808ull};
const vector<ull> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000, 10000000000000000000ull};
const vector<ll> di{0,1,0,-1};
const vector<ll> di8{0,1,1,1,0,-1,-1,-1};
const vector<ll> dj{1,0,-1,0};
const vector<ll> dj8{1,1,0,-1,-1,-1,0,1};


ll fast_isqrt(ll x){
    ll ret = sqrt(x);
    while (ret*ret > x){
        ret--;
    }
    while ((ret+1)*(ret+1) <= x){
        ret++;
    }
    return ret;
}

ll fast_icbrt(ll x){
    ll ret = cbrt(x);
    while (ret*ret*ret > x){
        ret--;
    }
    while ((ret+1)*(ret+1)*(ret+1) <= x){
        ret++;
    }
    return ret;
}

long long safe_pow(long long a,long long b){
  long long res=1;
  for(long long i=0;i<b;i++){
      long double dres=res;
      dres*=a;
      if(dres>2e18){return 2e18;}
      res*=a;
  }
  return res;
}

unsigned int bit_length(ll n){ return n > 0 ? 64 - __builtin_clzll(n) : 0;}


ll sum_iisqrti(ll N){
    if (N <= 0){return 0;}
    ll M = fast_isqrt(N);
    ll ans = 0;
    ans += 149736653*(M*M%998244353*M%998244353-M%998244353)%998244353*(8*M*M%998244353-5*M%998244353-2)%998244353;
    ans += ((M&1) ? (998244353+M)/2 : M/2)*(N%998244353*(N%998244353+1)%998244353-M*M%998244353*(M*M%998244353-1)%998244353)%998244353;
    return (998244353+ans%998244353)%998244353;
}

ll Sum_of_Prod_of_Root(ll N, ll MOD){

    if (N <= 0){ return 0;}

    int bound = fast_icbrt(N);

    auto modinv = vector<ll>(bound+1);
    auto ppower = vector<ll>(bound+1,0);
    auto mod = vector<ll>(bound+1,0);
    modinv[1] = 1;
    mod[1] = 1;

    for (int i = 2; i <= bound; i++){
        if (i < MOD){
            mod[i] = i;
            modinv[i] = (MOD-modinv[MOD%i]*(MOD/i)%MOD)%MOD;
        } else if (i%MOD == 0){
            ll ii = i;
            while(ii%MOD == 0){
                ii /= MOD;
                ppower[i]++;
            }
            mod[i] = ii%MOD;
            modinv[i] = modinv[ii];
        } else {
            mod[i] = i%MOD;
            modinv[i] = modinv[i%MOD];
        }
    }


    vector<pair<ll,ll>> transitions;
    transitions.push_back({N+1, 1});
    for (ll i = bit_length(N); i >= 3; i--){
        vector<pair<ll,ll>> temp;
        for (ll j = 2;; j++){
            __int128_t temp2 = safe_pow(j,i);
            if (temp2 > N){break;}
            temp.push_back({(ll)temp2, j});
        }
        reverse(temp.begin(), temp.end());
        vector<pair<ll,ll>> transitions2;
        while (true){
            if (temp.empty()){
                for (ll i = (ll)transitions.size()-1; i >= 0; i--){
                    transitions2.push_back(transitions[i]);
                }
                break;
            }
            if (transitions.empty()){
                for (ll i = (ll)temp.size()-1; i >= 0; i--){
                    transitions2.push_back(temp[i]);
                }
                break;
            }
            if (temp.back().first < transitions.back().first){
                transitions2.push_back(temp.back());
                temp.pop_back();
            }
            else{
                transitions2.push_back(transitions.back());
                transitions.pop_back();
            }
        }
        reverse(transitions2.begin(), transitions2.end());
        transitions = transitions2;
    }

    ll temp = 1;
    ll prev = 0;
    ll ans = 0;
    ll zero_pow = 0;
    for (ll i = (ll)transitions.size()-1; i >= 0; i--){
        auto& now = transitions[i];
        auto prev_tmp = sum_iisqrti(now.first-1);
        if (zero_pow == 0){
            ans += temp*(prev_tmp-prev)%MOD;
            ans %= MOD;
        }
        zero_pow += ppower[now.second] - ppower[now.second-1];
        temp = temp*modinv[now.second-1]%MOD*mod[now.second]%MOD;
        prev = prev_tmp;
    }

    ans %= MOD;
    if (ans < 0){ans += MOD;}
    return ans;
}

unsigned long long kth_root_integer(const unsigned long long x, const int k) {
    if(k <= 1) return k ? x : 1;
    if(x <= 1) return x;
    if(k >= 64) return 1;
    auto check = [&](unsigned long long a) -> bool {
        unsigned long long power = 1;
        for(int i = k; i;) {
            if(i & 1) {
                if(power > x / a) return false;
                power *= a;
            }
            if(i >>= 1) {
                if(a > x / a) return false;
                a *= a;
            }
        }
        return power <= x;
    };
    unsigned long long res = pow(x, 1.0 / k);
    while(!check(res)) --res;
    while(check(res + 1)) ++res;
    return res;
}


ll Sum_of_Prod_of_Root_test(ll N,ll MOD){
    if (N <= 0){ return 0;}
    ll ans = 0;
    for (int i=1;i<=N;i++){
        ll tmp = 1;
        for (int k = 1;k<=60;k++){
            tmp *= kth_root_integer(i, k);
            tmp %= MOD;
        }
        ans += tmp;
        ans %= MOD;
    }
    return ans;
}


// ===========================================

// void solve(){
//     constexpr int p = 3;
//     for(int N=64;N<=1025;N++){
//         if (Sum_of_Prod_of_Root<N>(N, p) == Sum_of_Prod_of_Root_test<N>(N, p)){
//         } else {
//             cout << N << " is false." << endl;
//             cout << Sum_of_Prod_of_Root(N, 998244353) << " " << Sum_of_Prod_of_Root_test(N, 998244353) << endl;
//             return;
//         };
//     }
//     cout << "all ok." << endl;
//     return;
// }

void solve(){
    ll N;
    cin >> N;
    cout << Sum_of_Prod_of_Root(N, 998244353) << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll T = 1;
    //cin >> T;
    while (T--){
        solve();
    }
}


