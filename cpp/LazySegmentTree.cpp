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

// ===============================================================================
  

template <typename info, typename func>
struct LazySegmentTree{

    vector<info> data;
    vector<func> lazy; 
    function<info(info,info)> op;
    function<info()> e;
    function<func(func,func)> composition;
    function<info(func,info)> mapping;
    function<func()> id;
    size_t sz;
    int N;
    int logN;


    LazySegmentTree(vector<info>& array,
            function<info(info,info)> op,
            function<info()> e,
            function<func(func,func)> composition,
            function<info(func,info)> mapping,
            function<func()> id) 
            : op(op), e(e), composition(composition), mapping(mapping), id(id), sz(array.size()) {
                logN = bit_length(sz-1);
                N = 1 << logN;
                data.assign(2*N, e());
                lazy.assign(2*N, id());

                for (int i = 0;i<sz;i++){
                    this->data[i+N] = array[i];
                }
                for (int i = N - 1; i > 0; i--){
                    this->data[i] = op(this->data[2*i],this->data[2*i+1]);
                }
            }

            info get(int i){
                assert(0 <= i && i <= sz);
                int ii= i + N;
                line_propagate(i);
                return data[ii];
            }

            info prod(int l,int r){
                assert(0 <= l && l <= r && r <= sz);
                line_propagate(l);
                line_propagate(r);
                info resL = e();
                info resR = e();
                int li = l + N;
                int ri = r + N;
                while (li < ri){
                    if (li&1){ // 必ず奇数ノードしか取らない
                        resL = op(resL, data[li]);
                        li += 1;
                    }
                    if (ri&1){ // 必ず偶数ノードしか取らない
                        ri -= 1;
                        resR = op(data[ri], resR);
                    }
                    li >>= 1;
                    ri >>= 1;
                }
                return op(resL, resR);
            }

            inline info prod_all(){
                return data[1];
            }
            
            void apply(int l,int r, func f){
                assert(0 <= l && l <= r && r <= sz);
                // 必要な遅延の解消
                line_propagate(l);
                line_propagate(r);

                // 遅延情報の書き込み
                int li = l + N;
                int ri = r + N;
                while (li < ri){
                    if (li&1){ // 必ず奇数ノードしか取らない
                        data[li] = mapping(f,data[li]);
                        lazy[li] = composition(f,lazy[li]);
                        li += 1;
                    }
                    if (ri&1){ // 必ず偶数ノードしか取らない
                        ri -= 1;
                        data[ri] = mapping(f,data[ri]);
                        lazy[ri] = composition(f,lazy[ri]);
                    }
                    li >>= 1;
                    ri >>= 1;
                }


                // 変更箇所の更新
                li = l + N;
                ri = r + N;
                li /= li&(-li);
                ri /= ri&(-ri);
                while (li > 1){
                    data[li>>1] = op(data[li&(-2)],data[li|1]);
                    li >>= 1;
                }
                while (ri > 1){
                    data[ri>>1] = op(data[ri&(-2)],data[ri|1]);
                    ri >>= 1;
                }
            }


            void point_propagate(int i){ // i番目のノードの遅延情報を 2*i, 2*i+1 番目のノードに伝える
                lazy[2*i] = composition(lazy[i],lazy[2*i]);
                lazy[2*i+1] = composition(lazy[i],lazy[2*i+1]);
                data[2*i] = mapping(lazy[i],data[2*i]);
                data[2*i+1] = mapping(lazy[i],data[2*i+1]);
                lazy[i] = id();
            }

            void line_propagate(int ind){ // index i のノード以上の部分の遅延を解消する
                if (ind != sz){
                    ind += N;
                    for (int i=logN;i>0;i--){
                        point_propagate(ind>>i);
                    }
                }
            }

            void all_propagate(){ // 全ての遅延を解消する
                for (int i = 1;i<N;i++){
                    point_propagate(i);
                }
            }
            
            void write(int i, info x){
                line_propagate(i);
                i += N;
                data[i] = x;
                while(i > 0){
                    data[i>>1] = op(data[i&(-2)],data[i|1]);
                    i >>= 1;
                }
            }

};

// RUQ + RSQ on ll
namespace RSQ_RUQ{
    struct S
    {
        ll value;
        ll length;
    };

    struct F{
        ll num;
        bool iswrited;
    };

    S op(S a,S b){
        return {a.value + b.value,a.length + b.length};
    }

    F comp(F f, F g){  // f○gを返す
        if (f.iswrited){
            return {f.num,true};
        } else if (g.iswrited){
            return {g.num,true};
        } else {
            return {0,false};
        }
    }

    S mapping(F f, S x){
        if (f.iswrited){
            return {f.num*x.length, x.length};
        } else {
            return {x.value,x.length};
        }
    }

    S e(){
        return {0,0};
    }

    F id(){
        return {0,false};
    }
}

// Affine on ll
namespace Affinell{
    ll infinity = (1LL<<51);

    struct S{
        ll sum;
        ll min;
        ll max;
        ll length;
    };

    struct F{
        ll a;
        ll b;
    };

    S op(S x,S y){
        return {x.sum + y.sum, min(x.min, y.min), max(x.max, y.max), x.length + y.length};
    }

    S mapping(F f, S x){
        if (f.a >= 0){
            return {f.a*x.sum+f.b*x.length ,f.a*x.min+f.b ,f.a*x.max+f.b, x.length};
        } else {
            return {f.a*x.sum+f.b*x.length ,f.a*x.max+f.b ,f.a*x.min+f.b ,x.length};
        }
    }

    F comp(F f, F g){  // a(cx+d)+b = 
        return {f.a*g.a, f.a*g.b+f.b};
    }

    S e(){
        return {0,infinity,-infinity,0};
    }

    F id(){
        return {1,0};
    }
}

// Affine on mint
namespace Affinemint{
    struct S{
        int sum;
        int length;
    };

    struct F{
        int a;
        int b;
    };

    S op(S x,S y){
        return {(x.sum + y.sum)%998244353, x.length + y.length};
    }

    S mapping(F f, S x){
        return {(int)((((ll)f.a)*((ll)x.sum)+((ll)f.b)*((ll)x.length))%998244353) ,x.length};
    }

    F comp(F f, F g){  // a(cx+d)+b = ac x + ad+b
        return {(int)((((ll)f.a)*((ll)g.a))%998244353), (int)((((ll)f.a)*((ll)g.b)+((ll)f.b))%998244353)};
    }

    S e(){
        return {0,0};
    }

    F id(){
        return {1,0};
    }
}

// RangeAdd RangeDiv RangeMax Range Restore
namespace ADD_DIV_MAX_RESTORE{

    struct S{
        ll max;
        ll initial_max;
        ll min;
        ll initial_min;
    };

    struct F{
        ll offset;
        ll den;
        ll add;
        bool flag;

    };

    S op(S x, S y){
        return {max(x.max,y.max),max(x.initial_max,y.initial_max),min(x.min,y.min),min(x.initial_min,y.initial_min)};
    }

    S e(){
        return {0,0,0,0};
    }


    S mapping(F f,S x){
        if (f.flag){
            //cout << "Debug : " << x.max << " " << x.initial_max << " " << f.offset << " " << f.den << " " <<  f.add << " " << endl;
            return {(x.initial_max+f.offset)/f.den+f.add,x.initial_max,(x.initial_min+f.offset)/f.den+f.add,x.initial_min};
        } else {
            // cout << "Debug : " << x.max << " " << f.offset << " " << f.den << " " <<  f.add << " " << endl;
            return {(x.max+f.offset)/f.den+f.add,x.initial_max,(x.min+f.offset)/f.den+f.add,x.initial_min};
        }
    }

    F comp(F g, F f){
        ll A,B,C;
        if (g.flag){
            return {g.offset,g.den,g.add,true};
        } else {
            A = f.offset + f.den * ((f.add + g.offset) % g.den);
            B = f.den * g.den;
            C = g.add + (f.add + g.offset) / g.den;
            if (B > pow10ll[9]){
                A = max(A-(B-pow10ll[9]),0LL);
                B = pow10ll[9];
            }
            // cout << f.offset << " " << f.den << " " << f.add << " " << f.flag << " " << g.offset <<  " " << g.den << " " << g.add << " : " << A << " " << B << " " << C << "\n";
            return {A, B, C, f.flag};
        }
    }

    F id(){return {0,1,0,false};}

    S gen(ll a){
        return {a,a,a,a};
    }
}

// ===============================================================================


int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll N,Q;
    cin >> N >> Q;

    vector<ADD_DIV_MAX_RESTORE::S> A(N);
    ll a;
    rep(i,N){
        cin >> a;
        A[i] = {a,a,a,a};
    }
    
    LazySegmentTree<lsegtype(ADD_DIV_MAX_RESTORE)> ST(A,lsegarg(ADD_DIV_MAX_RESTORE));
    ll t,l,r,x;
    ADD_DIV_MAX_RESTORE::S ans;

    rep(i,Q){
        cin >> t >> l >> r >> x;
        if (t == 0){
            ST.apply(l,r+1,{0,1,x,0});
        } else if (t == 1){
            ST.apply(l,r+1,{0,x,0,0});
        } else if (t == 2){
            ans = ST.prod(l,r+1);
            cout << ans.max << endl;
        } else if (t == 3){
            ST.apply(l,r+1,{0,1,0,true});
        } else if (t == 4){
            ans = ST.prod(l,r+1);
            cout << ans.min << endl;
        }
    }

    return 0;
}

/*
int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll N,Q;
    cin >> N >> Q;

    vector<Affinemint::S> vec(N);
    int tmp;
    rep(i,N){
        cin >> tmp;
        vec[i] = {tmp,1};
    }
    LazySegmentTree<lsegtype(Affinemint)> LST(vec, lsegarg(Affinemint));

    int t,l,r,b,c;
    rep(i,Q){
        cin >> t;
        if (t == 0){
            cin >> l >> r >> b >> c;
            LST.apply(l,r,{b,c});
        } else {
            cin >> l >> r;
            cout << LST.prod(l,r).sum << "\n";
        }
    }
}
*/

/*
int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    vector<RSQ_RUQ::S> vec = {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1}};
    LazySegmentTree<lsegtype(RSQ_RUQ)> LST(vec, lsegarg(RSQ_RUQ));

    cout << LST.get(-7).value << endl;
    cout << endl;
    cout << LST.get(-6).value << endl;
    cout << LST.get(-5).value << endl;
    cout << endl;
    cout << LST.get(-4).value << endl;
    cout << LST.get(-3).value << endl;
    cout << LST.get(-2).value << endl;
    cout << LST.get(-1).value << endl;
    cout << endl;
    cout << LST.get(0).value << endl;
    cout << LST.get(1).value << endl;
    cout << LST.get(2).value << endl;
    cout << LST.get(3).value << endl;
    cout << LST.get(4).value << endl;
    cout << LST.get(5).value << endl;
    cout << LST.get(6).value << endl;
    cout << LST.get(7).value << endl;
    cout << endl;
    cout << endl;
    cout << LST.prod(0,3).value << endl;
    cout << LST.prod(1,4).value << endl;
    cout << LST.prod(2,5).value << endl;
    cout << endl;
    cout << endl;
    cout << LST.prod(0,3).value << endl;
    cout << LST.prod(1,4).value << endl;
    cout << LST.prod(2,5).value << endl;
    cout << endl;
    cout << endl;
    LST.apply(0,3,{100,true});
    cout << endl;
    cout << endl;
    cout << LST.get(0).value << endl;
    cout << LST.get(1).value << endl;
    cout << LST.get(2).value << endl;
    cout << LST.get(3).value << endl;
    cout << LST.get(4).value << endl;
    cout << LST.get(5).value << endl;
    cout << LST.get(6).value << endl;
    cout << LST.get(7).value << endl;
    cout << endl;
    LST.apply(4,6,{200,true});
    LST.apply(5,8,{300,true});
    cout << endl;
    cout << endl;
    cout << LST.get(0).value << endl;
    cout << LST.get(1).value << endl;
    cout << LST.get(2).value << endl;
    cout << LST.get(3).value << endl;
    cout << LST.get(4).value << endl;
    cout << LST.get(5).value << endl;
    cout << LST.get(6).value << endl;
    cout << LST.get(7).value << endl;


    
}

*/

