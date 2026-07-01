#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

typedef long long ll;

// --- 提供された補助関数 ---
ll fast_isqrt(ll x){
    ll ret = sqrt(x);
    while (ret*ret > x) ret--;
    while ((ret+1)*(ret+1) <= x) ret++;
    return ret;
}

ll fast_icbrt(ll x){
    ll ret = cbrt(x);
    while (ret*ret*ret > x) ret--;
    while ((ret+1)*(ret+1)*(ret+1) <= x) ret++;
    return ret;
}

unsigned int bit_length(ll n){ return n > 0 ? 64 - __builtin_clzll(n) : 0;}

ll cipolla(ll a, ll const p) { 
    if (p == 2){return a;}
    if (a == 0){return 0;}
    int e = (p-1)/2;
    ll res = 1,tmp = a;
    while(e > 0){
        if (e & 1){res *= tmp;res %= p;}
        tmp *= tmp;tmp %= p;e >>= 1;
    }
    if (res != 1){return -1;}
    ll b = 0;
    while (res != p-1){
        b++;
        e = (p-1)/2;
        res = 1;tmp = (b*b+p-a)%p;
        while(e > 0){
            if (e & 1){res *= tmp;res %= p;}
            tmp *= tmp;tmp %= p;e >>= 1;
        }
    }
    ll base = (b*b+p-a)%p;
    ll res_real = 1, res_image = 0;
    ll tmp_real = b, tmp_image = 1;
    pair<ll,ll> tmpp;
    e = (p+1)/2;
    while(e > 0){
        if (e & 1){
            tmpp = {res_real*tmp_real%p + res_image*tmp_image%p*base%p, res_real*tmp_image%p + res_image*tmp_real%p};
            res_real = tmpp.first; res_image = tmpp.second;
            if (res_real >= p){res_real-=p;}
            if (res_image >= p){res_image-=p;}
        }
        tmpp = {tmp_real*tmp_real%p + tmp_image*tmp_image%p*base%p, tmp_real*tmp_image%p*2};
        tmp_real = tmpp.first; tmp_image = tmpp.second;
        if (tmp_real >= p){tmp_real-=p;}
        if (tmp_image >= p){tmp_image-=p;}
        e >>= 1;
    }
    return res_real;
}

inline ll ceil_sqrt(ll x){ 
    ll tmp = fast_isqrt(x);
    if (tmp * tmp == x) return tmp;
    else return tmp+1;
}

template <typename T>
pair<T,T> exgcd(T a, T b){
    T xs = 1, ys = 0, xt = 0, yt = 1, tmp;
    while (b != 0){
        tmp = a/b; a = a%b;
        xs -= tmp*xt; ys -= tmp*yt;
        swap(xs,xt); swap(ys,yt);
        swap(a,b);
    }
    return {xs,a};
}

// __int128_t に対応した安全な逆元計算
__int128_t inv(__int128_t x, __int128_t p){
    return (exgcd<__int128_t>(x, p).first % p + p) % p;
}

// --- 追加の補助関数 ---

// P^k が非常に大きくなった際のオーバーフローを防ぐ区間カウント関数
inline ll safe_count_liner(__int128_t l, __int128_t r, __int128_t c, __int128_t M) {
    if (l >= r) return 0;
    auto count_up_to = [&](__int128_t x) -> __int128_t {
        if (x <= c) return 0;
        return (x - 1 - c) / M + 1;
    };
    return (ll)(count_up_to(r) - count_up_to(l));
}

int count_divisors(int g) {
    int d = 0;
    for (int j = 1; j * j <= g; ++j) {
        if (g % j == 0) { d++; if (j * j != g) d++; }
    }
    return d;
}

// S(i) = i - 1 + f(i) における 3乗根以上の寄与を計算する関数
ll calc_f(ll i) {
    ll sum = 0;
    for (int m = 3; m <= 60; ++m) {
        ll r = round(pow(i, 1.0 / m));
        while (true) {
            __int128_t p = 1; bool over = false;
            for (int j = 0; j < m; ++j) { p *= r; if (p > i) { over = true; break; } }
            if (over || r <= 0) r--; else break;
        }
        while (true) {
            __int128_t p = 1; bool over = false;
            for (int j = 0; j < m; ++j) { p *= (r+1); if (p > i) { over = true; break; } }
            if (!over) r++; else break;
        }
        if (r < 2) break;
        sum += (r - 1);
    }
    return sum;
}


ll Prod_of_Sum_of_Log(ll N, ll P){
    // 1. S(N)! に含まれる素因数 P の個数を初期値とする
    ll S_N = N - 1 + (fast_isqrt(N) - 1) + calc_f(N);
    ll ans = 0;
    ll temp = S_N;
    while (temp >= P) {
        ans += temp / P;
        temp /= P;
    }

    // 2. グループ2: 高次累乗数の列挙と補正
    vector<ll> E_vec;
    for (int m = 3; m <= 60; ++m) {
        ll limit = round(pow(N, 1.0 / m)) + 2;
        for (ll z = 2; z <= limit; ++z) {
            __int128_t zm = 1; bool over = false;
            for (int i = 0; i < m; ++i) { zm *= z; if (zm > N) { over = true; break; } }
            if (!over) E_vec.push_back((ll)zm);
            else break;
        }
    }
    sort(E_vec.begin(), E_vec.end());
    E_vec.erase(unique(E_vec.begin(), E_vec.end()), E_vec.end());

    for (ll i : E_vec) {
        int g = 2;
        for (int k = 60; k >= 2; --k) {
            ll root = round(pow(i, 1.0 / k));
            __int128_t p = 1;
            for (int j = 0; j < k; ++j) p *= root;
            if (p == i) { g = k; break; }
        }
        ll S_i = i - 1 + (fast_isqrt(i) - 1) + calc_f(i);
        int d = count_divisors(g);
        int start_k = (g % 2 == 0) ? 2 : 1;
        for (int k = start_k; k <= d - 1; ++k) {
            ll v = S_i - k;
            while (v > 0 && v % P == 0) { ans--; v /= P; }
        }
    }

    // 3. グループ1: 平方数抜けの高速カウント (区間分割と平方完成)
    vector<ll> pts;
    pts.push_back(2);
    ll max_x = fast_isqrt(N);
    pts.push_back(max_x + 1);
    for (int m = 3; m <= 60; ++m) {
        ll z_limit = round(pow(N, 1.0 / m)) + 2;
        for (ll z = 2; z <= z_limit; ++z) {
            __int128_t zm = 1; bool over = false;
            for (int i = 0; i < m; ++i) { zm *= z; if (zm > N) { over = true; break; } }
            if (!over) {
                ll x = ceil_sqrt((ll)zm);
                if (x <= max_x) pts.push_back(x);
            } else break;
        }
    }
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    for (size_t i = 0; i < pts.size() - 1; ++i) {
        ll A = pts[i];
        ll B = pts[i + 1] - 1;
        if (A > B) continue;

        ll H = calc_f(A * A);
        ll D = 9 - 4 * H;
        __int128_t max_V_val = (__int128_t)B * B + B + H - 2;

        if (P == 2) {
            if (H % 2 == 0) {
                __int128_t R1 = 0, R2 = 1;
                __int128_t cur_mod = 2;
                for (int step = 2; step <= 62; ++step) {
                    cur_mod *= 2;
                    if ((R1 * R1 + R1 + H - 2) % cur_mod != 0) R1 += cur_mod / 2;
                    if ((R2 * R2 + R2 + H - 2) % cur_mod != 0) R2 += cur_mod / 2;
                }
                __int128_t pk = 2;
                for (int k = 1; pk <= max_V_val; ++k, pk *= 2) {
                    ll r1 = (ll)(R1 % pk);
                    ll r2 = (ll)(R2 % pk);
                    ans -= safe_count_liner(A, B + 1, r1, pk);
                    if (r1 != r2) ans -= safe_count_liner(A, B + 1, r2, pk);
                }
            }
        } else {
            ll temp_D = D;
            int m = 0;
            while (temp_D != 0 && temp_D % P == 0) { m++; temp_D /= P; }
            ll D_prime = temp_D;

            int max_k = 0;
            __int128_t pk_temp = P;
            while (pk_temp <= max_V_val) { max_k++; pk_temp *= P; }

            // 事前に Cipolla と ヘンゼルリフトを最大次数まで1度だけ行う
            bool has_roots = false;
            __int128_t Z_full = 0;
            if (m % 2 == 0 && max_k > m) {
                ll Dp_mod = (D_prime % P + P) % P;
                ll z0 = cipolla(Dp_mod, P);
                if (z0 != -1) {
                    has_roots = true;
                    int max_K_need = max_k - m;
                    Z_full = z0;
                    if (max_K_need > 1) {
                        __int128_t y = z0;
                        __int128_t p_prev = P;
                        __int128_t inv2 = inv((2 * z0) % P, P);
                        for (int exp = 2; exp <= max_K_need; ++exp) {
                            __int128_t f = y * y - D_prime;
                            __int128_t term = (f / p_prev) % P;
                            if (term < 0) term += P;
                            __int128_t t = ((P - term) * inv2) % P;
                            y = y + t * p_prev;
                            p_prev *= P;
                        }
                        Z_full = y;
                    }
                }
            }

            __int128_t pk = P;
            for (int k = 1; k <= max_k; ++k, pk *= P) {
                if (m >= k) {
                    // Y = 0 mod P^{ceil(k/2)}
                    int ceil_k2 = (k + 1) / 2;
                    __int128_t M = 1; for (int j = 0; j < ceil_k2; ++j) M *= P;
                    __int128_t inv2_M = inv(2, M);
                    __int128_t x0 = ((M - 1) * inv2_M) % M;
                    ans -= safe_count_liner(A, B + 1, x0, M);
                } else if (has_roots) {
                    // 分岐なしのパターンAによる O(1) カウント
                    int K_need = k - m;
                    __int128_t P_Kneed = 1; for (int j = 0; j < K_need; ++j) P_Kneed *= P;
                    __int128_t Z1 = Z_full % P_Kneed;
                    __int128_t Z2 = (P_Kneed - Z1) % P_Kneed;

                    __int128_t P_m2 = 1; for (int j = 0; j < m / 2; ++j) P_m2 *= P;
                    __int128_t M = P_Kneed * P_m2;

                    __int128_t Y1 = (Z1 * P_m2) % M;
                    __int128_t Y2 = (Z2 * P_m2) % M;

                    __int128_t inv2_M = inv(2, M);
                    __int128_t x1 = ((Y1 - 1 + M) % M * inv2_M) % M;
                    __int128_t x2 = ((Y2 - 1 + M) % M * inv2_M) % M;

                    ans -= safe_count_liner(A, B + 1, x1, M);
                    if (x1 != x2) ans -= safe_count_liner(A, B + 1, x2, M);
                }
            }
        }
    }

    return ans;
}


unsigned long long ilog_k(const unsigned long long x, const int k) { // k^ilog_k(x) <= x < k^(ilog_k(x)+1)
    assert(k >= 2 && x >= 1);
    if(x < k) return 0;
    unsigned long long res = 0;
    unsigned long long power = 1;
    while(power < x/k) {
        power *= k;
        res++;
    }
    return res;
}

// template <int prime>
ll Prod_of_Sum_of_Log_test(ll N, ll prime){

    if (N <= 1){ return 0;}
    ll ans = 0;
    for (int i=2;i<=N;i++){
        ll tmp = 1;
        for (int j=2;j<=i;j++){
            tmp += ilog_k(i, j);
        }
        while (tmp % prime == 0){
            tmp /= prime;
            ans++;
        }
    }
    return ans;
}

ll powll(ll a, ll n, ll m){
    if (n == 0){return 1;}
    if (n == 1){return a % m;}
    __int128_t ans = 1;
    __int128_t p = a;
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

bool MillerRabin(ll N, vector<ll> arr){
    int s = bit_length((N-1)&(-(N-1)))-1;
    ll d = (N-1)>>s;

    for (auto a:arr){
        if (N <= a){return true;}
        ll x = powll(a,d,N);
        int t;
        if (x != 1){
            for (t = 0; t < s; ++t) {
                if (x == N - 1) break;
                x = __int128_t(x) * x % N;
            }
            if (t == s) return false;
        }
    }
    return true;
}

bool isprime(ll N){
    if (N <= 1){return false;}
    if (N == 2){return true;}
    if (N % 2 == 0){return false;}
    if (N < 4759123141LL){
        return MillerRabin(N, {2, 7, 61});
    } else {
        return MillerRabin(N,{2, 325, 9375, 28178, 450775, 9780504, 1795265022});
    }
}



int main(){
    // 愚直解との比較
    for (int P = 2; P <= 10; ++P) {
        if (!isprime(P)) continue;
        for (int N = 2; N <= 1000; ++N) {
            ll res1 = Prod_of_Sum_of_Log(N, P);
            ll res2 = Prod_of_Sum_of_Log_test(N, P);
            if (res1 != res2) {
                cout << "Mismatch at N=" << N << ": " << res1 << " vs " << res2 << endl;
            } else {
                cout << "N=" << N << ": " << res1 << " (OK)" << endl;
            }
        }
    }

}