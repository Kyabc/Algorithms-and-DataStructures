#include <vector>
#include <functional>

template<class T, class F>
struct segment_tree {
	using value_type = T;
	using size_type = std::size_t;
private : 
	size_type n;
	const F op;
	const value_type identity;
	std::vector<value_type> node;

	void calc (size_type i) noexcept {
		node[i] = op(node[i << 1 | 0], node[i << 1 | 1]);
	}

public : 
	explicit constexpr segment_tree (const value_type &identity, const F &op) noexcept : identity(identity), op(op) { }

	void assign (size_type _size) noexcept {
		n = _size;
		node.assign(n << 1, identity);
	}

	void assign (size_type _size, const value_type &value) noexcept {
		assign(_size);
		std::fill(node.begin() + n, node.end(), value);
		for (int i = n; i--> 0;) calc(i);
	}

	template<class InputIterator>
	void assign (InputIterator first, InputIterator last) noexcept {
		assign(last - first);
		for (size_type i = n; first != last; i++, first++) node[i] = (*first);
		for (int i = n; i --> 0;) calc(i);
	}

	void set_val (size_type i, const value_type &value) noexcept {
		node[(i += n)] = value;
		for (i >>= 1; i > 0; i >>= 1) calc(i);
	}

	value_type fold (size_type first, size_type last) noexcept {
		value_type vl = identity, vr = identity;
		for (first += n, last += n; first != last; first >>= 1, last >>= 1) {
			if (first & 1) vl = op(vl, node[first++]);
			if (last  & 1) vr = op(node[--last],  vr);
		}
		return op(vl, vr);
	};

	const value_type& operator[] (size_type i) const noexcept {
		return node[i + n];
	}

	const value_type& at (size_type i) const noexcept {
		return node.at(i + n);
	}

};

template<class InputIterator, class T, class F>
segment_tree<T, F> make_segtree (InputIterator first, InputIterator last, const T &identity, const F &op) {
	segment_tree<T, F> segtree(identity, op);
	segtree.assign(first, last);
	return std::move(segtree);
}