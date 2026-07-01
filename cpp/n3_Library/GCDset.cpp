#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <stdexcept>


// ==========================================
// GCDset (集合版: 重複なし)
// ==========================================
template <typename T>
class GCDset {
private:
    int cap;
    int leaf_count;
    int active_size;
    std::vector<T> tree;
    std::unordered_map<T, int> pos_map;

    void update(int idx, T val) {
        idx += cap;
        tree[idx] = val;
        idx /= 2;
        while (idx > 0) {
            tree[idx] = std::gcd(tree[2 * idx], tree[2 * idx + 1]);
            idx /= 2;
        }
    }

    void rebuild() {
        std::vector<T> active_elements;
        active_elements.reserve(active_size);
        for (int i = 0; i < leaf_count; ++i) {
            if (tree[cap + i] != T(0)) {
                active_elements.push_back(tree[cap + i]);
            }
        }

        int new_cap = 4;
        while (new_cap <= static_cast<int>(active_elements.size()) * 2) {
            new_cap *= 2;
        }

        cap = new_cap;
        tree.assign(2 * cap, T(0));
        pos_map.clear();
        leaf_count = 0;

        for (T x : active_elements) {
            int id = leaf_count++;
            pos_map[x] = id;
            tree[cap + id] = x;
        }

        for (int i = cap - 1; i > 0; --i) {
            tree[i] = std::gcd(tree[2 * i], tree[2 * i + 1]);
        }
    }

public:
    GCDset() {
        cap = 4;
        leaf_count = 0;
        active_size = 0;
        tree.assign(2 * cap, T(0));
    }

    void insert(T x) {
        if (pos_map.count(x)) {
            return;
        }

        if (leaf_count == cap) {
            rebuild();
        }

        int id = leaf_count++;
        pos_map[x] = id;
        
        update(id, x);
        active_size++;
    }

    void erase(T x) {
        if (!pos_map.count(x)) {
            throw std::invalid_argument("Error: erase() called for a non-existent element.");
        }

        int id = pos_map[x];
        update(id, T(0));
        pos_map.erase(x);
        active_size--;
    }

    bool contains(T x) const {
        return pos_map.count(x) > 0;
    }

    int size() const {
        return active_size;
    }

    T get_gcd() const {
        return tree[1];
    }
};


// ==========================================
// GCDmultiset (多重集合版)
// ==========================================
template <typename T>
class GCDmultiset {
private:
    int cap;
    int leaf_count;
    int active_size;
    std::vector<T> tree;
    std::vector<int> prev_idx;
    std::unordered_map<T, int> head_map;

    void update(int idx, T val) {
        idx += cap;
        tree[idx] = val;
        idx /= 2;
        while (idx > 0) {
            tree[idx] = std::gcd(tree[2 * idx], tree[2 * idx + 1]);
            idx /= 2;
        }
    }

    void rebuild() {
        std::vector<T> active_elements;
        active_elements.reserve(active_size);
        for (int i = 0; i < leaf_count; ++i) {
            if (tree[cap + i] != T(0)) {
                active_elements.push_back(tree[cap + i]);
            }
        }

        int new_cap = 4;
        while (new_cap <= static_cast<int>(active_elements.size()) * 2) {
            new_cap *= 2;
        }

        cap = new_cap;
        tree.assign(2 * cap, T(0));
        prev_idx.assign(cap, -1);
        head_map.clear();
        leaf_count = 0;

        for (T x : active_elements) {
            int id = leaf_count++;
            if (head_map.count(x)) {
                prev_idx[id] = head_map[x];
            }
            head_map[x] = id;
            tree[cap + id] = x;
        }

        for (int i = cap - 1; i > 0; --i) {
            tree[i] = std::gcd(tree[2 * i], tree[2 * i + 1]);
        }
    }

public:
    GCDmultiset() {
        cap = 4;
        leaf_count = 0;
        active_size = 0;
        tree.assign(2 * cap, T(0));
        prev_idx.assign(cap, -1);
    }

    void insert(T x) {
        if (leaf_count == cap) {
            rebuild();
        }

        int id = leaf_count++;
        if (head_map.count(x)) {
            prev_idx[id] = head_map[x];
        } else {
            prev_idx[id] = -1;
        }
        head_map[x] = id;
        
        update(id, x);
        active_size++;
    }

    void erase(T x) {
        if (!head_map.count(x)) {
            throw std::invalid_argument("Error: erase() called for a non-existent element.");
        }

        int id = head_map[x];
        int prev = prev_idx[id];
        
        if (prev != -1) {
            head_map[x] = prev;
        } else {
            head_map.erase(x);
        }
        
        update(id, T(0));
        active_size--;
    }

    bool contains(T x) const {
        return head_map.count(x) > 0;
    }

    int size() const {
        return active_size;
    }

    T get_gcd() const {
        return tree[1];
    }
};



// ==========================================
// 使用例
// ==========================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int q;
    if (!(std::cin >> q)) return 0;

    // long long 型を指定してインスタンス化
    GCDmultiset<long long> ms;
    // GCDset<long long> s; 

    for (int i = 0; i < q; ++i) {
        int type;
        std::cin >> type;
        if (type == 1) {
            long long x;
            std::cin >> x;
            ms.insert(x);
        } else if (type == 2) {
            long long x;
            std::cin >> x;
            if (ms.contains(x)) {
                ms.erase(x);
            }
        } else if (type == 3) {
            std::cout << ms.get_gcd() << "\n";
        }
    }

    return 0;
}