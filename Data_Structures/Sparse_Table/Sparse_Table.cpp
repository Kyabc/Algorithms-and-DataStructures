#include <vector>
#include <functional>
#include <cassert>

template<class T>
struct sparse_table {
	using value_type = T;
	using size_type = std::size_t;
	using F = std::function<T(T, T)>;
private :
	const F f;
	std::vector<std::vector<value_type>> node;
	std::vector<size_type> msb;

public : 
	sparse_table (const F &f) : f(f) { }

	template<class Iterator>
	void build (Iterator l, Iterator r) {
		const size_type n = (r - l), b = 32 - __builtin_clz(n);
		msb.assign(n + 1, 0);
		node.assign(b, std::vector<value_type>(n));
		for (size_type i = 2; i < n + 1; i++) msb[i] = msb[i >> 1] + 1;
		for (size_type i = 0; i < n; i++, l++) node[0][i] = (*l);
		for (size_type i = 1; i < b; i++) {
			for (size_type j = 0; j < n; j++) {
				const size_type k = std::min(j + (1 << (i - 1)), n - 1);
				node[i][j] = f(node[i - 1][j], node[i - 1][k]);
			}
		}
	}

	value_type fold (size_type l, size_type r) {
		assert(l < r);
		const size_type &b = msb[r - l];
		return f(node[b][l], node[b][r - (1 << b)]);
	}

};