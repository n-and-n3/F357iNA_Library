#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace avl {
namespace detail {

template<class, class = void>
struct has_is_transparent : std::false_type {};

template<class T>
struct has_is_transparent<T, std::void_t<typename T::is_transparent>> : std::true_type {};

template<class Key>
struct avl_node {
    using size_type = std::size_t;

    avl_node* left = nullptr;
    avl_node* right = nullptr;
    avl_node* parent = nullptr;
    int height = 1;
    size_type subtree_size = 1;
    Key value;

    template<class... Args>
    explicit avl_node(Args&&... args)
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

} // namespace detail

template<class Key, class Compare = std::less<Key>,
         class Allocator = std::allocator<Key>, bool Multi = false>
class basic_indexed_set {
public:
    using key_type = Key;
    using key_compare = Compare;
    using value_type = Key;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    using node = detail::avl_node<value_type>;
    using node_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<node>;
    using node_traits = std::allocator_traits<node_allocator>;

    node* root_ = nullptr;
    size_type size_ = 0;
    key_compare comp_{};
    node_allocator alloc_{};

    template<class, class, class, bool>
    friend class basic_indexed_set;

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

    void destroy_node_with(node* p, node_allocator& a) noexcept {
        if (!p) return;
        node_traits::destroy(a, p);
        node_traits::deallocate(a, p, 1);
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
        while (cur) {
            parent = cur;
            if (comp_(fresh->value, cur->value)) {
                cur = cur->left;
                as_left = true;
            } else if (comp_(cur->value, fresh->value)) {
                cur = cur->right;
                as_left = false;
            } else {
                return {cur, false};
            }
        }
        link_new_child(parent, as_left, fresh);
        return {fresh, true};
    }

    node* attach_equal_node(node* fresh) {
        assert(fresh);
        node* parent = nullptr;
        node* cur = root_;
        bool as_left = false;
        while (cur) {
            parent = cur;
            if (comp_(fresh->value, cur->value)) {
                cur = cur->left;
                as_left = true;
            } else {
                // Equal keys are inserted at the end of their equivalent-key range.
                cur = cur->right;
                as_left = false;
            }
        }
        link_new_child(parent, as_left, fresh);
        return fresh;
    }

    template<class K>
    node* lower_bound_node(const K& key) const {
        node* cur = root_;
        node* ans = nullptr;
        while (cur) {
            if (!comp_(cur->value, key)) {
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
            if (comp_(key, cur->value)) {
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
        if (p && !comp_(key, p->value) && !comp_(p->value, key)) return p;
        return nullptr;
    }

    template<class K>
    size_type order_of_upper_key(const K& key) const {
        size_type ans = 0;
        node* cur = root_;
        while (cur) {
            if (!comp_(key, cur->value)) {
                ans += subtree_size(cur->left) + 1;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        return ans;
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
        friend class basic_indexed_set;
        template<class, class, class, bool>
        friend class basic_indexed_set;

        node* ptr_ = nullptr;
        const basic_indexed_set* owner_ = nullptr;

        const_iterator(node* ptr, const basic_indexed_set* owner) noexcept
            : ptr_(ptr), owner_(owner) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const Key*;
        using reference = const Key&;

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

    using iterator = const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    class node_type {
        friend class basic_indexed_set;
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
        value_type& value() const { return ptr_->value; }
        key_type& key() const { return ptr_->value; }
    };

    struct insert_return_type {
        iterator position;
        bool inserted;
        node_type node;
    };

    basic_indexed_set()
        : basic_indexed_set(key_compare()) {}

    explicit basic_indexed_set(const key_compare& comp,
                               const allocator_type& alloc = allocator_type())
        : root_(nullptr), size_(0), comp_(comp), alloc_(alloc) {}

    explicit basic_indexed_set(const allocator_type& alloc)
        : basic_indexed_set(key_compare(), alloc) {}

    template<class InputIt>
    basic_indexed_set(InputIt first, InputIt last,
                      const key_compare& comp = key_compare(),
                      const allocator_type& alloc = allocator_type())
        : basic_indexed_set(comp, alloc) {
        insert(first, last);
    }

    template<class InputIt>
    basic_indexed_set(InputIt first, InputIt last, const allocator_type& alloc)
        : basic_indexed_set(first, last, key_compare(), alloc) {}

    basic_indexed_set(std::initializer_list<value_type> init,
                      const key_compare& comp = key_compare(),
                      const allocator_type& alloc = allocator_type())
        : basic_indexed_set(init.begin(), init.end(), comp, alloc) {}

    basic_indexed_set(std::initializer_list<value_type> init, const allocator_type& alloc)
        : basic_indexed_set(init.begin(), init.end(), key_compare(), alloc) {}

    basic_indexed_set(const basic_indexed_set& other)
        : root_(nullptr), size_(0), comp_(other.comp_),
          alloc_(node_traits::select_on_container_copy_construction(other.alloc_)) {
        root_ = clone_subtree(other.root_, nullptr);
        size_ = other.size_;
    }

    basic_indexed_set(const basic_indexed_set& other, const allocator_type& alloc)
        : root_(nullptr), size_(0), comp_(other.comp_), alloc_(alloc) {
        root_ = clone_subtree(other.root_, nullptr);
        size_ = other.size_;
    }

    basic_indexed_set(basic_indexed_set&& other) noexcept
        : root_(std::exchange(other.root_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          comp_(std::move(other.comp_)),
          alloc_(std::move(other.alloc_)) {
        if (root_) root_->parent = nullptr;
    }

    basic_indexed_set(basic_indexed_set&& other, const allocator_type& alloc)
        : basic_indexed_set(key_compare(std::move(other.comp_)), alloc) {
        if (allocator_type(other.alloc_) == alloc) {
            clear();
            root_ = std::exchange(other.root_, nullptr);
            size_ = std::exchange(other.size_, 0);
            if (root_) root_->parent = nullptr;
        } else {
            insert(other.begin(), other.end());
            other.clear();
        }
    }

    ~basic_indexed_set() { clear(); }

    basic_indexed_set& operator=(const basic_indexed_set& other) {
        if (this == &other) return *this;
        basic_indexed_set tmp(other, allocator_type(alloc_));
        tmp.comp_ = other.comp_;
        swap(tmp);
        return *this;
    }

    basic_indexed_set& operator=(basic_indexed_set&& other) noexcept(
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

    basic_indexed_set& operator=(std::initializer_list<value_type> init) {
        clear();
        insert(init);
        return *this;
    }

    allocator_type get_allocator() const noexcept { return allocator_type(alloc_); }

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

    template<class... Args>
    auto emplace(Args&&... args) {
        node* fresh = make_node(std::forward<Args>(args)...);
        if constexpr (Multi) {
            return iterator(attach_equal_node(fresh), this);
        } else {
            auto [pos, inserted] = attach_unique_node(fresh);
            if (!inserted) destroy_node(fresh);
            return std::pair<iterator, bool>{iterator(pos, this), inserted};
        }
    }

    template<class... Args>
    iterator emplace_hint(const_iterator, Args&&... args) {
        if constexpr (Multi) {
            return emplace(std::forward<Args>(args)...);
        } else {
            return emplace(std::forward<Args>(args)...).first;
        }
    }

    auto insert(const value_type& x) { return emplace(x); }
    auto insert(value_type&& x) { return emplace(std::move(x)); }

    iterator insert(const_iterator hint, const value_type& x) {
        (void)hint;
        if constexpr (Multi) return insert(x);
        else return insert(x).first;
    }

    iterator insert(const_iterator hint, value_type&& x) {
        (void)hint;
        if constexpr (Multi) return insert(std::move(x));
        else return insert(std::move(x)).first;
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) insert(*first);
    }

    void insert(std::initializer_list<value_type> init) {
        insert(init.begin(), init.end());
    }

    auto insert(node_type&& nh) {
        if constexpr (Multi) {
            if (nh.empty()) return end();
            if (!(nh.get_allocator() == get_allocator())) {
                throw std::invalid_argument("indexed_multiset::insert(node_type): allocator mismatch");
            }
            node* p = std::exchange(nh.ptr_, nullptr);
            return iterator(attach_equal_node(p), this);
        } else {
            if (nh.empty()) return insert_return_type{end(), false, node_type{}};
            if (!(nh.get_allocator() == get_allocator())) {
                throw std::invalid_argument("indexed_set::insert(node_type): allocator mismatch");
            }
            node* p = nh.ptr_;
            auto [pos, inserted] = attach_unique_node(p);
            if (inserted) {
                nh.ptr_ = nullptr;
                return insert_return_type{iterator(pos, this), true, node_type{}};
            }
            return insert_return_type{iterator(pos, this), false, std::move(nh)};
        }
    }

    iterator insert(const_iterator, node_type&& nh) {
        if (nh.empty()) return end();
        if (!(nh.get_allocator() == get_allocator())) {
            throw std::invalid_argument("indexed_set::insert(hint, node_type): allocator mismatch");
        }
        if constexpr (Multi) {
            node* p = std::exchange(nh.ptr_, nullptr);
            return iterator(attach_equal_node(p), this);
        } else {
            node* p = nh.ptr_;
            auto [pos, inserted] = attach_unique_node(p);
            if (inserted) nh.ptr_ = nullptr;
            return iterator(pos, this);
        }
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

    iterator erase(const_iterator position) noexcept {
        assert(position.ptr_ && position.owner_ == this);
        iterator next = position;
        ++next;
        erase_node(position.ptr_);
        return next;
    }

    size_type erase(const key_type& key) {
        auto [first, last] = equal_range(key);
        size_type removed = 0;
        while (first != last) {
            first = erase(first);
            ++removed;
        }
        return removed;
    }

    template<bool B = Multi, class = std::enable_if_t<B>>
    size_type erase1(const key_type& key) {
        iterator it = find(key);
        if (it == end()) return 0;
        erase(it);
        return 1;
    }

    iterator erase(const_iterator first, const_iterator last) noexcept {
        while (first != last) first = erase(first);
        return iterator(last.ptr_, this);
    }

    void swap(basic_indexed_set& other) noexcept(
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

    void clear() noexcept {
        clear_subtree(root_);
        root_ = nullptr;
        size_ = 0;
    }

    template<class C2, bool M2>
    void merge(basic_indexed_set<key_type, C2, allocator_type, M2>& source) {
        if (this == reinterpret_cast<const basic_indexed_set*>(&source)) return;
        if (!(get_allocator() == source.get_allocator())) {
            throw std::invalid_argument("indexed_set::merge: allocator mismatch");
        }
        for (auto it = source.begin(); it != source.end(); ) {
            node* p = it.ptr_;
            ++it;
            if constexpr (!Multi) {
                if (find(p->value) != end()) continue;
            }
            source.unlink_node(p);
            if constexpr (Multi) {
                attach_equal_node(p);
            } else {
                auto [pos, inserted] = attach_unique_node(p);
                (void)pos;
                assert(inserted);
            }
        }
    }

    template<class C2, bool M2>
    void merge(basic_indexed_set<key_type, C2, allocator_type, M2>&& source) {
        merge(source);
    }

    key_compare key_comp() const { return comp_; }
    value_compare value_comp() const { return comp_; }

    iterator find(const key_type& key) { return iterator(find_node(key), this); }
    const_iterator find(const key_type& key) const { return const_iterator(find_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    iterator find(const K& key) { return iterator(find_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    const_iterator find(const K& key) const { return const_iterator(find_node(key), this); }

    size_type count(const key_type& key) const {
        if constexpr (!Multi) return contains(key) ? 1u : 0u;
        else return order_of_upper_key(key) - order_of_key(key);
    }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    size_type count(const K& key) const {
        if constexpr (!Multi) return contains(key) ? 1u : 0u;
        else return order_of_upper_key(key) - order_of_key(key);
    }

    bool contains(const key_type& key) const { return find(key) != end(); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    bool contains(const K& key) const { return find(key) != end(); }

    iterator lower_bound(const key_type& key) { return iterator(lower_bound_node(key), this); }
    const_iterator lower_bound(const key_type& key) const { return const_iterator(lower_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    iterator lower_bound(const K& key) { return iterator(lower_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    const_iterator lower_bound(const K& key) const { return const_iterator(lower_bound_node(key), this); }

    iterator upper_bound(const key_type& key) { return iterator(upper_bound_node(key), this); }
    const_iterator upper_bound(const key_type& key) const { return const_iterator(upper_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    iterator upper_bound(const K& key) { return iterator(upper_bound_node(key), this); }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    const_iterator upper_bound(const K& key) const { return const_iterator(upper_bound_node(key), this); }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
    std::pair<iterator, iterator> equal_range(const K& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    template<class K, class C = key_compare,
             std::enable_if_t<detail::has_is_transparent<C>::value, int> = 0>
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

    const_reference at(size_type index) const {
        node* p = nth_node(index);
        if (!p) throw std::out_of_range("indexed_set::at: index out of range");
        return p->value;
    }

    const_reference operator[](size_type index) const noexcept {
        return *nth(index); // Undefined behavior if index >= size().
    }

    template<class K>
    size_type order_of_key(const K& key) const {
        size_type ans = 0;
        node* cur = root_;
        while (cur) {
            if (comp_(cur->value, key)) {
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
        if (it.owner_ != this || !it.ptr_) throw std::invalid_argument("indexed_set::index_of: foreign iterator");
        return rank_node(it.ptr_);
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
        if (prev) {
            if constexpr (Multi) {
                if (comp_(p->value, prev->value)) ok = false;
            } else {
                if (!comp_(prev->value, p->value)) ok = false;
            }
        }
        prev = p;
        ++counted;
        int rh = verify_rec(p->right, p, counted, ok, prev);
        if (p->height != 1 + std::max(lh, rh)) ok = false;
        if (p->subtree_size != 1 + subtree_size(p->left) + subtree_size(p->right)) ok = false;
        if (lh - rh < -1 || lh - rh > 1) ok = false;
        return 1 + std::max(lh, rh);
    }
};

template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
using indexed_set = basic_indexed_set<Key, Compare, Allocator, false>;

template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
using indexed_multiset = basic_indexed_set<Key, Compare, Allocator, true>;

template<class Key, class Compare, class Allocator, bool Multi>
bool operator==(const basic_indexed_set<Key, Compare, Allocator, Multi>& a,
                const basic_indexed_set<Key, Compare, Allocator, Multi>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

template<class Key, class Compare, class Allocator, bool Multi>
auto operator<=>(const basic_indexed_set<Key, Compare, Allocator, Multi>& a,
                 const basic_indexed_set<Key, Compare, Allocator, Multi>& b) {
    return std::lexicographical_compare_three_way(
        a.begin(), a.end(), b.begin(), b.end(), detail::synth_three_way{});
}

template<class Key, class Compare, class Allocator, bool Multi>
void swap(basic_indexed_set<Key, Compare, Allocator, Multi>& a,
          basic_indexed_set<Key, Compare, Allocator, Multi>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

template<class Key, class Compare, class Allocator, bool Multi, class Predicate>
typename basic_indexed_set<Key, Compare, Allocator, Multi>::size_type
erase_if(basic_indexed_set<Key, Compare, Allocator, Multi>& c, Predicate pred) {
    auto old = c.size();
    for (auto it = c.begin(); it != c.end(); ) {
        if (pred(*it)) it = c.erase(it);
        else ++it;
    }
    return old - c.size();
}

} // namespace avl

