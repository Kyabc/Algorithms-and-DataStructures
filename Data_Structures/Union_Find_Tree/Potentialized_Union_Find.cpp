#include <vector>
#include <cassert>

template<class T>
struct potentialized_union_find {
    using value_type = T;
    using size_type = std::size_t;
private :
    size_type _size;
    std::vector<int> node;
    std::vector<value_type> potential;

public :
    potentialized_union_find (size_type n, const value_type &ie) : 
        _size(n), node(n, -1), potential(n, ie) { }

    size_type root (size_type x) {
        if (node[x] < 0) return x;
        int r = root(node[x]);
        potential[x] += potential[node[x]];
        return (node[x] = r);
    }

    value_type weight (size_type x) {
        root(x);
        return (potential[x]);
    }

    bool unite (size_type x, size_type y, value_type p) {
        p += (weight(x) - weight(y));
        if ((x = root(x)) == (y = root(y))) return false;
        if (node[x] > node[y]) {
            std::swap(x, y);
            p = -p;
        }
        node[x] += node[y];
        node[y] = x;
        potential[y] = p;
        _size--;
        return true;
    }

    bool same (size_type x, size_type y) {
        return (root(x) == root(y));
    }

    value_type diff (size_type x, size_type y) {
        assert(same(x, y));
        return (weight(y) - weight(x));
    }

    const size_type& size () const noexcept {
        return (_size);
    }

    size_type size (size_type x) {
        return (size_type(-node[root(x)]));
    }

};