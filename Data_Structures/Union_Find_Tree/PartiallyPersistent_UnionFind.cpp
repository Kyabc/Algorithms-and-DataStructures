#include <vector>
#include <tuple>

struct partially_persistent_union_find {
private :
    std::vector<int> node, last;
    std::vector<std::vector<std::pair<int, int>>> add;
public :
    partially_persistent_union_find (int sz)
    : node(sz, -1), last(sz, 1e9), add(sz) {
        for (auto& v : add) v.emplace_back(-1, -1);
    }

    bool unite (int t, int x, int y) {
        if ((x = root(t, x)) == (y = root(t, y))) return false;
        if (node[x] > node[y]) std::swap(x, y);
        node[x] += node[y];
        add[x].emplace_back(t, node[x]);
        node[y] = x;
        last[y] = t;
        return true;
    }

    bool same (int t, int x, int y) {
        return ((x = root(t, x)) == (y = root(t, y)));
    }

    int root (int t, int x) {
        if (t < last[x]) return x;
        return root(t, node[x]);
    }

    std::size_t size (int t, int x) {
        x = root(t, x);
        return -std::prev(std::lower_bound(add[x].begin(), add[x].end(), std::make_pair(t, 0)))->second;
    }

};