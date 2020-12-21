#include <vector>
#include <functional>
#include <cassert>

template<class Tp>
struct segment_tree {
	using reference = Tp&;
	using const_reference = const Tp&;
	using size_type = size_t;
	using value_type = Tp;
	using binary_op = std::function<Tp(Tp, Tp)>;
private :
	value_type identity;
	binary_op op;
	size_type last_referenced;
	std::vector<value_type> node;

	void merge (size_type index) noexcept {
		node[index] = op(node[index << 1 | 0], node[index << 1 | 1]);
	}

	void update (size_type index, size_type next_index = 0) noexcept {
		for (index >>= 1; index > 0; index >>= 1) merge(index);
		last_referenced = next_index;
	}

public :
	segment_tree () = default;

	segment_tree (const_reference identity, const binary_op &op)
	noexcept : identity(identity), op(op), node() { }

	explicit segment_tree (size_type n, const_reference identity, const binary_op &op)
	noexcept : identity(identity), op(op) { assign(n); }

	segment_tree (size_type n, const_reference value, const_reference identity, const binary_op &op)
	noexcept : identity(identity), op(op) { assign(n, value); }

	template<class InputIter>
	segment_tree (InputIter first, InputIter last, const_reference identity, const binary_op &op)
	noexcept : identity(identity), op(op) { assign(first, last); }

	size_type size () const noexcept {
		return (node.size() >> 1);
	}

	void assign (size_type n) noexcept {
		last_referenced = 0;
		node.assign(n << 1, identity);
	}

	void assign (size_type n, const_reference value) noexcept {
		assign(n);
		for (size_type i = size(); i < node.size(); i++) node[i] = value;
		for (size_type i = size(); i > 1; i--) merge(i - 1);
	}

	template<class InputIterator>
	void assign (InputIterator first, InputIterator last) noexcept {
		assign(std::distance(first, last));
		for (size_type i = size(); i < node.size(); i++, first++) node[i] = *first;
		for (size_type i = size(); i > 1; i--) merge(i - 1);
	}

	void set (const_reference new_identity, const binary_op &new_op) noexcept {
		identity = new_identity; op = new_op;
	}

	value_type fold (size_type first, size_type last) noexcept {
		update(last_referenced);
		value_type left = identity, right = identity;
		for (first += size(), last += size(); first < last; first >>= 1, last >>= 1) {
			if (first & 1) left = op(left, node[first++]);
			if (last & 1) right = op(node[--last], right);
		}
		return op(left, right);
	}

	reference operator[] (size_type index) noexcept {
		update(last_referenced, (index += size()));
		return node[index];
	}

	reference at (size_type index) noexcept {
		assert(index < size());
		return operator[](index);
	}

};