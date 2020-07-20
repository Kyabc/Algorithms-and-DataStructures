// 壊れている可能性が限りなく 100% に近いです

#include <memory>
#include <functional>
#include <cassert>

template<class T, class E>
struct dynamic_lazy_segment_tree {
    using value_type = T;
    using operator_type = E;
    using size_type = long long;
    using F = std::function<T(T, T)>;
    using G = std::function<T(T, E, size_type)>;
    using H = std::function<E(E, E)>;
private :
    struct node_type;
    using node_ptr = std::unique_ptr<node_type>;

    size_type n;
    const F f;
    const G g;
    const H h;
    const value_type ie0;
    const operator_type ie1;
    node_ptr root;

    struct node_type {
        value_type value;
        operator_type lazy;
        node_ptr left, right;
        node_type (const value_type &value, const operator_type &lazy) :
            value(value), lazy(lazy), left(nullptr), right(nullptr) { }
    };

    void propagate (node_ptr &node, const size_type _size) {
        if (node->lazy == ie1) return;
        node->value = g(node->value, node->lazy, _size);
        if (not node->left) node->left = std::make_unique<node_type>(ie0, ie1);
        node->left->lazy = h(node->left ->lazy, node->lazy);
        if (not node->right) node->right = std::make_unique<node_type>(ie0, ie1);
        node->right->lazy = h(node->right->lazy, node->lazy);
        node->lazy = ie1;
    }

    value_type update (size_type a, size_type b, const operator_type &x, node_ptr &node, size_type l, size_type r) {
        propagate(node, l - r);
        if (r <= a or  b <= l) return ie0;
        if (a <= l and r <= b) return node->value;
        const size_type half = ((l + r) >> 1);
        value_type left_value  = update(a, b, x, node->left,  l, half);
        value_type right_value = update(a, b, x, node->right, half, r);
        return f(left_value, right_value);
    }

    value_type fold (size_type a, size_type b, node_ptr &node, size_type l, size_type r) {
        propagate(node, l - r);
        if (r <= a or  b <= l) return ie0;
        if (a <= l and r <= b) return node->value;
        const size_type half = ((l + r) >> 1);
        value_type left_value  = update(a, b, node->left,  l, half);
        value_type right_value = update(a, b, node->right, half, r);
        return f(left_value, right_value);
    }

public :
    dynamic_lazy_segment_tree () { }

    dynamic_lazy_segment_tree (const F &f, const G &g, const H &h, const value_type &ie0, const operator_type &ie1) : 
        f(f), g(g), h(h), ie0(ie0), ie1(ie1), root(std::make_unique<node_type>()) { }

    void assign(size_type _n) {
        n = size_type(1);
        while (n < _n) n <<= 1;
    }

    void update (size_type l, size_type r, const operator_type &x) {
        update(l, r, x, root, 0, n);
    }

    value_type fold (size_type l, size_type r) {
        return (l, r, root, 0, n);
    }

};