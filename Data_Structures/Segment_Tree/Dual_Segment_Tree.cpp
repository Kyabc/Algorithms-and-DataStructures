#include <vector>
#include <functional>
#include <algorithm>

template<class T>
struct dual_segment_tree {
	using F = std::function<T(T, T)>;
	using value_type = T;
private : 
	int n, hight;
	const F f;
	const value_type ie;
	std::vector<value_type> node;

	void propagate (int i) {
		if (node[i] == ie) return;
		node[i << 1 | 0] = f(node[i << 1 | 0], node[i]);
		node[i << 1 | 1] = f(node[i << 1 | 1], node[i]);
		node[i] = ie;
	}

	void topdown_update (int i) {
		for (int j = hight; j > 0; j--) propagate(i >> j);
	}

public : 
	dual_segment_tree (const F &f, const value_type &ie) : f(f), ie(ie) { }
	
	void assign (int _n) {
		n = 1; hight = 0;
		while (n < _n) { n <<= 1; hight++; }
		node.assign(2 * n, ie);
	}

	void assign (int _n, const value_type &x) {
		assign(_n);
		for (int i = n; i < 2 * n; i++) node[i] = x;
	}
	
	template<class Iterator>
	void build (Iterator l, Iterator r) {
		assign(r - l);
		for (int i = n; l != r; i++, l++) node[i] = (*l);
	}

	void update (int a, int b, const value_type &x) {
		topdown_update(a + n); topdown_update(b + n - 1);
		for (int l = a + n, r = b + n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) { node[l] = f(node[l], x); l++; }
			if (r & 1) { r--; node[r] = f(node[r], x); }
		}
	}

	const value_type &operator[] (int i) {
		topdown_update(i + n);
		return node[i + n];
	}

	const value_type &at (int i) {
		topdown_update(i + n);
		return (node.at(i));
	}

};