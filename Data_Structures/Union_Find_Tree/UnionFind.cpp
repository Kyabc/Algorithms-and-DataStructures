#include <vector>

struct union_find {
    using size_type = std::size_t;
private :
    size_type _size;
    std::vector<int> node;
public :
    union_find (int n) : node(n, -1), _size(n) { }

    bool unite (int x, int y) {
        if ((x = root(x)) == (y = root(y))) return false;
        if (node[x] > node[y]) std::swap(x, y);
        node[x] += node[y];
        node[y] = x;
        _size--;
        return true;
    }

    bool same (int x, int y) {
        return (root(x) == root(y));
    }

    int root (int x) {
        if (node[x] < 0) return x;
        return node[x] = root(node[x]);
    }

    size_type size (size_type x) {
        return (size_type(-node[root(x)]));
    }

    const size_type &size () const noexcept {
        return _size;
    }

};
