#include "fast_factorization.hpp"
#include <bits/stdc++.h>
using namespace std;
using namespace fast_factorization;

static vector<pair<long long,long long>> naive_factor(long long n){
    vector<pair<long long,long long>> res;
    for(long long p=2; p*p<=n; ++p){
        if(n%p==0){
            long long c=0;
            while(n%p==0) n/=p, ++c;
            res.push_back({p,c});
        }
    }
    if(n>1) res.push_back({n,1});
    return res;
}

static bool naive_prime(long long n){
    if(n<2) return false;
    for(long long d=2; d*d<=n; ++d) if(n%d==0) return false;
    return true;
}

static __int128 product_from_factorization(const vector<pair<long long,long long>>& f){
    __int128 prod = 1;
    for(auto [p,e] : f) while(e--) prod *= p;
    return prod;
}

int main(){
    set_seed(123456789);

    for(long long n=0; n<=200000; ++n){
        if(fast_isprime(n) != naive_prime(n)){
            cerr << "prime mismatch " << n << "\n";
            return 1;
        }
        auto f = fast_factorize(n);
        auto g = naive_factor(n);
        if(f != g){
            cerr << "factor mismatch " << n << "\n";
            return 1;
        }
    }

    vector<long long> tests = {
        1,
        2,
        3,
        4,
        60,
        360,
        999983,
        1000000,
        1000000000000LL,
        600851475143LL,
        2147483647LL,
        2305843009213693951LL,
        341550071728321LL,
        9223372036854775807LL
    };
    for(long long n : tests){
        auto f = fast_factorize(n);
        if(n >= 2 && product_from_factorization(f) != (__int128)n){
            cerr << "product mismatch " << n << "\n";
            return 1;
        }
        for(auto [p,e] : f){
            if(!fast_isprime(p)){
                cerr << "factor not prime " << p << " in " << n << "\n";
                return 1;
            }
            if(e <= 0){
                cerr << "bad exponent\n";
                return 1;
            }
        }
    }

    auto f360 = fast_factorize(360);
    vector<pair<long long,long long>> e360 = {{2,3},{3,2},{5,1}};
    if(f360 != e360){
        cerr << "360 factor mismatch\n";
        return 1;
    }
    auto d360 = fast_divisor(f360);
    vector<long long> expected360 = {1,2,3,4,5,6,8,9,10,12,15,18,20,24,30,36,40,45,60,72,90,120,180,360};
    if(d360 != expected360){
        cerr << "360 divisor mismatch\n";
        return 1;
    }

    if(fast_isprime(341550071728321LL)){
        cerr << "known pseudoprime composite misclassified\n";
        return 1;
    }
    if(!fast_isprime(2305843009213693951LL)){
        cerr << "2^61-1 should be prime\n";
        return 1;
    }

    mt19937_64 rng(0);
    for(int tc=0; tc<20000; ++tc){
        long long n = (long long)(rng() % 1000000000000ULL) + 1;
        auto f = fast_factorize(n);
        if(product_from_factorization(f) != (__int128)n){
            cerr << "random product mismatch " << n << "\n";
            return 1;
        }
        for(auto [p,e] : f){
            if(!fast_isprime(p)){
                cerr << "random factor not prime " << p << " in " << n << "\n";
                return 1;
            }
        }
    }

    cout << "OK\n";
}
