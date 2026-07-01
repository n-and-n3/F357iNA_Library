#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace avl {
namespace map_detail {

template<class, class = void>
struct has_is_transparent : std::false_type {};

template<class T>
struct has_is_transparent<T, std::void_t<typename T::is_transparent>> : std::true_type {};

template<class Value>
struct avl_map_node {
    using size_type = std::size_t;

    avl_map_node* left = nullptr;
    avl_map_node* right = nullptr;
    avl_map_node* parent = nullptr;
    int height = 1;
    size_type subtree_size = 1;
    Value value;

    template<class... Args>
    explicit avl_map_node(Args&&... args)
        : value(std::forward<Args>(args)...) {}
};

struct synth_three_way {
    template<class T, class U>
    constexpr auto operator()(const T& t, const U& u) const {
        if constexpr (requires { t <=> u; }) {
            return t <=> u;
        } else {
            if (t < u) return std::weak_ordering::less;
            if (u < t) return std::weak_ordering::greater;
            return std::weak_ordering::equivalent;
        }
    }
};

} // namespace map_detail

template<class Key, class T, class Compare = std::less<Key>,
         class Allocator = std::allocator<std::pair<const Key, T>>>
class indexed_map {
public:
    using key_type = Key;
    using mapped_type = T;
    using key_compare = Compare;
    using value_type = std::pair<const Key, T>;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    class value_compare {
        friend class indexed_map;
        key_compare comp_;

        explicit value_compare(key_compare comp) : comp_(std::move(comp)) {}

    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp_(lhs.first, rhs.first);
        }
    };

private:
    using node = map_detail::avl_map_node<value_type>;
    using node_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<node>;
    using node_traits = std::allocator_traits<node_allocator>;

    node* root_ = nullptr;
    size_type size_ = 0;
    key_compare comp_{};
    node_allocator alloc_{};

    template<class, class, class, class>
    friend class indexed_map;

    static int height(node* p) noexcept { return p ? p->height : 0; }
    static size_type subtree_size(node* p) noexcept { return p ? p->subtree_size : 0; }

    static node* min_node(node* p) noexcept {
        if (!p) return nullptr;
        while (p->left) p = p->left;
        return p;
    }

    static node* max_node(node* p) noexcept {
        if (!p) return nullptr;
        while (p->right) p = p->right;
        return p;
    }

    static void refresh(node* p) noexcept {
        if (!p) return;
        p->height = 1 + std::max(height(p->left), height(p->right));
        p->subtree_size = 1 + subtree_size(p->left) + subtree_size(p->right);
    }

    static int balance_factor(node* p) noexcept {
        return height(p->left) - height(p->right);
    }

    template<class K1, class K2>
    bool equivalent_keys(const K1& a, const K2& b) const {
        return !comp_(a, b) && !comp_(b, a);
    }

    void replace_child(node* parent, node* old_child, node* new_child) noexcept {
        if (!parent) {
            root_ = new_child;
        } else if (parent->left == old_child) {
            parent->left = new_child;
        } else {
            parent->right = new_child;
        }
        if (new_child) new_child->parent = parent;
    }

    node* rotate_left(node* x) noexcept {
        node* y = x->right;
        node* beta = y->left;
        node* parent = x->parent;

        replace_child(parent, x, y);
        y->left = x;
        x->parent = y;
        x->right = beta;
        if (beta) beta->parent = x;

        refresh(x);
        refresh(y);
        return y;
    }

    node* rotate_right(node* y) noexcept {
        node* x = y->left;
        node* beta = x->right;
        node* parent = y->parent;

        replace_child(parent, y, x);
        x->right = y;
        y->parent = x;
        y->left = beta;
        if (beta) beta->parent = y;

        refresh(y);
        refresh(x);
        return x;
    }

    void rebalance_from(node* p) noexcept {
        while (p) {
            refresh(p);
            const int bf = balance_factor(p);
            if (bf > 1) {
                if (balance_factor(p->left) < 0) rotate_left(p->left);
                p = rotate_right(p);
            } else if (bf < -1) {
                if (balance_factor(p->right) > 0) rotate_right(p->right);
                p = rotate_left(p);
            }
            p = p->parent;
        }
    }

    template<class... Args>
    node* make_node(Args&&... args) {
        node* p = node_traits::allocate(alloc_, 1);
        try {
            node_traits::construct(alloc_, p, std::forward<Args>(args)...);
        } catch (...) {
            node_traits::deallocate(alloc_, p, 1);
            throw;
        }
        return p;
    }

    void destroy_node(node* p) noexcept {
        if (!p) return;
        node_traits::destroy(alloc_, p);
        node_traits::deallocate(alloc_, p, 1);
    }

    void prepare_singleton(node* p) noexcept {
        p->left = p->right = p->parent = nullptr;
        p->height = 1;
        p->subtree_size = 1;
    }

    void link_new_child(node* parent, bool as_left, node* child) noexcept {
        prepare_singleton(child);
        child->parent = parent;
        if (!parent) {
            root_ = child;
        } else if (as_left) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        ++size_;
        rebalance_from(parent ? parent : child);
    }

    std::pair<node*, bool> attach_unique_node(node* fresh) {
        assert(fresh);
        node* parent = nullptr;
        node* cur = root_;
        bool as_left = false;
        const key_type& key = fresh->value.first;

        while (cur) {
            parent = cur;
            if (comp_(key, cur->value.first)) {
                cur = cur->left;
                as_left = true;
            } else if (comp_(cur->value.first, key)) {
                cur = cur->right;
                as_left = false;
            } else {
                return {cur, false};
            }
        }
        link_new_child(parent, as_left, fresh);
        return {fresh, true};
    }

    template<class K>
    node* lower_bound_node(const K& key) const {
        node* cur = root_;
        node* ans = nullptr;
        while (cur) {
            if (!comp_(cur->value.first, key)) {
                ans = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return ans;
    }

    template<class K>
    node* upper_bound_node(const K& key) const {
        node* cur = root_;
        node* ans = nullptr;
        while (cur) {
            if (comp_(key, cur->value.first)) {
                ans = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return ans;
    }

    template<class K>
    node* find_node(const K& key) const {
        node* p = lower_bound_node(key);
        if (p && equivalent_keys(key, p->value.first)) return p;
        return nullptr;
    }

    size_type rank_node(const node* p) const noexcept {
        size_type ans = subtree_size(p->left);
        while (p->parent) {
            if (p == p->parent->right) ans += subtree_size(p->parent->left) + 1;
            p = p->parent;
        }
        return ans;
    }

    node* nth_node(size_type index) const noexcept {
        node* cur = root_;
        while (cur) {
            const size_type left_size = subtree_size(cur->left);
            if (index < left_size) {
                cur = cur->left;
            } else if (index == left_size) {
                return cur;
            } else {
                index -= left_size + 1;
                cur = cur->right;
            }
        }
        return nullptr;
    }

    void transplant(node* old_node, node* new_node) noexcept {
        replace_child(old_node->parent, old_node, new_node);
    }

    node* unlink_node(node* z) noexcept {
        assert(z);
        node* rebalance_start = nullptr;

        if (!z->left) {
            rebalance_start = z->parent;
            transplant(z, z->right);
        } else if (!z->right) {
            rebalance_start = z->parent;
            transplant(z, z->left);
        } else {
            node* y = min_node(z->right);
            if (y->parent != z) {
                rebalance_start = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                if (y->right) y->right->parent = y;
            } else {
                rebalance_start = y;
            }
            transplant(z, y);
            y->left = z->left;
            if (y->left) y->left->parent = y;
            refresh(y);
        }

        --size_;
        prepare_singleton(z);

        if (rebalance_start) {
            rebalance_from(rebalance_start);
        } else if (root_) {
            rebalance_from(root_);
        }
        return z;
    }

    void erase_node(node* p) noexcept {
        destroy_node(unlink_node(p));
    }

    node* clone_subtree(node* other, node* parent) {
        if (!other) return nullptr;
        node* p = make_node(other->value);
        p->parent = parent;
        try {
            p->left = clone_subtree(other->left, p);
            p->right = clone_subtree(other->right, p);
            refresh(p);
        } catch (...) {
            clear_subtree(p->left);
            clear_subtree(p->right);
            p->left = p->right = nullptr;
            destroy_node(p);
            throw;
        }
        return p;
    }

    void clear_subtree(node* p) noexcept {
        if (!p) return;
        clear_subtree(p->left);
        clear_subtree(p->right);
        destroy_node(p);
    }

public:
    class const_iterator {
        friend class indexed_map;
        template<class, class, class, class>
        friend class indexed_map;

        node* ptr_ = nullptr;
        const indexed_map* owner_ = nullptr;

        const_iterator(node* ptr, const indexed_map* owner) noexcept
            : ptr_(ptr), owner_(owner) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using value_type = indexed_map::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() noexcept = default;

        reference operator*() const noexcept { return ptr_->value; }
        pointer operator->() const noexcept { return std::addressof(ptr_->value); }

        const_iterator& operator++() noexcept {
            if (ptr_->right) {
                ptr_ = min_node(ptr_->right);
            } else {
                node* p = ptr_->parent;
                while (p && ptr_ == p->right) {
                    ptr_ = p;
                    p = p->parent;
                }
                ptr_ = p;
            }
            return *this;
        }

        const_iterator operator++(int) noexcept {
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        const_iterator& operator--() noexcept {
            if (!ptr_) {
                ptr_ = max_node(owner_->root_);
            } else if (ptr_->left) {
                ptr_ = max_node(ptr_->left);
            } else {
                node* p = ptr_->parent;
                while (p && ptr_ == p->left) {
                    ptr_ = p;
                    p = p->parent;
                }
                ptr_ = p;
            }
            return *this;
        }

        const_iterator operator--(int) noexcept {
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }

        friend bool operator==(const const_iterator& a, const const_iterator& b) noexcept {
            return a.ptr_ == b.ptr_ && a.owner_ == b.owner_;
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b) noexcept {
            return !(a == b);
        }
    };

    class iterator {
        friend class indexed_map;
        template<class, class, class, class>
        friend class indexed_map;

        node* ptr_ = nullptr;
        const indexed_map* owner_ = nullptr;

        iterator(node* ptr, const indexed_map* owner) noexcept
            : ptr_(ptr), owner_(owner) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using value_type = indexed_map::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() noexcept = default;

        operator const_iterator() const noexcept { return const_iterator(ptr_, owner_); }

        reference operator*() const noexcept { return ptr_->value; }
        pointer operator->() const noexcept { return std::addressof(ptr_->value); }

        iterator& operator++() noexcept {
            if (ptr_->right) {
                ptr_ = min_node(ptr_->right);
            } else {
                node* p = ptr_->parent;
                while (p && ptr_ == p->right) {
                    ptr_ = p;
                    p = p->parent;
                }
                ptr_ = p;
            }
            return *this;
        }

        iterator operator++(int) noexcept {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        iterator& operator--() noexcept {
            if (!ptr_) {
                ptr_ = max_node(owner_->root_);
            } else if (ptr_->left) {
                ptr_ = max_node(ptr_->left);
            } else {
                node* p = ptr_->parent;
                while (p && ptr_ == p->left) {
                    ptr_ = p;
                    p = p->parent;
                }
                ptr_ = p;
            }
            return *this;
        }

        iterator operator--(int) noexcept {
            iterator tmp = *this;
            --*this;
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b) noexcept {
            return a.ptr_ == b.ptr_ && a.owner_ == b.owner_;
        }

        friend bool operator!=(const iterator& a, const iterator& b) noexcept {
            return !(a == b);
        }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    class node_type {
        friend class indexed_map;
        node* ptr_ = nullptr;
        node_allocator alloc_{};

        node_type(node* ptr, const node_allocator& alloc) : ptr_(ptr), alloc_(alloc) {}

        void reset() noexcept {
            if (ptr_) {
                node_traits::destroy(alloc_, ptr_);
                node_traits::deallocate(alloc_, ptr_, 1);
                ptr_ = nullptr;
            }
        }

    public:
        node_type() noexcept(std::is_nothrow_default_constructible_v<node_allocator>) = default;
        node_type(const node_type&) = delete;
        node_type& operator=(const node_type&) = delete;

        node_type(node_type&& other) noexcept(std::is_nothrow_move_constructible_v<node_allocator>)
            : ptr_(std::exchange(other.ptr_, nullptr)), alloc_(std::move(other.alloc_)) {}

        node_type& operator=(node_type&& other) noexcept(
            std::is_nothrow_move_assignable_v<node_allocator>) {
            if (this != &other) {
                reset();
                alloc_ = std::move(other.alloc_);
                ptr_ = std::exchange(other.ptr_, nullptr);
            }
            return *this;
        }

        ~node_type() { reset(); }

        [[nodiscard]] bool empty() const noexcept { return ptr_ == nullptr; }
        explicit operator bool() const noexcept { return ptr_ != nullptr; }

        allocator_type get_allocator() const noexcept { return allocator_type(alloc_); }

        // std::map の node_handle では key() が変更可能ですが、この実装では
        // value_type = pair<const Key, T> をそのまま保持しているため、参照代入による
        // key 変更は提供せず、明示的な set_key() で detached node を再構築します。
        const key_type& key() const { return ptr_->value.first; }
        mapped_type& mapped() const { return ptr_->value.second; }

        void set_key(const key_type& key) { replace_key(key); }
        void set_key(key_type&& key) { replace_key(std::move(key)); }

    private:
        template<class K>
        void replace_key(K&& key) {
            assert(ptr_);
            mapped_type mapped(std::move(ptr_->value.second));
            node_traits::destroy(alloc_, ptr_);
            try {
                node_traits::construct(alloc_, ptr_, std::piecewise_construct,
                                       std::forward_as_tuple(std::forward<K>(key)),
                                       std::forward_as_tuple(std::move(mapped)));
            } catch (...) {
                node_traits::deallocate(alloc_, ptr_, 1);
                ptr_ = nullptr;
                throw;
            }
        }
    };

    struct insert_return_type {
        iterator position;
        bool inserted;
        node_type node;
    };

    indexed_map()
        : indexed_map(key_compare()) {}

    explicit indexed_map(const key_compare& comp,
                         const allocator_type& alloc = allocator_type())
        : root_(nullptr), size_(0), comp_(comp), alloc_(alloc) {}

    explicit indexed_map(const allocator_type& alloc)
        : indexed_map(key_compare(), alloc) {}

    template<class InputIt>
    indexed_map(InputIt first, InputIt last,
                const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
        : indexed_map(comp, alloc) {
        insert(first, last);
    }

    template<class InputIt>
    indexed_map(InputIt first, InputIt last, const allocator_type& alloc)
        : indexed_map(first, last, key_compare(), alloc) {}

    indexed_map(std::initializer_list<value_type> init,
                const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
        : indexed_map(init.begin(), init.end(), comp, alloc) {}

    indexed_map(std::initializer_list<value_type> init, const allocator_type& alloc)
        : indexed_map(init.begin(), init.end(), key_compare(), alloc) {}

    indexed_map(const indexed_map& other)
        : root_(nullptr), size_(0), comp_(other.comp_),
          alloc_(node_traits::select_on_container_copy_construction(other.alloc_)) {
        root_ = clone_subtree(other.root_, nullptr);
        size_ = other.size_;
    }

    indexed_map(const indexed_map& other, const allocator_type& alloc)
        : root_(nullptr), size_(0), comp_(other.comp_), alloc_(alloc) {
        root_ = clone_subtree(other.root_, nullptr);
        size_ = other.size_;
    }

    indexed_map(indexed_map&& other) noexcept
        : root_(std::exchange(other.root_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          comp_(std::move(other.comp_)),
          alloc_(std::move(other.alloc_)) {
        if (root_) root_->parent = nullptr;
    }

    indexed_map(indexed_map&& other, const allocator_type& alloc)
        : indexed_map(key_compare(std::move(other.comp_)), alloc) {
        if (allocator_type(other.alloc_) == alloc) {
            root_ = std::exchange(other.root_, nullptr);
            size_ = std::exchange(other.size_, 0);
            if (root_) root_->parent = nullptr;
        } else {
            insert(other.begin(), other.end());
            other.clear();
        }
    }

    ~indexed_map() { clear(); }

    indexed_map& operator=(const indexed_map& other) {
        if (this == &other) return *this;
        indexed_map tmp(other, allocator_type(alloc_));
        tmp.comp_ = other.comp_;
        swap(tmp);
        return *this;
    }

    indexed_map& operator=(indexed_map&& other) noexcept(
        std::allocator_traits<allocator_type>::is_always_equal::value &&
        std::is_nothrow_move_assignable_v<key_compare>) {
        if (this == &other) return *this;
        clear();
        if constexpr (node_traits::propagate_on_container_move_assignment::value) {
            alloc_ = std::move(other.alloc_);
        }
        comp_ = std::move(other.comp_);
        if constexpr (node_traits::propagate_on_container_move_assignment::value ||
                      node_traits::is_always_equal::value) {
            root_ = std::exchange(other.root_, nullptr);
            size_ = std::exchange(other.size_, 0);
            if (root_) root_->parent = nullptr;
        } else {
            if (alloc_ == other.alloc_) {
                root_ = std::exchange(other.root_, nullptr);
                size_ = std::exchange(other.size_, 0);
                if (root_) root_->parent = nullptr;
            } else {
                insert(other.begin(), other.end());
                other.clear();
            }
        }
        return *this;
    }

    indexed_map& operator=(std::initializer_list<value_type> init) {
        clear();
        insert(init);
        return *this;
    }

    allocator_type get_allocator() const noexcept { return allocator_type(alloc_); }

    mapped_type& at(const key_type& key) {
        node* p = find_node(key);
        if (!p) throw std::out_of_range("indexed_map::at: key not found");
        return p->value.second;
    }

    const mapped_type& at(const key_type& key) const {
        node* p = find_node(key);
        if (!p) throw std::out_of_range("indexed_map::at: key not found");
        return p->value.second;
    }

    mapped_type& operator[](const key_type& key) {
        return try_emplace(key).first->second;
    }

    mapped_type& operator[](key_type&& key) {
        return try_emplace(std::move(key)).first->second;
    }

    iterator begin() noexcept { return iterator(min_node(root_), this); }
    const_iterator begin() const noexcept { return const_iterator(min_node(root_), this); }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator(nullptr, this); }
    const_iterator end() const noexcept { return const_iterator(nullptr, this); }
    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return node_traits::max_size(alloc_); }

    void clear() noexcept {
        clear_subtree(root_);
        root_ = nullptr;
        size_ = 0;
    }

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        node* fresh = make_node(std::forward<Args>(args)...);
        auto [pos, inserted] = attach_unique_node(fresh);
        if (!inserted) destroy_node(fresh);
        return {iterator(pos, this), inserted};
    }

    template<class... Args>
    iterator emplace_hint(const_iterator, Args&&... args) {
        return emplace(std::forward<Args>(args)...).first;
    }

    std::pair<iterator, bool> insert(const value_type& x) { return emplace(x); }
    std::pair<iterator, bool> insert(value_type&& x) { return emplace(std::move(x)); }

    template<class P,
             std::enable_if_t<std::is_constructible_v<value_type, P&&> &&
                              !std::is_same_v<std::remove_cv_t<std::remove_reference_t<P>>, value_type>, int> = 0>
    std::pair<iterator, bool> insert(P&& x) {
        return emplace(std::forward<P>(x));
    }

    iterator insert(const_iterator hint, const value_type& x) {
        (void)hint;
        return insert(x).first;
    }

    iterator insert(const_iterator hint, value_type&& x) {
        (void)hint;
        return insert(std::move(x)).first;
    }

    template<class P,
             std::enable_if_t<std::is_constructible_v<value_type, P&&> &&
                              !std::is_same_v<std::remove_cv_t<std::remove_reference_t<P>>, value_type>, int> = 0>
    iterator insert(const_iterator hint, P&& x) {
        (void)hint;
        return insert(std::forward<P>(x)).first;
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) insert(*first);
    }

    void insert(std::initializer_list<value_type> init) {
        insert(init.begin(), init.end());
    }

    insert_return_type insert(node_type&& nh) {
        if (nh.empty()) return insert_return_type{end(), false, node_type{}};
        if (!(nh.get_allocator() == get_allocator())) {
            throw std::invalid_argument("indexed_map::insert(node_type): allocator mismatch");
        }
        node* p = nh.ptr_;
        auto [pos, inserted] = attach_unique_node(p);
        if (inserted) {
            nh.ptr_ = nullptr;
            return insert_return_type{iterator(pos, this), true, node_type{}};
        }
        return insert_return_type{iterator(pos, this), false, std::move(nh)};
    }

    iterator insert(const_iterator, node_type&& nh) {
        if (nh.empty()) return end();
        if (!(nh.get_allocator() == get_allocator())) {
            throw std::invalid_argument("indexed_map::insert(hint, node_type): allocator mismatch");
        }
        node* p = nh.ptr_;
        auto [pos, inserted] = attach_unique_node(p);
        if (inserted) nh.ptr_ = nullptr;
        return iterator(pos, this);
    }

    template<class... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args) {
        if (node* p = find_node(key)) return {iterator(p, this), false};
        node* fresh = make_node(std::piecewise_construct,
                                std::forward_as_tuple(key),
                                std::forward_as_tuple(std::forward<Args>(args)...));
        auto [pos, inserted] = attach_unique_node(fresh);
        assert(inserted);
        return {iterator(pos, this), true};
    }

    template<class... Args>
    std::pair<iterator, bool> try_emplace(key_type&& key, Args&&... args) {
        if (node* p = find_node(key)) return {iterator(p, this), false};
        node* fresh = make_node(std::piecewise_construct,
                                std::forward_as_tuple(std::move(key)),
                                std::forward_as_tuple(std::forward<Args>(args)...));
        auto [pos, inserted] = attach_unique_node(fresh);
        assert(inserted);
        return {iterator(pos, this), true};
    }

    template<class... Args>
    iterator try_emplace(const_iterator, const key_type& key, Args&&... args) {
        return try_emplace(key, std::forward<Args>(args)...).first;
    }

    template<class... Args>
    iterator try_emplace(const_iterator, key_type&& key, Args&&... args) {
        return try_emplace(std::move(key), std::forward<Args>(args)...).first;
    }

    template<class M>
    std::pair<iterator, bool> insert_or_assign(const key_type& key, M&& obj) {
        if (node* p = find_node(key)) {
            p->value.second = std::forward<M>(obj);
            return {iterator(p, this), false};
        }
        node* fresh = make_node(std::piecewise_construct,
                                std::forward_as_tuple(key),
                                std::forward_as_tuple(std::forward<M>(obj)));
        auto [pos, inserted] = attach_unique_node(fresh);
        assert(inserted);
        return {iterator(pos, this), true};
    }

    template<class M>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, M&& obj) {
        if (node* p = find_node(key)) {
            p->value.second = std::forward<M>(obj);
            return {iterator(p, this), false};
        }
        node* fresh = make_node(std::piecewise_construct,
                                std::forward_as_tuple(std::move(key)),
                                std::forward_as_tuple(std::forward<M>(obj)));
        auto [pos, inserted] = attach_unique_node(fresh);
        assert(inserted);
        return {iterator(pos, this), true};
    }

    template<class M>
    iterator insert_or_assign(const_iterator, const key_type& key, M&& obj) {
        return insert_or_assign(key, std::forward<M>(obj)).first;
    }

    template<class M>
    iterator insert_or_assign(const_iterator, key_type&& key, M&& obj) {
        return insert_or_assign(std::move(key), std::forward<M>(obj)).first;
    }

    node_type extract(const_iterator position) {
        assert(position.ptr_ && position.owner_ == this);
        node* p = unlink_node(position.ptr_);
        return node_type(p, alloc_);
    }

    node_type extract(const key_type& key) {
        node* p = find_node(key);
        if (!p) return node_type{};
        return extract(const_iterator(p, this));
    }

    iterator erase(iterator position) noexcept {
        return erase(const_iterator(position.ptr_, this));
    }

    iterator erase(const_iterator position) noexcept {
        assert(position.ptr_ && position.owner_ == this);
        iterator next(position.ptr_, this);
        ++next;
        erase_node(position.ptr_);
        return next;
    }

    size_type erase(const key_type& key) {
        node* p = find_node(key);
        if (!p) return 0;
        erase_node(p);
        return 1;
    }

    iterator erase(const_iterator first, const_iterator last) noexcept {
        while (first != last) first = erase(first);
        return iterator(last.ptr_, this);
    }

    void swap(indexed_map& other) noexcept(
        std::allocator_traits<allocator_type>::is_always_equal::value &&
        std::is_nothrow_swappable_v<key_compare>) {
        using std::swap;
        swap(root_, other.root_);
        swap(size_, other.size_);
        swap(comp_, other.comp_);
        if constexpr (node_traits::propagate_on_container_swap::value) {
            swap(alloc_, other.alloc_);
        } else {
            assert(alloc_ == other.alloc_ && "swapping containers with unequal non-propagating allocators is undefined");
        }
        if (root_) root_->parent = nullptr;
        if (other.root_) other.root_->parent = nullptr;
    }

    template<class C2>
    void merge(indexed_map<key_type, mapped_type, C2, allocator_type>& source) {
        if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&source)) return;
        if (!(get_allocator() == source.get_allocator())) {
            throw std::invalid_argument("indexed_map::merge: allocator mismatch");
        }
        for (auto it = source.begin(); it != source.end(); ) {
            node* p = it.ptr_;
            ++it;
            if (find(p->value.first) != end()) continue;
            source.unlink_node(p);
            auto [pos, inserted] = attach_unique_node(p);
            (void)pos;
            assert(inserted);
        }
    }

    template<class C2>
    void merge(indexed_map<key_type, mapped_type, C2, allocator_type>&& source) {
        merge(source);
    }

    key_compare key_comp() const { return comp_; }
    value_compare value_comp() const { return value_compare(comp_); }

    iterator find(const key_type& key) { return iterator(find_node(key), this); }
    const_iterator find(const key_type& key) const { return const_iterator(find_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    iterator find(const K& key) { return iterator(find_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    const_iterator find(const K& key) const { return const_iterator(find_node(key), this); }

    size_type count(const key_type& key) const { return contains(key) ? 1u : 0u; }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    size_type count(const K& key) const { return contains(key) ? 1u : 0u; }

    bool contains(const key_type& key) const { return find(key) != end(); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    bool contains(const K& key) const { return find(key) != end(); }

    iterator lower_bound(const key_type& key) { return iterator(lower_bound_node(key), this); }
    const_iterator lower_bound(const key_type& key) const { return const_iterator(lower_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    iterator lower_bound(const K& key) { return iterator(lower_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    const_iterator lower_bound(const K& key) const { return const_iterator(lower_bound_node(key), this); }

    iterator upper_bound(const key_type& key) { return iterator(upper_bound_node(key), this); }
    const_iterator upper_bound(const key_type& key) const { return const_iterator(upper_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    iterator upper_bound(const K& key) { return iterator(upper_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    const_iterator upper_bound(const K& key) const { return const_iterator(upper_bound_node(key), this); }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    std::pair<iterator, iterator> equal_range(const K& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    template<class K, class C = key_compare,
             std::enable_if_t<map_detail::has_is_transparent<C>::value, int> = 0>
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    // Index access extensions. All are O(log n), except index_of(end()) which is O(1).
    iterator nth(size_type index) noexcept {
        return index < size_ ? iterator(nth_node(index), this) : end();
    }

    const_iterator nth(size_type index) const noexcept {
        return index < size_ ? const_iterator(nth_node(index), this) : end();
    }

    iterator find_by_order(size_type index) noexcept { return nth(index); }
    const_iterator find_by_order(size_type index) const noexcept { return nth(index); }

    reference at_index(size_type index) {
        node* p = nth_node(index);
        if (!p) throw std::out_of_range("indexed_map::at_index: index out of range");
        return p->value;
    }

    const_reference at_index(size_type index) const {
        node* p = nth_node(index);
        if (!p) throw std::out_of_range("indexed_map::at_index: index out of range");
        return p->value;
    }

    mapped_type& mapped_at_index(size_type index) {
        return at_index(index).second;
    }

    const mapped_type& mapped_at_index(size_type index) const {
        return at_index(index).second;
    }

    const key_type& key_at_index(size_type index) const {
        return at_index(index).first;
    }

    template<class K>
    size_type order_of_key(const K& key) const {
        size_type ans = 0;
        node* cur = root_;
        while (cur) {
            if (comp_(cur->value.first, key)) {
                ans += subtree_size(cur->left) + 1;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        return ans;
    }

    size_type index_of(const_iterator it) const {
        if (it == end()) return size_;
        if (it.owner_ != this || !it.ptr_) throw std::invalid_argument("indexed_map::index_of: foreign iterator");
        return rank_node(it.ptr_);
    }

    difference_type index_distance(const_iterator first, const_iterator last) const {
        return static_cast<difference_type>(index_of(last)) -
               static_cast<difference_type>(index_of(first));
    }

    bool verify_invariants() const {
        size_type counted = 0;
        bool ok = true;
        const node* prev = nullptr;
        verify_rec(root_, nullptr, counted, ok, prev);
        return ok && counted == size_;
    }

private:
    int verify_rec(const node* p, const node* parent, size_type& counted,
                   bool& ok, const node*& prev) const {
        if (!p) return 0;
        if (p->parent != parent) ok = false;
        int lh = verify_rec(p->left, p, counted, ok, prev);
        if (prev && !comp_(prev->value.first, p->value.first)) ok = false;
        prev = p;
        ++counted;
        int rh = verify_rec(p->right, p, counted, ok, prev);
        if (p->height != 1 + std::max(lh, rh)) ok = false;
        if (p->subtree_size != 1 + subtree_size(p->left) + subtree_size(p->right)) ok = false;
        if (lh - rh < -1 || lh - rh > 1) ok = false;
        return 1 + std::max(lh, rh);
    }
};

template<class Key, class T, class Compare, class Allocator>
bool operator==(const indexed_map<Key, T, Compare, Allocator>& a,
                const indexed_map<Key, T, Compare, Allocator>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

template<class Key, class T, class Compare, class Allocator>
auto operator<=>(const indexed_map<Key, T, Compare, Allocator>& a,
                 const indexed_map<Key, T, Compare, Allocator>& b) {
    return std::lexicographical_compare_three_way(
        a.begin(), a.end(), b.begin(), b.end(), map_detail::synth_three_way{});
}

template<class Key, class T, class Compare, class Allocator>
void swap(indexed_map<Key, T, Compare, Allocator>& a,
          indexed_map<Key, T, Compare, Allocator>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

template<class Key, class T, class Compare, class Allocator, class Predicate>
typename indexed_map<Key, T, Compare, Allocator>::size_type
erase_if(indexed_map<Key, T, Compare, Allocator>& c, Predicate pred) {
    auto old = c.size();
    for (auto it = c.begin(); it != c.end(); ) {
        if (pred(*it)) it = c.erase(it);
        else ++it;
    }
    return old - c.size();
}

} // namespace avl

