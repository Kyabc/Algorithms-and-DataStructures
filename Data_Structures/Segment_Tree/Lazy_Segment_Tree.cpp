#include <vector>
#include <functional>


template<class T, class E>
struct lazy_segment_tree {
	using F = std::function<T(T, T)>;
	using G = std::function<T(T, E)>;
	using H = std::function<E(E, E)>;
	using value_type = T;
	using operator_type = E;
private : 
	int n, hight;
	F f;
	G g;
	H h;
	value_type ie0;
	operator_type ie1;
	std::vector<value_type> node;
	std::vector<operator_type> lazy;

	constexpr value_type reflect (int i) noexcept {
		return ((lazy[i] == ie1) ? node[i] : g(node[i], lazy[i]));
	}

	void propagate (int i) {
		if (lazy[i] == ie1) return;
		lazy[i << 1 | 0] = h(lazy[i << 1 | 0], lazy[i]);
		lazy[i << 1 | 1] = h(lazy[i << 1 | 1], lazy[i]);
		node[i] = reflect(i);
		lazy[i] = ie1;
	}

	void apply (int i) {
		for (int j = hight; j > 0; j--) {
			propagate(i >> j);
		}
	}

	void recalc (int i) {
		while (i >>= 1) {
			node[i] = f(reflect(i << 1 | 0), reflect(i << 1 | 1));
		}
	}

public : 
	lazy_segment_tree () { }

	lazy_segment_tree (const F &f, const G &g, const H &h, const value_type &ie0, const operator_type &ie1) : 
		f(f), g(g), h(h), ie0(ie0), ie1(ie1) { }

	void reset (const F &f, const G &g, const H &h, const value_type &ie0, const operator_type &ie1) {
		(this->f) = f; (this->g) = g; (this->h) = h;
		(this->ie0) = ie0; (this->ie1) = ie1;
		node.clear(); lazy.clear();
	}

	void assign (int _n) noexcept {
		n = 1; hight = 0;
		while (n < _n) { n <<= 1; hight++; }
		node.assign(2 * n, ie0);
		lazy.assign(2 * n, ie1);
	}

	void assign (int _n, const value_type &x) noexcept {
		assign(_n);
		std::fill(node.begin() + n, node.end(), x);
		for (int i = n; i--> 0;) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	template<class iter>
	void build (iter l, iter r) noexcept {
		assign(r - l);
		for (int i = n; l != r; i++, l++) node[i] = (*l);
		for (int i = n - 1; i > 0; i--) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	void update (int a, int b, const operator_type &x) {
		apply(a + n);
		apply(b + n - 1);
		for (int l = a + n, r = b + n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) { lazy[l] = h(lazy[l], x); l++; }
			if (r & 1) { r--; lazy[r] = h(lazy[r], x); }
		}
		recalc(a + n);
		recalc(b + n - 1);
	}

	value_type fold (int l, int r) {
		apply(l + n);
		apply(r + n - 1);
		value_type vl = ie0, vr = ie0;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) { vl = f(vl, reflect(l)); l++; }
			if (r & 1) { r--; vr = f(reflect(r), vr); }
		}
		return f(vl, vr);
	}

};