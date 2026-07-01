#include "indexed_avl_set.hpp"
#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <cassert>

int main(){
    avl::indexed_set<int> s;
    auto r1=s.insert(3); assert(r1.second);
    auto r2=s.insert(1); assert(r2.second);
    auto r3=s.insert(3); assert(!r3.second);
    assert(s.size()==2);
    assert(s.at(0)==1 && s.at(1)==3);
    assert(s.order_of_key(3)==1);
    assert(s.index_of(s.find(3))==1);
    assert(s.verify_invariants());

    avl::indexed_multiset<int> ms;
    ms.insert(3); ms.insert(1); ms.insert(3); ms.insert(2); ms.insert(3);
    std::vector<int> v(ms.begin(), ms.end());
    for(int x: v) std::cerr << x << ' '; std::cerr << '\n';
    assert((v == std::vector<int>{1,2,3,3,3}));
    assert(ms.count(3)==3);
    assert(ms.order_of_key(3)==2);
    assert(ms.at(3)==3);
    assert(ms.erase1(3)==1);
    assert(ms.count(3)==2);
    assert(ms.erase(3)==2);
    assert(ms.count(3)==0);
    assert(ms.verify_invariants());

    std::mt19937 rng(1);
    avl::indexed_multiset<int> a;
    std::multiset<int> b;
    for(int t=0;t<10000;t++){
        int op = rng()%4;
        int x = (int)(rng()%100);
        if(op==0){ a.insert(x); b.insert(x); }
        else if(op==1){ auto ra=a.erase(x); auto rb=b.erase(x); assert(ra==rb); }
        else if(op==2){ auto ra=a.erase1(x); auto it=b.find(x); std::size_t rb=0; if(it!=b.end()){ b.erase(it); rb=1; } assert(ra==rb); }
        else { assert(a.count(x)==b.count(x)); }
        assert(a.size()==b.size());
        assert(a.verify_invariants());
        std::vector<int> va(a.begin(), a.end()), vb(b.begin(), b.end());
        assert(va==vb);
        for(size_t i=0;i<va.size();++i) assert(a.at(i)==va[i]);
        for(int k=0;k<100;k++){
            assert(a.order_of_key(k)==(size_t)std::distance(b.begin(), b.lower_bound(k)));
        }
    }

    avl::indexed_set<std::string, std::less<>> ss;
    ss.insert("abc");
    assert(ss.contains("abc"));
    assert(ss.find(std::string_view("abc")) != ss.end());

    avl::indexed_set<int> x{1,2,3};
    auto nh = x.extract(2);
    assert(nh && nh.value()==2 && x.count(2)==0);
    nh.value() = 4;
    auto ir = x.insert(std::move(nh));
    assert(ir.inserted && x.contains(4));
    assert(!nh);

    avl::indexed_set<int> y{3,4,5};
    avl::indexed_multiset<int> z{3,3,6};
    y.merge(z); // only 6 moves, 3 remains duplicate
    assert(y.contains(6));
    assert(z.count(3)==2);
    assert(z.count(6)==0);
    assert(y.verify_invariants() && z.verify_invariants());

    std::cout << "ok\n";
}
