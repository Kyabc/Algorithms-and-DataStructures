#include <vector>
#include <functional>

template<class T>
struct segment_tree {
	using value_type = T;
	using size_type = std::size_t;
	using F = std::function<T(T, T)>;
private : 
	size_type n;
	const F f;
	const value_type ie;
	std::vector<value_type> node;

public : 
	segment_tree (const F &f, const value_type &ie) noexcept : f(f), ie(ie) { }

	void assign (size_type _n) noexcept {
		n = _n;
		node.assign(n << 1, ie);
	}

	void assign (size_type _n, const value_type &x) noexcept {
		assign(_n);
		std::fill(node.begin() + n, node.end(), x);
		for (int i = n; i--> 0;) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	template<class Iterator>
	void build (Iterator first, Iterator last) {
		assign(last - first);
		for (size_type i = n; first != last; i++, first++) node[i] = (*first);
		for (int i = n; i --> 0;) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	void set_val (size_type i, const value_type &x) {
		i += n;
		node[i] = x;
		for (i >>= 1; i > 0; i >>= 1) {
			node[i] = f(node[i << 1 | 0], node[i << 1 | 1]);
		}
	}

	value_type fold (size_type l, size_type r) {
		value_type vl = ie, vr = ie;
		for (l += n, r += n; l ^ r; l >>= 1, r >>= 1) {
			if (l & 1) vl = f(vl, node[l++]);
			if (r & 1) vr = f(node[--r], vr);
		}
		return f(vl, vr);
	};

	constexpr const value_type& fold_all() const noexcept {
		return (node[1]);
	}

	const value_type& operator[] (size_type i) const noexcept {
		return node[i + n];
	}

	const value_type& at (size_type i) const noexcept {
		return node.at(i + n);
	}

};