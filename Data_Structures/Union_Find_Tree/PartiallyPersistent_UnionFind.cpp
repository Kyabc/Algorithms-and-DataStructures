#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>

struct partially_persistent_union_find {
    using size_type = std::size_t;
private :
    std::vector<int> node, last;
    std::vector<std::vector<std::pair<int, size_type>>> add;
public :
    partially_persistent_union_find (int n) : node(n, -1), last(n, (1 << 30)), add(n) {
        for (auto& v : add) v.emplace_back(-1, 1);
    }

    bool unite (int t, int x, int y) {
        if ((x = root(t, x)) == (y = root(t, y))) return false;
        if (node[x] > node[y]) std::swap(x, y);
        node[x] += node[y];
        add[x].emplace_back(t, size_type(-node[x]));
        node[y] = x;
        last[y] = t;
        return true;
    }

    bool same (int t, int x, int y) {
        return (root(t, x) == root(t, y));
    }

    int root (int t, int x) {
        while (t >= last[x]) x = node[x];
        return x;
    }

    size_type size (int t, int x) {
        x = root(t, x);
        const std::pair<int, size_type> p(t + 1, size_type(0));
        return std::prev(std::lower_bound(add[x].begin(), add[x].end(), p))->second;
    }

};