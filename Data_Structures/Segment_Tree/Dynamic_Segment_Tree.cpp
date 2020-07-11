#include <memory>
#include <functional>

template<class T>
struct dynamic_segment_tree {
    using value_type = T;
    using const_reference = const value_type&;
    using F = std::function<T(T, T)>;
    using G = std::function<T(T, T)>;
    using size_type = long long;
private :
    struct node_type {
    public :
        value_type value;
        std::unique_ptr<node_type> left, right;
        node_type (const value_type &value) :
            value(value), left(nullptr), right(nullptr) { }
    };

    using node_ptr = std::unique_ptr<node_type>;

    size_type n;
    const F f;
    const G g;
    value_type ie;
    node_ptr root;

    value_type get_value (const node_ptr &node) const noexcept {
        return (node ? (node->value) : ie);
    }

    value_type set_val (size_type i, const_reference x, node_ptr &node, size_type l, size_type r) {
        if (r - l == 1) return node->value = g(node->value, x);
        const size_type half = ((l + r) >> 1);
        if (i < half) {
            if (not node->left) node->left = std::make_unique<node_type>(ie);
            return node->value = f(set_val(i, x, node->left, l, half), get_value(node->right));
        } else {
            if (not node->right) node->right = std::make_unique<node_type>(ie);
            return node->value = f(get_value(node->left), set_val(i, x, node->right, half, r));
        }
    }

    value_type fold (size_type a, size_type b, node_ptr &node, size_type l, size_type r) {
        if (a <= l and r <= b) return node->value;
        if (r <= a or  b <= l) return ie;
        const size_type half = ((l + r) >> 1);
        value_type left_value  = (node->left  ? fold(a, b, node->left,  l, half) : ie);
        value_type right_value = (node->right ? fold(a, b, node->right, half, r) : ie);
        return f(left_value, right_value);
    }

public : 
    dynamic_segment_tree (const F &f, const G &g, const_reference ie) :
        f(f), g(g), ie(ie), root(std::make_unique<node_type>(ie)) { }

    void build (const size_type _n) noexcept {
        n = size_type(1);
        while (n < _n) n <<= 1;
    }

    value_type set_val (size_type i, const_reference x) {
        return set_val(i, x, root, 0, n);
    }

    value_type fold (size_type l, size_type r) {
        return fold(l, r, root, 0, n);
    }

};