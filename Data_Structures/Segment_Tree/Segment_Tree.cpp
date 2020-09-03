#include <vector>
#include <functional>
#include <cstddef>
#include <cassert>

template<class T>
struct segment_tree {
	using const_reference = const T&;
	using size_type = std::size_t;
	using value_type = T;
	using binary_op = std::function<T(T, T)>;
private :
	const binary_op op;
	const value_type identity;
	std::vector<value_type> node;

	void merge (size_type i) noexcept {
		node[i] = op(node[i << 1 | 0], node[i << 1 | 1]);
	}

public :
	segment_tree (const_reference identity, const binary_op &op)
	: identity(identity), op(op) { }

	segment_tree (size_type n, const_reference identity, const binary_op &op)
	: node(n << 1, identity), identity(identity), op(op) { }

	segment_tree (size_type n, const_reference value, const_reference identity, const binary_op &op)
	: node(n << 1, value), identity(identity), op(op) {
		for (size_type i = n; i > 1; i--) merge(i - 1);
	}

	template<class InputIterator>
	explicit segment_tree (InputIterator first, InputIterator last, const_reference identity, const binary_op &op)
	: node((last - first) << 1), identity(identity), op(op) {
		assign(last - first);
		for (size_type i = size(); first != last; i++, first++) node[i] = (*first);
		for (size_type i = size(); i > 1; i--) merge(i - 1);
	}

	size_type size () const noexcept {
		return (node.size() >> 1);
	}

	void assign (size_type n) noexcept {
		node.assign(n << 1, identity);
	}

	void assign (size_type n, const_reference value) noexcept {
		assign(n);
		for (size_type i = n; i < node.size(); i++) node[i] = value;
		for (size_type i = n; i > 1; i--) merge(i - 1);
	}

	template<class InputIterator>
	void assign (InputIterator first, InputIterator last) noexcept {
		assign(last - first);
		for (size_type i = size(); first != last; i++, first++) node[i] = (*first);
		for (size_type i = size(); i > 1; i--) merge(i - 1);
	}

	void update (size_type i,const_reference value) noexcept {
		node[(i += size())] = value;
		for (i >>= 1; i > 0; i >>= 1) merge(i);
	}

	value_type fold (size_type first, size_type last) noexcept {
		value_type vl = identity, vr = identity;
		for (first += size(), last += size(); first != last; first >>= 1, last >>= 1) {
			if (first & 1) vl = op(vl, node[first++]);
			if (last  & 1) vr = op(node[--last],  vr);
		}
		return op(vl, vr);
	}

	const_reference operator[] (size_type i) const noexcept {
		return node[i + size()];
	}

	const_reference at (size_type i) const noexcept {
		assert(i < size());
		return node[i + size()];
	}

};

template<class T, class F>
segment_tree<T> make_segtree (const T &identity, const F &op) {
	return segment_tree<T>(identity, op);
}

template<class T, class F>
segment_tree<T> make_segtree (std::size_t n, const T &identity, const F &op) {
	return segment_tree<T>(n, identity, op);
}

template<class T, class F>
segment_tree<T> make_segtree (std::size_t n, const T &value, const T &identity, const F &op) {
	return segment_tree<T>(n, value, identity, op);
}

template<class InputIterator, class T, class F>
segment_tree<T> make_segtree (InputIterator first, InputIterator last, const T &identity, const F &op) {
	return segment_tree<T>(first, last, identity, op);
}