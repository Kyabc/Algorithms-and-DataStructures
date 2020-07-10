#include <vector>

struct union_find {
private :
    std::size_t sz;
    std::vector<int> node;
public :
    union_find (int sz) : node(sz, -1), sz(sz) { }

    bool unite (int x, int y) {
        if ((x = root(x)) == (y = root(y))) return false;
        if (node[x] > node[y]) std::swap(x, y);
        node[x] += node[y];
        node[y] = x;
        sz--;
        return true;
    }

    bool same (int x, int y) { return root(x) == root(y); }

    int root (int x) { return node[x] < 0 ? x : node[x] = root(node[x]); }

    std::size_t size (int x) { return (std::size_t(-node[root(x)])); }

    std::size_t size () { return sz; }

};