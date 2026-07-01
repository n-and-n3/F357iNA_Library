#include "indexed_avl_map.hpp"
#include "indexed_avl_set.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <iterator>

int main() {
    static_assert(std::bidirectional_iterator<avl::indexed_map<int,int>::iterator>);
    static_assert(!std::random_access_iterator<avl::indexed_map<int,int>::iterator>);
    static_assert(std::bidirectional_iterator<avl::indexed_set<int>::iterator>);
    static_assert(!std::random_access_iterator<avl::indexed_set<int>::iterator>);

    avl::indexed_map<int, std::string> m;
    auto [it1, ok1] = m.insert({3, "three"});
    assert(ok1 && it1->first == 3 && it1->second == "three");
    auto [it2, ok2] = m.insert({1, "one"});
    assert(ok2 && it2->first == 1);
    auto [it3, ok3] = m.insert({3, "THREE"});
    assert(!ok3 && it3->second == "three");
    assert(m.size() == 2);
    assert(m.at_index(0).first == 1 && m.at_index(0).second == "one");
    assert(m.at_index(1).first == 3 && m.at_index(1).second == "three");
    assert(m.order_of_key(3) == 1);
    assert(m.index_of(m.find(3)) == 1);
    assert(m.index_distance(m.begin(), m.end()) == 2);
    assert(m.verify_invariants());

    m[2] = "two";
    assert(m.at(2) == "two");
    assert(m.key_at_index(1) == 2);
    m.mapped_at_index(1) = "TWO";
    assert(m.at(2) == "TWO");

    auto [ioait, inserted] = m.insert_or_assign(2, "two again");
    assert(!inserted && ioait->second == "two again");
    auto [teit, teinserted] = m.try_emplace(2, "ignored");
    assert(!teinserted && teit->second == "two again");
    auto [teit2, teinserted2] = m.try_emplace(4, 5, 'x');
    assert(teinserted2 && teit2->second == "xxxxx");

    assert(m.erase(3) == 1);
    assert(m.erase(3) == 0);
    assert(!m.contains(3));
    assert(m.verify_invariants());

    avl::indexed_map<std::string, int, std::less<>> sm;
    sm.insert(std::pair<std::string, int>{"abc", 1});
    assert(sm.contains(std::string_view("abc")));
    assert(sm.find(std::string_view("abc"))->second == 1);

    avl::indexed_map<int, int> a{{1, 10}, {2, 20}, {3, 30}};
    auto nh = a.extract(2);
    assert(nh && nh.key() == 2 && nh.mapped() == 20 && !a.contains(2));
    nh.set_key(4);
    nh.mapped() = 40;
    auto ir = a.insert(std::move(nh));
    assert(ir.inserted && ir.position->first == 4 && ir.position->second == 40);
    assert(!nh);

    avl::indexed_map<int, int> b{{3, 300}, {4, 400}, {5, 500}};
    a.merge(b); // 3,4 already in a; 5 moves.
    assert(a.contains(5));
    assert(b.contains(3) && b.contains(4));
    assert(!b.contains(5));
    assert(a.verify_invariants() && b.verify_invariants());

    avl::indexed_map<int, int> x;
    std::map<int, int> y;
    std::mt19937 rng(123);
    for (int step = 0; step < 20000; ++step) {
        int op = static_cast<int>(rng() % 7);
        int k = static_cast<int>(rng() % 200);
        int v = static_cast<int>(rng() % 100000);
        if (op == 0) {
            auto rx = x.insert({k, v});
            auto ry = y.insert({k, v});
            assert(rx.second == ry.second);
        } else if (op == 1) {
            x[k] = v;
            y[k] = v;
        } else if (op == 2) {
            auto rx = x.insert_or_assign(k, v);
            auto ry = y.insert_or_assign(k, v);
            assert(rx.second == ry.second);
        } else if (op == 3) {
            auto rx = x.erase(k);
            auto ry = y.erase(k);
            assert(rx == ry);
        } else if (op == 4) {
            assert(x.count(k) == y.count(k));
            assert(x.contains(k) == (y.find(k) != y.end()));
        } else if (op == 5) {
            auto rx = x.lower_bound(k);
            auto ry = y.lower_bound(k);
            assert((rx == x.end()) == (ry == y.end()));
            if (rx != x.end()) assert(rx->first == ry->first && rx->second == ry->second);
        } else {
            auto rx = x.upper_bound(k);
            auto ry = y.upper_bound(k);
            assert((rx == x.end()) == (ry == y.end()));
            if (rx != x.end()) assert(rx->first == ry->first && rx->second == ry->second);
        }
        assert(x.size() == y.size());
        assert(x.verify_invariants());
        std::vector<std::pair<int, int>> vx;
        std::vector<std::pair<int, int>> vy;
        for (auto& [kk, vv] : x) vx.push_back({kk, vv});
        for (auto& [kk, vv] : y) vy.push_back({kk, vv});
        assert(vx == vy);
        for (std::size_t i = 0; i < vx.size(); ++i) {
            assert(x.at_index(i).first == vx[i].first);
            assert(x.at_index(i).second == vx[i].second);
            assert(x.index_of(x.find(vx[i].first)) == i);
        }
        for (int q = 0; q < 200; ++q) {
            assert(x.order_of_key(q) == static_cast<std::size_t>(std::distance(y.begin(), y.lower_bound(q))));
        }
    }

    std::cout << "ok\n";
}
