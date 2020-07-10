#include <vector>
#include <functional>

template<class T>
struct segment_tree {
	using value_type = T;
	using F = std::function<T(T, T)>;
private : 
	int n;
	F f;
	value_type ie;
	std::vector<value_type> node;

public : 
	segment_tree () { }

	segment_tree (const F &f, const value_type &ie) : f(f), ie(ie) { }

	void reset (const F &f, const value_type &ie) {
		(this->f) = f;
		(this->ie) = ie;
		node.clear();
	}

	void assign (int _n) noexcept {
		n = 1;
		while (n < _n) n <<= 1;
		node.assign(2 * n, ie);
	}

	void assign (int _n, const value_type &x) noexcept {
		assign(_n);
		std::fill(node.begin() + n, node.end(), x);
		for (int i = n; i--> 0;) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	template<class Iterator>
	void build (Iterator l, Iterator r) {
		assign(r - l);
		for (int i = n; l != r; i++, l++) node[i] = *l;
		for (int i = n; i--> 0;) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	template<class Sequence>
	void build (const Sequence &monoid) {
		build(monoid.begin(), monoid.end());
	}

	void set_val (int i, const value_type &x) {
		i += n;
		node[i] = x;
		for (i >>= 1; i > 0; i >>= 1) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	constexpr value_type fold (int l, int r) {
		value_type vl = ie, vr = ie;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) vl = f(vl, node[l++]);
			if (r & 1) vr = f(node[--r], vr);
		}
		return f(vl, vr);
	};

	constexpr const value_type& fold_all() const {
		return (node[1]);
	}

	constexpr const value_type& operator[] (int i) const {
		return node[i + n];
	}

	constexpr const value_type& at (int i) const {
		return node.at(i + n);
	}

};