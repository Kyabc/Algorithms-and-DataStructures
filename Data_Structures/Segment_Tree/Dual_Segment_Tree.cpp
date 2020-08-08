#include <vector>
#include <functional>

template<class T, class F>
struct dual_segment_tree {
	using value_type = T;
	using size_type = std::size_t;
private : 
	size_type n;
	const F op;
	const value_type identity;
	std::vector<value_type> node;

	void propagate (size_type i) noexcept {
		if (node[i] == identity) return;
		node[i << 1 | 0] = op(node[i << 1 | 0], node[i]);
		node[i << 1 | 1] = op(node[i << 1 | 1], node[i]);
		node[i] = identity;
	}

	void topdown_update (size_type i) noexcept {
		if (i == 0) return;
		const size_type ctz = __builtin_ctz(i);
		for (size_type h = 31 - __builtin_clz(i); h != ctz; h--) {
			propagate(i >> h);
		}
	}

public : 
	explicit constexpr dual_segment_tree (const value_type &identity, const F &op) noexcept : identity(identity), op(op) { }

	void assign (size_type _size) noexcept {
		n = _size;
		node.assign(n << 1, identity);
	}

	void assign (size_type _size, const value_type &value) noexcept {
		assign(_size);
		for (size_type i = n; i < (n << 1); i++) {
			node[i] = value;
		}
	}

	template<class InputIterator>
	void assign (InputIterator first, InputIterator last) noexcept {
		assign(last - first);
		for (size_type i = n; first != last; i++, first++) {
			node[i] = (*first);
		}
	}

	void update (size_type first, size_type last, const value_type &value) noexcept {
		topdown_update(first + n); topdown_update(last + n);
		for (first += n, last += n; first != last; first >>= 1, last >>= 1) {
			if (first & 1) { node[first] = op(node[first], value); first++; }
			if (last & 1) { last--; node[last] = op(node[last], value); }
		}
	}

	value_type fold (size_type i) noexcept {
		value_type value = node[(i += n)];
		for (i >>= 1; i > 0; i >>= 1) {
			value = op(value, node[i]);
		}
		return value;
	}

	value_type operator[] (size_type i) noexcept {
		return fold(i);
	}

	value_type at (size_type i) noexcept {
		return fold(i);
	}

};

template<class InputIterator, class T, class F>
dual_segment_tree<T, F> make_dualsegtree (InputIterator first, InputIterator last, const T &identity, const F &op) {
	dual_segment_tree<T, F> segtree(identity, op);
	segtree.assign(first, last);
	return segtree;
}