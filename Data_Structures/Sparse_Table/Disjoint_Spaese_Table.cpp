#include <vector>
#include <functional>
#include <cassert>

template<class T>
struct disjoint_sparse_table {
    using value_type = T;
    using size_type = std::size_t;
    using F = std::function<T(T, T)>;
private :
    const F f;
    std::vector<std::vector<value_type>> node;
    std::vector<size_type> msb;

public :
    disjoint_sparse_table (const F &f) : f(f) { }

    template<class Iterator>
	void build (Iterator l, Iterator r) {
        const size_type n = (r - l), b = 32 - __builtin_clz(n);
        msb.assign(1 << b, 0);
		node.assign(b, std::vector<value_type>(n));
        for (size_type i = 2; i < msb.size(); i++) msb[i] = msb[i >> 1] + 1;
		for (size_type i = 0; i < n; i++, l++) node[0][i] = (*l);
        for (size_type i = 1; i < b; i++) {
            const size_type width = (1 << i);
            for (size_type j = 0; j < n; j += (width << 1)) {
                const size_type h = std::min(j + width, n);
                node[i][h - 1] = node[0][h - 1];
                for (size_type k = h - 1; k > j; k--) {
                    node[i][k - 1] = f(node[0][k - 1], node[i][k]);
                }
                if (h == n) break;
                const size_type t = std::min(h + width, n);
                node[i][h] = node[0][h];
                for (size_type k = h; k + 1 < t; k++) {
                    node[i][k + 1] = f(node[i][k], node[0][k + 1]);
                }
            }
        }
    }

    value_type fold (size_type l, size_type r) {
        assert(l < r);
        if (l == --r) return node[0][l];
        const size_type &b = msb[l ^ r];
        return f(node[b][l], node[b][r]);
    }

};