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


template <typename T>
struct SegmentTree{
    int _size;
    int N;
    vector<T> array;
    function<T(T,T)> op;
    function<T()> e;

    SegmentTree() : _size(0), N(0), array(), op([](T a, T){ return a; }), e([](){ return T(); }) {}

    SegmentTree(vector<T> _array,function<T(T,T)> _op, function<T()> _e){
        this->_size = _array.size();
        this->op = _op;
        this->e = _e;
        this->N = 1;
        while (this->N < this->_size) this->N <<= 1;
        this->array = vector<T>(2 * this->N, this->e());
        for (int i = 0;i<_size;i++){
            this->array[i+N] = _array[i];
        }
        for (int i = N - 1; i > 0; i--){
            this->array[i] = _op(this->array[2*i],this->array[2*i+1]);
        }
    }

    T get(int i){
        return array[this->N + i];
    }

    void write(int i, T value){
        assert(0 <= i && i < _size);
        int ii = i + N;
        array[ii] = value;
        while (ii > 1){
            array[ii>>1] = op(array[ii&(-2)],array[ii|1]);
            ii >>= 1;
        }
    }

    T prod(int l,int r){
        assert(0 <= l && l <= r && r <= _size);

        T resL = e();
        T resR = e();
        int li = l + N;
        int ri = r + N;
        while (li < ri){
            if (li&1){
                resL = op(resL, array[li]);
                li += 1;
            }
            if (ri&1){
                ri -= 1;
                resR = op(array[ri], resR);
            }
            li >>= 1;
            ri >>= 1;
        }
        return op(resL, resR);
    }

    size_t size(){
        return this->_size;
    }
};


struct FullyIndexableDictionary{
    using u64 = unsigned long long;
    using u32 = unsigned int;

    size_t sz,count;  // szはビット数、countはブロック数
    bool is_builded;

    vector<u64> data;  // 64bit区切りでデータを持っておく
    vector<u32> BT;  // ブロックテーブル、各ブロックの先頭の rank を前計算する。
    int popcount;

    FullyIndexableDictionary() : sz(0), count(0), is_builded(false), data(), BT(), popcount(0) {}

    FullyIndexableDictionary(size_t sz):is_builded(false), sz(sz), count(sz/64+1), data(sz/64+1,0), BT(sz/64+1,0){};

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
        for (int i=0; i<count ;i++){
            BT[i] =  rank_all;
            rank_all += __builtin_popcountll(data[i]);
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

    inline int rank1(int n){
        return BT[n/64] + __builtin_popcountll((data[n/64] & ((((u64)1)<<(n&63))-((u64)1))));
    }

    int select1(int n){  // 二分探索、rank(k,1) <= n となる最大の k を返す
        int ok_b = 0;
        int ng_b = count;
        int mid_b;
        while (ng_b - ok_b > 1){
            mid_b = (ok_b+ng_b)/2;
            if (BT[mid_b] <= n){
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
            if (BT[ok_b] + __builtin_popcountll((data[ok_b] & ((((u64)1)<<(mid_d))-((u64)1)))) <= n){
                ok_d = mid_d;
            } else {
                ng_d = mid_d;
            }
        }

        return 64*ok_b + ok_d;
    }

    int select0(int n){  // 二分探索、rank(k,0) <= n となる最大の k を返す
        int ok_b = 0;
        int ng_b = count;
        int mid_b;
        while (ng_b - ok_b > 1){
            mid_b = (ok_b+ng_b)/2;
            if ((64*mid_b) - (BT[mid_b]) <= n){
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
            if ((64*ok_b + mid_d) - (BT[ok_b] + __builtin_popcountll((data[ok_b] & ((((u64)1)<<(mid_d))-((u64)1))))) <= n){
                ok_d = mid_d;
            } else {
                ng_d = mid_d;
            }
        }

        return 64*ok_b + ok_d;
    }

};


template <typename K, typename V>
struct WaveletMatrixSegmentTree{
    // このテンプレート関数に依存
    unsigned int bit_length(int n){ return n > 0 ? 32 - __builtin_clz(n) : 0;}
    unsigned int bit_length(ll n){ return n > 0 ? 64 - __builtin_clzll(n) : 0;}

    size_t sz, degits;
    vector<FullyIndexableDictionary> data;
    vector<SegmentTree<V>> segs;

    WaveletMatrixSegmentTree(vector<pair<K,V>> array, function<V(V,V)> op, function<V()> e) : sz(array.size()){
        K M = 1;
        for (int i = 0;i<sz;i++){
            if (M < array[i].first){
                M = array[i].first;
            }
        }
        degits = bit_length(M);
        data.resize(degits);
        segs.resize(degits);
        int count0,count1;
        vector<pair<K,V>> tmp(sz);
        vector<V> segtmp(sz);
        for (int d = degits-1;d>=0;d--){
            data[d] = FullyIndexableDictionary(sz);
            for (int i=0;i<sz;i++){
                data[d].set(i,(array[i].first>>d)&1);
            }
            data[d].build();
            int offset = data[d].rank_all(0);
            count0 = 0; count1 = 0;
            for(int i=0;i<sz;i++){
                if (((array[i].first>>d)&1)==0){
                    tmp[count0] = array[i];
                    segtmp[count0] = array[i].second;
                    count0 += 1; 
                } else {
                    tmp[offset+count1] = array[i];
                    segtmp[offset+count1] = array[i].second;
                    count1 += 1;
                }
            }
            segs[d] = SegmentTree<V>(segtmp, op, e);
            swap(array, tmp);
        }
    }

    K keys(int i){ // 元の配列の i 番目のkeyを返す
        K res = 0;
        int c;
        for (int d=degits-1;d>=0;d--){
            c = data[d][i];
            res = res*2 + c;
            i = c*data[d].rank_all(0) + data[d].rank(i,c);
        }
        return res;
    }

    V values(int i){ // 元の配列の i 番目のvalueを返す
        int d = degits-1;
        int c = data[d][i];
        return segs[degits-1].get(c*data[d].rank_all(0) + data[d].rank(i,c));
    }


    int count_key(int l, int r, K x){ // index区間[l,r) の中に何個のキーが c があるかを返す
        assert(0 <= l && l <= r && r <= sz);
        if (x < 0 || ((K)1<<degits) <= x){return 0;}
        int c;
        for (int d=degits-1;d >= 0;d--){
            c = (x>>d)&1;
            r = c*data[d].rank_all(0) + data[d].rank(r,c);
            l = c*data[d].rank_all(0) + data[d].rank(l,c);
        }
        return r-l;
    }

    inline int rank_key(int i, K c){ // 区間[0,i) の中に何個のキーが c があるかを返す
        return count_key(0,i,c);
    }

    int select(int i, K x){  // 全体の中で、i番目のキーが x のインデックスを返す
        assert(0 <= i && i < sz);
        if (x < 0 || (((K)1)<<degits) <= x){return -1;}
        int l = 0;
        int r = sz;
        int c;
        for (int d=degits-1;d >= 0;d--){
            c = (x>>d)&1;
            r = c*data[d].rank_all(0) + data[d].rank(r,c);
            l = c*data[d].rank_all(0) + data[d].rank(l,c);
        }

        if (r-l <= i){
            return -1;
        }

        i += l;
        for (int d=0;d < degits;d++){
            c = (x>>d)&1;
            // i = c*data[d].rank_all(0) + data[d].rank(j,c);
            // ↑ を j について解く必要がある
            // rank(j, c) = i - c * rank_all(0)
            // j = select(i - c * rank_all(0), c) でいい？
            i =  data[d].select(i - c * data[d].rank_all(0), c);
        }
        return i;
    }

    K range_kth_min(int l, int r, int i){  // 区間[l,r)に含まれる中で i 番目に小さいキーを返す
        assert(0 <= l && l <= r && r <= sz);
        if (i < 0 || r-l <= i){
            return -1;
        }
        int c;
        K res = 0;
        for (int d = degits-1;d >= 0;d--){
            c = data[d].rank(r,0) - data[d].rank(l,0);
            if (i < c){
                res = 2*res;
                r = data[d].rank(r,0);
                l = data[d].rank(l,0);
            } else {
                res = 2*res+1;
                i -= c;
                r = data[d].rank_all(0) + data[d].rank(r,1);
                l = data[d].rank_all(0) + data[d].rank(l,1);
            }
        }
        return res;
    }

    inline K range_kth_max(int l, int r, int i){ // 区間[l,r)に含まれる中で i 番目に大きいものを返す
        return range_kth_min(l,r,(r-l)-i-1);
    }


    int range_freq(int l,int r, K x, K y){ // 区間[l,r)、キーの範囲[x,y) であるようなものの個数を返す
        assert(0 <= l && l <= r && r <= sz);
        x = max(x,(K)0);
        y = min(y,((K)1)<<degits);
        if (y <= x){return 0;}
        
        int c;
        int ans = 0;
        int p = bit_length(x^y)-1;
        for (int d=degits-1;d > p;d--){
            c = (x>>d)&1;
            r = c*data[d].rank_all(0) + data[d].rank(r,c);
            l = c*data[d].rank_all(0) + data[d].rank(l,c);
        }

        int l0,l1,r0,r1;
        if (p != degits){
            r0 = data[p].rank(r,0);
            l0 = data[p].rank(l,0);
            r1 = data[p].rank_all(0) + data[p].rank(r,1);
            l1 = data[p].rank_all(0) + data[p].rank(l,1);
        } else {
            r0 = r;
            l0 = l;
            r1 = sz;
            l1 = sz;
        }

        for (int d=p-1;d >= 0;d--){
            c = (y>>d)&1;
            if (c == 0){
                // pass
            } else { // c == 1
                ans += data[d].rank(r1,0) - data[d].rank(l1,0);
            }
            r1 = c*data[d].rank_all(0) + data[d].rank(r1,c);
            l1 = c*data[d].rank_all(0) + data[d].rank(l1,c);
        }

        for (int d=p-1;d >= 0;d--){
            c = (x>>d)&1;
            if (c == 0){
                ans += data[d].rank(r0,1) - data[d].rank(l0,1);
            }
            r0 = c*data[d].rank_all(0) + data[d].rank(r0,c);
            l0 = c*data[d].rank_all(0) + data[d].rank(l0,c);
        }

        ans += (r0 - l0);
        return ans;
    }

    V range_prod(int l,int r, K x, K y){ // 区間[l,r)、キーの範囲[x,y) であるようなもののvalueの総和を返す
        assert(0 <= l && l <= r && r <= sz);
        x = max(x,(K)0);
        y = min(y,((K)1)<<degits);

        auto e = segs[0].e;
        auto op = segs[0].op;
        if (y <= x){return e();}
        
        int c;
        V ans = e();
        int p = bit_length(x^y)-1;
        for (int d=degits-1;d > p;d--){
            c = (x>>d)&1;
            r = c*data[d].rank_all(0) + data[d].rank(r,c);
            l = c*data[d].rank_all(0) + data[d].rank(l,c);
        }

        int l0,l1,r0,r1,offset;
        if (p != degits){
            r0 = data[p].rank(r,0);
            l0 = data[p].rank(l,0);
            r1 = data[p].rank_all(0) + data[p].rank(r,1);
            l1 = data[p].rank_all(0) + data[p].rank(l,1);
        } else {
            r0 = r;
            l0 = l;
            r1 = sz;
            l1 = sz;
        }

        for (int d=p-1;d >= 0;d--){
            c = (y>>d)&1;
            if (c == 1){ // c == 1
                ans = op(ans, segs[d].prod(data[d].rank(l1,0), data[d].rank(r1,0)));
            }
            r1 = c*data[d].rank_all(0) + data[d].rank(r1,c);
            l1 = c*data[d].rank_all(0) + data[d].rank(l1,c);
        }

        for (int d=p-1;d >= 0;d--){
            c = (x>>d)&1;
            if (c == 0){
                offset = data[d].rank_all(0);
                ans = op(ans, segs[d].prod(offset+data[d].rank(l0,1), offset+data[d].rank(r0,1)));
            }
            r0 = c*data[d].rank_all(0) + data[d].rank(r0,c);
            l0 = c*data[d].rank_all(0) + data[d].rank(l0,c);
        }

        ans = op(ans, segs[0].prod(l0, r0));
        return ans;
    }

    void updata(int ind, V x){
        int c;
        for (int d=degits-1;d>=0;d--){
            c = data[d][ind];
            ind = c*data[d].rank_all(0) + data[d].rank(ind,c);
            segs[d].write(ind,x);
        }
    }

    K successor(int l, int r, K v){ // 区間[l,r)に含まれる v 以上の最小の要素を返す。無ければ -1 を返す。
        return range_kth_min(l,r,range_freq(l,r,0,v));
    }

    K predecessor(int l, int r, K v){ // 区間[l,r)に含まれる v 未満の最小の要素を返す。無ければ -1 を返す。
        return range_kth_max(l,r,range_freq(l,r,v,((K)1)<<degits));
    }

    size_t size(){
        return sz;
    }

};



// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    cin >> N;
    vector<pair<int,mint>> A(N);
    int a;
    rep(i,N){
        cin >> a;
        A[i] = {a,mint(2).pow(i)};
    }

    WaveletMatrixSegmentTree<int,mint> WMST(A,[](mint a,mint b){return a+b;}, [](){return mint(0);});

    mint ans = 0;
    rep(i,N){
        ans += WMST.range_prod(i+1,N,A[i].first,pow10ll[10]+1)/mint(2).pow(i+1);
    }

    cout << ans << "\n";
}


/*
int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);


    vector<pair<ll, mint>> arr(4);
    arr[0] = {1LL, mint(1)};
    arr[1] = {2LL, mint(4)};
    arr[2] = {3LL, mint(9)};
    arr[3] = {4LL, mint(16)};
    WaveletMatrixSegmentTree WMST(arr, [](mint a, mint b){ return a + b; }, [](){ return mint(0); });

    cout << WMST.range_prod(0,1,1,1) << endl;
    cout << WMST.range_prod(0,1,1,2) << endl;
    cout << WMST.range_prod(0,1,1,3) << endl;
    cout << WMST.range_prod(0,1,1,4) << endl;
    cout << WMST.range_prod(0,1,1,5) << endl;
    cout << WMST.range_prod(0,1,2,5) << endl;
    cout << WMST.range_prod(0,1,3,5) << endl;
    cout << WMST.range_prod(0,1,4,5) << endl;
    cout << WMST.range_prod(0,1,5,5) << endl;

    cout << WMST.range_prod(0,2,1,1) << endl;
    cout << WMST.range_prod(0,2,1,2) << endl;
    cout << WMST.range_prod(0,2,1,3) << endl;
    cout << WMST.range_prod(0,2,1,4) << endl;
    cout << WMST.range_prod(0,2,1,5) << endl;
    cout << WMST.range_prod(0,2,2,5) << endl;
    cout << WMST.range_prod(0,2,3,5) << endl;
    cout << WMST.range_prod(0,2,4,5) << endl;
    cout << WMST.range_prod(0,2,5,5) << endl;

    cout << endl;
    WMST.updata(0,100);

    cout << WMST.range_prod(0,1,1,1) << endl;
    cout << WMST.range_prod(0,1,1,2) << endl;
    cout << WMST.range_prod(0,1,1,3) << endl;
    cout << WMST.range_prod(0,1,1,4) << endl;
    cout << WMST.range_prod(0,1,1,5) << endl;
    cout << WMST.range_prod(0,1,2,5) << endl;
    cout << WMST.range_prod(0,1,3,5) << endl;
    cout << WMST.range_prod(0,1,4,5) << endl;
    cout << WMST.range_prod(0,1,5,5) << endl;

    cout << WMST.range_prod(0,2,1,1) << endl;
    cout << WMST.range_prod(0,2,1,2) << endl;
    cout << WMST.range_prod(0,2,1,3) << endl;
    cout << WMST.range_prod(0,2,1,4) << endl;
    cout << WMST.range_prod(0,2,1,5) << endl;
    cout << WMST.range_prod(0,2,2,5) << endl;
    cout << WMST.range_prod(0,2,3,5) << endl;
    cout << WMST.range_prod(0,2,4,5) << endl;
    cout << WMST.range_prod(0,2,5,5) << endl;
    
}
*/
