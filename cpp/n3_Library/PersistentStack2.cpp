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

template <typename T>
bool chmax(T &a, const T& b) { return a < b ? a = b, true : false; }
template <typename T>
bool chmin(T &a, const T& b) { return a > b ? a = b, true : false; }

istream &operator>>(istream &is, mint &i){long long t; is >> t; i = t; return is; }
ostream &operator<<(ostream &os, const mint &i){ os << i.val(); return os;}

// ===========================================================================================================================

template <typename T>
struct PersistentStack
{
    vector<int>* parent;
    vector<T>* data;
    int pos = -1;
    size_t size_num = 0;
    
    PersistentStack() : parent(new vector<int>()), data(new vector<T>()), size_num(0), pos(-1){}
    PersistentStack(const PersistentStack& ps) : parent(ps.parent), data(ps.data), size_num(ps.size_num), pos(ps.pos){}

    void push(T x){
        this->parent->push_back(this->pos);
        this->data->push_back(x);
        this->pos = this->parent->size()-1;
        this->size_num += 1;
    }

    T top(){
        assert(!this->empty());
        return (*this->data)[this->pos];
    }

    T top_or(const T& v) {
        if(this->empty()) return v;
        return this->top();
    }

    void pop(){
        assert(!this->empty());
        this->pos = (*this->parent)[this->pos];
        this->size_num -= 1;
    }

    void pop_safe(){
        if(!this->empty()) this->pop();
    }

    bool empty(){
        return this->pos < 0;
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

// ===========================================================================================================================

int main(){
    int Q;
    cin >> Q;

    string task;
    int num,page;

    PersistentStack<int> A;
    unordered_map<int,PersistentStack<int>> note;

    rep(i,Q){
        cin >> task;
        if (task == "ADD"){
            cin >> num;
            A.push(num);
        } else if (task == "DELETE") {
            A.pop_safe();
        } else if (task == "SAVE"){
            cin >> page;
            note[page] = A.clone();
        } else if (task == "LOAD"){
            cin >> page;
            A = note[page].clone();
        }
        cout << A.top_or(-1) << " \n"[i == Q-1];
        //cout << "res :\n" <<  A.top_or(-1) << "\n";
        //vout((*A.data));
        //vout((*A.parent));
        //cout << A.pos << "\n";
        //cout << A.size_num << "\n";
    }
    return 0;
}

/*
11
ADD 3
SAVE 1
ADD 4
SAVE 2
LOAD 1
DELETE
DELETE
LOAD 2
SAVE 1
LOAD 3
LOAD 1
*/