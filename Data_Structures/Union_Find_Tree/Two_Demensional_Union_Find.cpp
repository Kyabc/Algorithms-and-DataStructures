#include <vector>
#include <tuple>

struct two_demensional_union_find {
    using size_type = std::size_t;
    using point = std::pair<size_type, size_type>;
private :
    size_type _size;
    std::vector<std::vector<point>> par;
    std::vector<std::vector<size_type>> rank;

public : 
    two_demensional_union_find (size_type h, size_type w) : _size(h * w), par(h, std::vector<point>(w)), rank(h, std::vector<size_type>(w, 1)) {
        for (size_type i = 0; i < h; i++) {
            for (size_type j = 0; j < w; j++) {
                par[i][j] = point(i, j);
            }
        }
    }

    point root (size_type x, size_type y) {
        const point &p = par[x][y];
        if (p == point(x, y)) return p;
        return par[x][y] = root(p.first, p.second);
    }

    bool same (size_type x1, size_type y1, size_type x2, size_type y2) {
        return (root(x1, y1) == root(x2, y2));
    }

    bool unite (size_type x1, size_type y1, size_type x2, size_type y2) {
        std::tie(x1, y1) = root(x1, y1);
        std::tie(x2, y2) = root(x2, y2);
        if (x1 == x2 and y1 == y2) return false;
        if (rank[x1][y1] < rank[x2][y2]) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        rank[x1][y1] += rank[x2][y2];
        par[x2][y2] = point(x1, y1);
        _size--;
        return true;
    }

    const size_type &size (size_type x, size_type y) const noexcept {
        return (rank[x][y]);
    }

    const size_type &size () const noexcept {
        return (_size);
    }

};