#include <iostream>
#include <vector>

using namespace std;

unsigned long long isqrt_aux(int c,unsigned long long n){
    if (c == 0){
        return 1;
    } else {
        int k = (c - 1) / 2;
        unsigned long long a = isqrt_aux(c / 2, n >> (2*k + 2));
        return (a << k) + (n >> (k+2)) / a;
    }
}

unsigned int isqrt(unsigned long long n){
    if (n == 0){
        return 0;
    } else {
        unsigned long long a = isqrt_aux(( 63 - __builtin_clzll(n)) / 2, n);
        return n <= a * a - 1 ? a - 1 : a;
    }
}

// ===========================================

// https://trap.jp/post/2728/

int main(){
    vector<unsigned long long> sample{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,100,1000,1024,314159265,4611686018427387903ULL,4611686018427387904ULL,18446744073709551614ULL,18446744073709551615ULL};
    for (int i=0;i < sample.size();i++){
        unsigned long long s,ss;
        s = sample[i];
        ss = isqrt(sample[i]);
        cout << s << " -> " << ss << "\n";
    }
}
