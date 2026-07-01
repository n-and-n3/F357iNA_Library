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
#include <compare>
#include <array>
#include <bitset>

using namespace std;

#define ll long long
#define LL __int128
#define ld long double
#define INF 2251799813685248
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define reps(i, l, r) for(ll i = (l); i < (r); ++i)
#define foreach(c, A) for(auto c:(A))
#define _vall(A) (A).begin(),(A).end()
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

template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &p){ os << "{" <<  p.first << ", " << p.second << "}"; return os;}


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

namespace n3{
    namespace array_ds{
        template <typename T>
        struct CumulativeTable
        {
            vector<T> array_sum;
            CumulativeTable(const vector<T>& array) : array_sum(array.size()+1, 0){
                for (int i=0;i<array.size();i++){
                    array_sum[i+1] = array_sum[i] + array[i];
                }
            }

            T prod(int l,int r){
                return array_sum[r] - array_sum[l];
            }
        };

        template <typename T>
        struct BIT{
            vector<T> array;
            size_t N;

            BIT(int N) : N(N), array(N+1,0){}
            BIT(vector<T> data) : N(data.size()) , array(data.size()+1){
                data.push_back(0);
                for (int i=N-1;i>=0;i--){
                    data[i] += data[i+1];
                }
                for (int i=1;i<=N;i++){
                    array[i] = data[i&(i-1)] - data[i];
                }
            }

            void add(int ind, T x){
                while (ind <= N){
                    array[ind] += x;
                    ind += ind & (-ind);
                }
            }

            T sum(int ind){
                T res = 0;
                while (ind > 0){
                    res += array[ind];
                    ind -= ind & (-ind);
                }
                return res;
            }

};

        // @brief TernaryIndexedTree
        template <typename T>
        struct TIT {
            vector<T> array;
            size_t sz;
            TIT(initializer_list<T> init_list){
                init(init_list.begin(), init_list.end());
            }

            TIT(const vector<T>& init_vec){
                init(init_vec.begin(), init_vec.end());
            }

        private:
            template <class It>
            void init(It first, It last){
                // 冒頭に0を挿入したうえで初期化
                array.clear();
                array.push_back(0);
                array.insert(array.end(), first, last); // 元の配列を代入

                // サイズを (3^k-1)/2 型 にする。3の累乗でもいいけど、平衡三進数的にはこれが自然。
                int tmp = 1;
                while (tmp < (int)array.size()) {
                    tmp = 3*tmp+1;
                }
                array.resize(tmp+1, 0); // 1-indexed なので +1 する
                sz = array.size() - 1;

                // テーブルの構築
                int pos = 2;
                int p = 1;
                while (pos < (int)array.size()) {
                    for (int i = pos; i <= (int)sz; i+=3*p) {
                        array[i] += array[i - p];
                        array[i] += array[i + p];
                    }
                    pos = 3 * pos - 1;
                    p *= 3;
                }
            }

        public:

            T cum(int ind){
                T res = 0;
                int p = 1;
                while (ind > 0){
                    if (ind % 3 == 1){
                        res += array[ind*p - (p-1)/2];
                        ind -= 1;
                    } else if (ind % 3 == 2){
                        ind += 1;
                        res -= array[ind*p - (p-1)/2];
                    }
                    ind /= 3;
                    p *= 3;
                }
                return res;
            }

            T get(int ind){
                return cum(ind) - cum(ind-1);
            }

            void add(int ind, T x){
                int p = 1;
                while (ind*p <= sz){
                    if (ind % 3 != 2){
                        array[ind*p - (p-1)/2] += x;
                    }
                    ind = (ind-1)/3+1;
                    p *= 3;
                }
            }

            void write(int ind, T x){
                add(ind, x - get(ind));
            }

            size_t size(){
                return sz;
            }


        };


        template <typename T>
        struct PartiallyPersistentArray {
            struct Data{
                vector<vector<pair<int,T>>> historys;
                Data(vector<T> array){
                    int N = array.size();
                    historys.resize(N); 
                    for (int i=0; i<N; i++){
                        this->historys[i] = {{0,array[i]},};
                    }
                }


                T get(int v, int ind){
                    int ok=0;
                    int ng=historys[ind].size();
                    int mid;
                    while (ng - ok > 1){
                        mid = (ok + ng)/2;
                        if (this->historys[ind][mid].first <= v){
                            ok = mid;
                        } else {
                            ng = mid;
                        }
                    }
                    return this->historys[ind][ok].second;
                }

                void write(int v, int ind, T x){
                    assert(this->historys[ind].back().first < v); // 最新版より大きい数値でないと、バージョンとして認めない。要するに二分探索がバグらないように単調性を要請する
                    if (this->historys[ind].back().second != x){
                        this->historys[ind].push_back({v,x});
                    }
                }

                T current(int ind){
                    return this->historys[ind].back().second;
                }
            };

            struct NodeRef {
                PartiallyPersistentArray<T>* st;
                int idx;

                NodeRef(PartiallyPersistentArray<T>* st, int idx) : st(st), idx(idx) {}

                NodeRef& operator=(const T& _val) {st->write(idx, _val); return *this;}
                NodeRef& operator=(const NodeRef& other) {st->write(idx, (T)other);return *this;}
                
                #define DEFINE_COMPOUND_OP(OP, BI_OP) \
                NodeRef& operator OP (const T& _val) {st->write(idx, st->get(idx) BI_OP _val); return *this;} \
                NodeRef& operator OP (const NodeRef& other) {st->write(idx, st->get(idx) BI_OP (T)other); return *this;} 

                DEFINE_COMPOUND_OP(+=, +)
                DEFINE_COMPOUND_OP(-=, -)
                DEFINE_COMPOUND_OP(*=, *)
                DEFINE_COMPOUND_OP(/=, /)
                DEFINE_COMPOUND_OP(%=, %)
                DEFINE_COMPOUND_OP(|=, |)
                DEFINE_COMPOUND_OP(&=, &)
                DEFINE_COMPOUND_OP(^=, ^)
                DEFINE_COMPOUND_OP(<<=, <<)
                DEFINE_COMPOUND_OP(>>=, >>)

                #undef DEFINE_COMPOUND_OP
                
                NodeRef& operator++() {st->write(idx, st->get(idx) + 1);return *this;}
                T operator++(int) {T old = st->get(idx);st->write(idx, old + 1);return old;}
                NodeRef& operator--() {st->write(idx, st->get(idx) - 1);return *this;}
                T operator--(int) {T old = st->get(idx);st->write(idx, old - 1);return old;}

                operator T() const {return st->get(idx);}
            };

            NodeRef operator[](int i) {
                return NodeRef(this, i);
            }

        // --- 内部構造体 iterator の定義 ---
            struct iterator {
                using iterator_category = std::random_access_iterator_tag;
                using _value_type        = T;
                using difference_type   = ptrdiff_t;
                using pointer           = T*;
                using reference         = NodeRef; // 代入を可能にするため NodeRef を返す

                PartiallyPersistentArray<T>* ppa;
                int idx;

                iterator(PartiallyPersistentArray<T>* ppa, int idx) : ppa(ppa), idx(idx) {}

                // 間接参照
                NodeRef operator*() const { return (*ppa)[idx]; }
                // ※ポインタアクセス(->)は T が構造体の場合に複雑になるため、必要に応じて定義
                
                // 算術演算
                iterator& operator++() { idx++; return *this; }
                iterator operator++(int) { iterator temp = *this; idx++; return temp; }
                iterator& operator--() { idx--; return *this; }
                iterator operator--(int) { iterator temp = *this; idx--; return temp; }
                iterator& operator+=(difference_type n) { idx += n; return *this; }
                iterator& operator-=(difference_type n) { idx -= n; return *this; }
                iterator operator+(difference_type n) const { return iterator(ppa, idx + n); }
                iterator operator-(difference_type n) const { return iterator(ppa, idx - n); }
                difference_type operator-(const iterator& other) const { return idx - other.idx; }

                // 比較演算
                bool operator==(const iterator& other) const { return idx == other.idx; }
                bool operator!=(const iterator& other) const { return idx != other.idx; }
                bool operator<(const iterator& other) const { return idx < other.idx; }
                bool operator<=(const iterator& other) const { return idx <= other.idx; }
                bool operator>(const iterator& other) const { return idx > other.idx; }
                bool operator>=(const iterator& other) const { return idx >= other.idx; }

                // インデックスアクセス
                NodeRef operator[](difference_type n) const { return (*ppa)[idx + n]; }
            };

            // --- begin / end メソッド ---
            iterator begin() { return iterator(this, 0); }
            iterator end() { return iterator(this, sz); }


            Data* data;
            unsigned int version;
            bool is_original;
            size_t sz;

            // 通常定義用 1
            PartiallyPersistentArray(vector<T> array){
                Data* new_data = new Data{array};
                this->data = new_data;
                this->is_original = true;
                this->version = 0;
                this->sz = this->data->historys.size();
            }

            // 通常定義用 2
            PartiallyPersistentArray(int N){
                Data* new_data = new Data{vector<T>(N)};
                this->data = new_data;
                this->is_original = true;
                this->version = 0;
                this->sz = this->data->historys.size();
            }

            // 通常定義用 3
            PartiallyPersistentArray(int N, T initial){
                Data* new_data = new Data{vector<T>(N,initial)};
                this->data = new_data;
                this->is_original = true;
                this->version = 0;
                this->sz = this->data->historys.size();
            }

            // リスト初期化用
            PartiallyPersistentArray(initializer_list<T> init): PartiallyPersistentArray(vector<T>(init)) {}



            T get(int ind){
                assert(0 <= ind && ind < sz);
                if (this->is_original){
                    return data->current(ind);
                } else {
                    return data->get(this->version,ind);
                }
            }

            void write(int ind, T x){
                assert(0 <= ind && ind < sz);
                assert(this->is_original);
                this->version++;
                data->write(this->version,ind,x);
            }

            size_t size(){
                return sz;
            }

            PartiallyPersistentArray copy(){
                return PartiallyPersistentArray(this);
            }

            operator vector<T>(){
                vector<T> ans(sz);
                for (int i=0; i<sz;i++){
                    ans[i] = get(i);
                }
                return ans;
            }

            // コピー用
            private:
            PartiallyPersistentArray(const PartiallyPersistentArray* ppa){
                this->data = ppa->data;
                this->is_original = false;
                this->version = ppa->version;
                this->sz = ppa->data->historys.size();
            }
        };
            
        template <typename T>
        struct PersistentStack
        {
            struct Node {
                Node* parent;
                T _value;
            };

            Node* head = nullptr;
            size_t size_num = 0;
            
            PersistentStack(){}
            PersistentStack(const PersistentStack& ps) : head(ps.head), size_num(ps.size_num){}

            void push(T x){
                Node* new_node = new Node{ this->head, x };
                this->head = new_node;
                this->size_num += 1;
            }

            T top(){
                assert(!this->empty());
                return this->head->_value;
            }

            T top_or(const T& v) {
                if(this->empty()) return v;
                return this->top();
            }

            void pop(){
                assert(!this->empty());
                this->head = this->head->parent;
                this->size_num -= 1;
            }

            void pop_safe(){
                if (!this->empty()){
                    this->head = this->head->parent;
                    this->size_num -= 1;
                }
            }

            bool empty(){
                return head == nullptr;
            }

            size_t size(){
                return this->size_num;
            }

            PersistentStack clone(){
                return *this;
            }

            operator vector<T>(){
                auto pt = this->clone();
                vector<T> memo(pt.size_num);
                for (int i = this->size_num-1; 0 <= i; i--){
                    memo[i] = pt.top();
                    pt.pop();
                }
                return memo;
            }

        };

        template <typename T>
        struct SegmentTree{
            int _size;
            int N;
            vector<T> array;
            function<T(T,T)> op;
            function<T()> e;

            SegmentTree() = default;
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

            struct NodeRef {
                SegmentTree<T>* st;
                int idx;

                NodeRef(SegmentTree<T>* st, int idx) : st(st), idx(idx) {}

                NodeRef& operator=(const T& _val) {st->write(idx, _val); return *this;}
                NodeRef& operator=(const NodeRef& other) {st->write(idx, (T)other);return *this;}
                
                #define DEFINE_COMPOUND_OP(OP, BI_OP) \
                NodeRef& operator OP (const T& _val) {st->write(idx, st->get(idx) BI_OP _val); return *this;} \
                NodeRef& operator OP (const NodeRef& other) {st->write(idx, st->get(idx) BI_OP (T)other); return *this;} 

                DEFINE_COMPOUND_OP(+=, +)
                DEFINE_COMPOUND_OP(-=, -)
                DEFINE_COMPOUND_OP(*=, *)
                DEFINE_COMPOUND_OP(/=, /)
                DEFINE_COMPOUND_OP(%=, %)
                DEFINE_COMPOUND_OP(|=, |)
                DEFINE_COMPOUND_OP(&=, &)
                DEFINE_COMPOUND_OP(^=, ^)
                DEFINE_COMPOUND_OP(<<=, <<)
                DEFINE_COMPOUND_OP(>>=, >>)

                #undef DEFINE_COMPOUND_OP
                
                NodeRef& operator++() {st->write(idx, st->get(idx) + 1);return *this;}
                T operator++(int) {T old = st->get(idx);st->write(idx, old + 1);return old;}
                NodeRef& operator--() {st->write(idx, st->get(idx) - 1);return *this;}
                T operator--(int) {T old = st->get(idx);st->write(idx, old - 1);return old;}

                operator T() const {return st->get(idx);}
            };

            NodeRef operator[](int i) {
                return NodeRef(this, i);
            }

            T get(int i){
                return array[this->N + i];
            }

            void write(int i, T _value){
                assert(0 <= i && i < _size);
                int ii = i + N;
                array[ii] = _value;
                while (ii > 1){
                    array[ii>>1] = op(array[ii&(-2)],array[ii|1]);
                    ii >>= 1;
                }
            }

            void inplace_op(int i, T _value){
                write(i, op(get(i) ,_value));
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


            LazySegmentTree() = default;
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

        };

        struct FullyIndexableDictionary{
            using u64 = unsigned long long;
            using u32 = unsigned int;

            size_t sz,count;  // szはビット数、countはブロック数
            bool is_builded;

            vector<u64> data;  // 64bit区切りでデータを持っておく
            vector<u32> BT;  // ブロックテーブル、各ブロックの先頭の rank を前計算する。
            int popcount;

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

        template <typename T>
        struct WaveletMatrix{

            size_t sz, degits;
            vector<FullyIndexableDictionary> data;

            WaveletMatrix(vector<T> array) : sz(array.size()), degits(bit_length(*max_element(array.begin(),array.end()))) , data(bit_length(*max_element(array.begin(),array.end()))){
                int count0,count1;
                vector<T> tmp(sz);
                for (int d = degits-1;d>=0;d--){
                    for (int i=0;i<sz;i++){
                        data[d].set(i,(array[i]>>d)&1);
                    }
                    data[d].build();
                    int offset = data[d].rank_all(0);
                    count0 = 0; count1 = 0;
                    for(int i=0;i<sz;i++){
                        if (((array[i]>>d)&1)==0){
                            tmp[count0] = array[i];
                            count0 += 1; 
                        } else {
                            tmp[offset+count1] = array[i];
                            count1 += 1;
                        }
                    }
                    swap(array, tmp);
                }
            }

            T access(int i){ // 元の配列の i 番目の要素を返す
                T res = 0;
                int c;
                for (int d=degits-1;d>=0;d--){
                    c = data[d][i];
                    res = res*2 + c;
                    i = c*data[d].rank_all(0) + data[d].rank(i,c);
                }
                return res;
            }

            int count(int l, int r, T x){ // 区間[l,r) の中に何個のcがあるかを返す
                assert(0 <= l && l <= r && r <= sz);
                if (x < 0 || ((T)1<<degits) <= x){return 0;}
                int c;
                for (int d=degits-1;d >= 0;d--){
                    c = (x>>d)&1;
                    r = c*data[d].rank_all(0) + data[d].rank(r,c);
                    l = c*data[d].rank_all(0) + data[d].rank(l,c);
                }
                return r-l;
            }

            inline int rank(int i, T c){ // 区間[0,i) の中に何個のcがあるかを返す
                return count(0,i,c);
            }

            int select(int i, T x){  // 全体の中で、i番目の x のインデックスを返す
                assert(0 <= i && i < sz);
                if (x < 0 || (((T)1)<<degits) <= x){return -1;}
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

            T range_kth_min(int l, int r, int i){  // 区間[l,r)に含まれる中で i 番目に小さいものを返す
                assert(0 <= l && l <= r && r <= sz);
                if (i < 0 || r-l <= i){
                    return -1;
                }
                int c;
                T res = 0;
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

            inline T range_kth_max(int l, int r, int i){ // 区間[l,r)に含まれる中で i 番目に大きいものを返す
                return range_kth_min(l,r,(r-l)-i-1);
            }

            int mex0(int l, int r){  // 配列の値が、0-indexedの置換のとき、区間[l,r)のmexを返す
                assert(0 <= l && l <= r && r <= sz);
                int c;
                T res = 0;
                for (int d = degits-1;d >= 0;d--){
                    c = data[d].rank(r,0) - data[d].rank(l,0);
                    if (c < (1<<d)){
                        res = 2*res;
                        r = data[d].rank(r,0);
                        l = data[d].rank(l,0);
                    } else {
                        res = 2*res+1;
                        r = data[d].rank_all(0) + data[d].rank(r,1);
                        l = data[d].rank_all(0) + data[d].rank(l,1);
                    }
                }
                return res; 
            }

            int mex1(int l, int r){  // 配列の値が、0-indexedの置換のとき、区間[l,r)のmexを返す
                assert(0 <= l && l <= r && r <= sz);
                return min(l != 0 ? range_kth_min(0,l,0) : sz, r != sz ? range_kth_min(r,sz,0) : sz);
            }

            int range_freq(int l,int r, T x, T y){ // 区間[l,r)、値の範囲[x,y) であるようなものの個数を返す
                assert(0 <= l && l <= r && r <= sz);
                x = max(x,(T)0);
                y = min(y,((T)1)<<degits);
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

            T successor(int l, int r, T v){ // 区間[l,r)に含まれる v 以上の最小の要素を返す。無ければ -1 を返す。
                return range_kth_min(l,r,range_freq(l,r,0,v));
            }

            T predecessor(int l, int r, T v){ // 区間[l,r)に含まれる v 未満の最小の要素を返す。無ければ -1 を返す。
                return range_kth_max(l,r,range_freq(l,r,v,((T)1)<<degits));
            }

            inline T operator[](int i) {
                return access(i);
            }

            size_t size(){
                return sz;
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

            V _values(int i){ // 元の配列の i 番目の_valueを返す
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

            V range_prod(int l,int r, K x, K y){ // 区間[l,r)、キーの範囲[x,y) であるようなものの_valueの総和を返す
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


    }

    namespace set_ds{
        template <size_t BinaryTree_universe>
        struct BinaryTreeSet{
            bitset<2*BinaryTree_universe> array;
            int sz;
            
            BinaryTreeSet(){};
            BinaryTreeSet(string bitarray): sz(BinaryTree_universe){
                assert(bitarray.size() <= BinaryTree_universe);
                for (int i=0;i<bitarray.size();i++){
                    array.set(sz+i, bitarray[i] == '0' ? 0 : 1);
                }
                for (int i=sz-1;i>0;i--){
                    array.set(i, array.test(2*i) | array.test(2*i+1));
                }
            };

            bool contains(int x){
                return array.test(sz+x);
            }

            void insert(int i){
                this->write(i,1);
            }

            void erase(int i){
                this->write(i,0);
            }

            void flip(int i){
                this->write(i,1^this->contains(i));
            }

            int predecessor(int i){
                i += sz;
                while (i > 0){
                    if ((i&1)==0){
                        if (array.test(i)){
                            break;
                        }
                        if ((i & (i-1))==0){
                            return -1;
                        }
                        i -= 1;
                    }
                    i >>= 1;
                }
                while (i < sz){
                    i = (i<<1)+array.test((i<<1)+1);
                }
                return i-sz; 
            }

            int successor(int i){
                i += sz;
                while (i > 0){
                    if ((i&1)==1){
                        if (array.test(i)){
                            break;
                        }
                        i += 1;
                        if ((i & (i-1))==0){
                            return -1;
                        }
                    }
                    i >>= 1;
                }
                while (i < sz){
                    i = (i<<1)+(1 - array.test(i<<1));
                }
                return i-sz;
            }

            private:
            void write(int i, bool x){
                i += sz;
                array.set(i, x);
                while (i > 1){
                    array.set(i>>1,array.test(i)|array.test(i^1));
                    i >>= 1;
                }
            }
        }; 

        struct BinaryTrie{
            struct Node{
                Node* zero;
                Node* one;
                size_t count;
                Node(Node* left, Node* right, size_t count) : zero(left), one(right), count(count) {}
            };

            int d;
            Node* root;
            ll world;
            BinaryTrie(int d):d(d),world(0){
                root = new Node(nullptr, nullptr, 0);
            };

            bool find(ll x){
                assert(0 <= x <= (((ll)1)<<d)-((ll)1));
                return _find(x^world);
            }

            void insert(ll x){
                assert(0 <= x <= (((ll)1)<<d)-((ll)1));
                if (!_find(x^world)){_insert(x^world);}
            }

            void erase(ll x){
                assert(0 <= x <= (((ll)1)<<d)-((ll)1));
                if (_find(x^world)){_erase(x^world);}
            }

            void all_xor(ll x){
                assert(0 <= x <= (((ll)1)<<d)-((ll)1));
                world ^= x;
            }


            size_t size(){
                return root->count;
            }

            ll kth_min(int k){
                Node* cur = root;
                ll res = 0;
                int c = 0;
                for (int i = d-1; i >= 0; i--){
                    if ((world & (((ll)1)<<i)) == 0){
                        if (cur->zero != nullptr){
                            c = cur->zero->count;
                        } else {
                            c = 0;
                        }
                        if (c > k){
                            cur = cur->zero;
                            res = 2*res;
                        } else {
                            k -= c;
                            cur = cur->one;
                            res = 2*res+1;
                        }
                    } else {
                        if (cur->one != nullptr){
                            c = cur->one->count;
                        } else {
                            c = 0;
                        }
                        if (c > k){
                            cur = cur->one;
                            res = 2*res;
                        } else {
                            k -= c;
                            cur = cur->zero;
                            res = 2*res+1;
                        }
                    }
                }
                return res;
            }

            ll kth_max(int i){
                return kth_min(size()-i-1);
            }

            ll min_xor(ll x){
                all_xor(x);
                ll ans = kth_min(0);
                all_xor(x);
                return ans;
            }

            ll max_xor(ll x){
                all_xor(x);
                ll ans = kth_max(0);
                all_xor(x);
                return ans;
            }

            private:
            void _insert(ll x){
                Node* cur = root;
                for (int i = d-1; i >= 0; i--){
                    cur->count += 1;
                    if ((x & (((ll)1)<<i)) == 0){
                        if (cur->zero == nullptr){
                            cur->zero = new Node(nullptr, nullptr, 0);
                        }
                        cur = cur->zero;
                    } else {
                        if (cur->one == nullptr){
                            cur->one = new Node(nullptr, nullptr, 0);
                        }
                        cur = cur->one;
                    }
                }
                cur -> count += 1;
            }

            bool _find(ll x){
                Node* cur = root;
                for (int i = d-1; i >= 0; i--){
                    if ((x & (((ll)1)<<i)) == 0){
                        if (cur->zero == nullptr){
                            return false;
                        }
                        cur = cur->zero;
                    } else {
                        if (cur->one == nullptr){
                            return false;
                        }
                        cur = cur->one;
                    }
                }
                if (cur->count > 0){
                    return true;
                } else {
                    return false;
                }

            }

            void _erase(ll x){
                Node* cur = root;
                for (int i = d-1; i >= 0; i--){
                    cur->count -= 1;
                    if ((x & (((ll)1)<<i)) == 0){
                        if (cur->zero == nullptr){
                            cur->zero = new Node(nullptr, nullptr, 0);
                        }
                        cur = cur->zero;
                    } else {
                        if (cur->one == nullptr){
                            cur->one = new Node(nullptr, nullptr, 0);
                        }
                        cur = cur->one;
                    }
                }
                cur -> count -= 1;
            }



        };


    }

    namespace typical_ds{

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

            int extend(){
                int idx = (int)parent.size();
                push();
                return idx;
            }

            vector<int> comped_label(){
                vector<int> A = label();
                vector<int> B = A;
                sort(B.begin(), B.end());
                B.erase(unique(B.begin(), B.end()), B.end());
                vector<int> res(parent.size());
                for (int i = 0; i < (int)parent.size(); i++) {
                    res[i] = lower_bound(B.begin(), B.end(), A[i]) - B.begin();
                }
                return res;
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

            DynamicUnionFind() : UF(0) { }

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
                return UF.merge(px,py);
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

        //部分永続配列を使うバージョン
        using array_ds::PartiallyPersistentArray;
        struct PartiallyPersistentUnionFind_PartiallyPersistentArray{
            PartiallyPersistentArray<int> parent;
            int c;
            bool is_original;

            PartiallyPersistentUnionFind_PartiallyPersistentArray(int n) : parent(n, -1), c(n), is_original(true) {}

            PartiallyPersistentUnionFind_PartiallyPersistentArray(PartiallyPersistentArray<int>& parent_snapshot, int c)
                : parent(parent_snapshot.copy()), c(c), is_original(false) {}

            int root(int x){
                int tmp = x;
                while (parent[tmp] >= 0){
                    tmp = parent[tmp];
                }

                if (is_original){
                    while (parent[x] >= 0){
                        parent[x] = tmp;
                        x = parent[x];
                    }
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
                assert(is_original);
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

            PartiallyPersistentUnionFind_PartiallyPersistentArray copy(){
                return PartiallyPersistentUnionFind_PartiallyPersistentArray(parent, c);
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

            void print(){
              cout << "{";
                for (int i = 0; i < parent.size(); i++){
                    cout << parent[i] << ", ";
                }
              cout << "}" << "\n";
            }
        };


    }

    namespace math{
        // https://trap.jp/post/2728/
        unsigned long long isqrt_aux(int c,unsigned long long n){
            if (c == 0){
                return 1;
            } else {
                int k = (c - 1) / 2;
                unsigned long long a = isqrt_aux(c / 2, n >> (2*k + 2));
                return (a << k) + (n >> (k+2)) / a;
            }
        }

        unsigned long isqrt(unsigned long long n){
            if (n == 0){
                return 0;
            } else {
                unsigned long long a = isqrt_aux(( 63 - __builtin_clzll(n)) / 2, n);
                return n <= a * a - 1 ? a - 1 : a;
            }
        }

        bool next_partation(vector<int>& array){
            int s = 0;
            for (int i=array.size()-1;i>=0;i--){
                if (array[i] == 1){
                    array.pop_back();
                    s += 1;
                } else {
                    array[i] -= 1;
                    s += 1;
                    for (int j=i+1;s > 0;j++){
                        array.push_back(min(s, array[i]));
                        s -= array[j];
                    }
                    return true;
                }
            }
            array.push_back(s);
            return false;
        }

        struct Permutation{
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
                next_permutation(_vall(this->array));
                for (int i=0;i<N;i++){
                    array_inv[array[i]] = i;
                }
            }

            void prev(){
                prev_permutation(_vall(this->array));
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



    }

    namespace library_checker{
        using namespace array_ds;
        struct AreaOfUnionOfRectangles{
            static pair<int,int> op(pair<int,int> x,pair<int,int> y){
                if (x.first < y.first){
                    return {x.first, x.second};
                } else if (x.first > y.first){
                    return {y.first, y.second};
                } else {
                    return {x.first, x.second + y.second};
                }
            }

            static int comp(int f,int g){
                return f + g;
            }

            static pair<int,int> mapping(int f, pair<int,int> x){
                return {x.first + f, x.second};
            }

            static int id(){
                return 0;
            }

            static pair<int,int> e(){
                return {1<<30,0};
            }

            vector<array<int,4>> data;
            vector<array<int,4>> vec;
            vector<int> _val;
            LazySegmentTree<pair<int,int>,int> LST;

            AreaOfUnionOfRectangles() = default;
            AreaOfUnionOfRectangles(vector<array<int,4>> _data) : data(_data) {}

            void push(array<int,4> a){
                data.push_back(a);
            }

            ll solve(){
                vec.reserve(2*data.size());
                _val.reserve(2*data.size());
                for(const auto& a : data){
                    auto [x1,y1,x2,y2] = a;
                    vec.push_back({y1,x1,x2,1});
                    vec.push_back({y2,x1,x2,-1});
                    _val.push_back(x1);
                    _val.push_back(x2);
                }

                sort(_vall(vec));
                sort(_vall(_val));
                _val.erase(unique(_val.begin(), _val.end()), _val.end());

                ll X = _val[_val.size()-1] - _val[0];
                vector<pair<int,int>> array;
                array.reserve(_val.size()-1);
                for(int i = 0;i < _val.size()-1;i++){
                    array.push_back({0 ,_val[i+1] - _val[i]});
                }

                LST = LazySegmentTree<pair<int,int>,int>(array, op, e, comp, mapping, id);

                ll ans = 0;
                int pret = 0;

                for(int i = 0;i < vec.size();i++){
                    auto [t,l,r,c] = vec[i];
                    auto res = LST.prod_all();
                    if (res.first == 0){
                        ans += ((ll)(t - pret)) * (X - (ll)res.second);
                    } else {
                        ans += ((ll)(t - pret)) * X;
                    }
                    pret = t;
                    l = lower_bound(_val.begin(), _val.end(), l) - _val.begin();
                    r = lower_bound(_val.begin(), _val.end(), r) - _val.begin();
                    LST.apply((int)l,(int)r,c);
                }

                return ans;
            }

        };

    }

    namespace seg_tmp{
        
    }

    namespace lseg_tmp{
        // RSQ + RUQ on ll
        namespace RSQ_RUQ{
            struct S
            {
                ll _value;
                ll length;
            };

            struct F{
                ll num;
                bool iswrited;
            };

            S op(S a,S b){
                return {a._value + b._value,a.length + b.length};
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
                    return {x._value,x.length};
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
    
}

    namespace segbeats_tmp{
        
    }
}

// // 欲しいもの
// エラトステネスの篩
// 線形篩
// 前計算素因数分解
// gcd
// 拡張gcd
// 逆元
// 自作mint, dynamic_mint
// モンゴメリ乗算
// ミラーラビン素数判定
// ポラードロー素因数分解
// 素因数分解型 using factorized_form = vector<pair<ll>>;
// 約数列挙
// 平方数の約数列挙
// 約数の総和
// オイラーのファイ関数


template <typename T>
T gcd(T a, T b){
    while(b != 0){
        a %= b;
        swap(a,b);
    }
    return a;
}

// {a,b} → {a^-1 mod b,gcd(a,b)}
template <typename T>
pair<T,T> exgcd(T a, T b){
    T xs = 1, ys = 0, xt = 0, yt = 1, tmp;
    while (b != 0){
        tmp = a/b;
        a = a%b;
        xs -= tmp*xt;
        ys -= tmp*yt;
        swap(xs,xt);
        swap(ys,yt);
        swap(a,b);
    }
    return {xs,a};
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


using factorized_form = vector<pair<ll, ll>>;

// ===============================================================================

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    cout << isprime(1) << "\n";
    cout << isprime(2) << "\n";
    cout << isprime(3) << "\n";
    cout << isprime(4) << "\n";
    cout << isprime(5) << "\n";
    cout << isprime(6) << "\n";
    cout << isprime(7) << "\n";
    cout << isprime(8) << "\n";
    cout << isprime(9) << "\n";
    cout << isprime(10) << "\n";
    cout << isprime(11) << "\n";
    cout << "\n";
    
    cout << gcd(24,18) << "\n";
    cout << gcd(114,514) << "\n";
    cout << "\n";

    cout << exgcd(24, 18) << "\n";
    cout << exgcd(114, 514) << "\n";
}