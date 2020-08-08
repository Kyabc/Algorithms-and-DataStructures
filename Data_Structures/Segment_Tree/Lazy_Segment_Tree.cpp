#include <vector>
#include <functional>

template<class T, class E, class F, class G, class H>
struct lazy_segment_tree {
	using value_type = T;
	using operand_type = E;
	using size_type = std::size_t;
private :
	class node_type {
	public :
		value_type value;
		operand_type operand;
		node_type () = default;
		node_type (const value_type &value, const operand_type &operand) noexcept
			: value(value), operand(operand) { }
	};

	size_type n;
	const F f;
	const G g;
	const H h;
	const value_type value_identity;
	const operand_type operand_identity;
	std::vector<node_type> node;

	value_type reflect (const size_type &i) noexcept {
		if (node[i].operand == operand_identity) return node[i].value;
		return g(node[i].value, node[i].operand);
	}

	void merge (const size_type &i) noexcept {
		node[i].value = f(reflect(i << 1 | 0), reflect(i << 1 | 1));
	}

	void add (const size_type &i, const operand_type &operand) noexcept {
		node[i].operand = h(node[i].operand, operand);
	}

	void propagate (size_type i) noexcept {
		if (node[i].operand == operand_identity) return;
		add(i << 1 | 0, node[i].operand);
		add(i << 1 | 1, node[i].operand);
		node[i].value = reflect(i);
		node[i].operand = operand_identity;
	}

	void topdown_update (size_type i) noexcept {
		if (i == 0) return;
		for (size_type h = 32 - __builtin_clz(i); h; h--) {
			propagate(i >> h);
		}
	}

	void bottomup_update (size_type i) noexcept {
		if (i == 0) return;
		for (i >>= 1; i > 0; i >>= 1) {
			merge(i);
		}
	}

public :
	lazy_segment_tree (const value_type &value_identity, const operand_type &operand_identity, const F &f, const G &g, const H &h) noexcept :
		value_identity(value_identity), operand_identity(operand_identity), f(f), g(g), h(h) { }

	void assign (size_type _size) noexcept {
		n = _size;
		node.assign(_size << 1, node_type(value_identity, operand_identity));
	}

	void assign(size_type _size, const value_type &value) noexcept {
		assign(_size);
		for (size_type i = n; i < node.size(); i++) node[i].value = value;
		for (size_type i = n; i --> 0;) merge(i);
	}

	template<class InputIterator>
	void assign (InputIterator first, InputIterator last) noexcept {
		assign(last - first);
		for (size_type i = n; first != last; first++, i++) node[i].value = (*first);
		for (size_type i = n; i --> 0;) merge(i);
	}

	void update (size_type first, size_type last, const operand_type &operand) noexcept {
		const size_type _first = first + n, _last = last + n - 1;
		topdown_update(_first);
		topdown_update(_last);
		for (first += n, last += n; first != last; first >>= 1, last >>= 1) {
			if (first & 1) add(first++, operand);
			if (last  & 1) add(--last,  operand);
		}
		bottomup_update(_first);
		bottomup_update(_last);
	}

	value_type fold (size_type first, size_type last) noexcept {
		topdown_update(first + n);
		topdown_update(last + n - 1);
		value_type left = value_identity, right = value_identity;
		for (first += n, last += n; first != last; first >>= 1, last >>= 1) {
			if (first & 1) left = f(left, reflect(first++));
			if (last & 1) right = f(reflect(--last), right);
		}
		return f(left, right);
	}

};

template<class InputIterator, class T, class E, class F, class G, class H>
lazy_segment_tree<T, E, F, G, H> make_lazysegtree (InputIterator first, InputIterator last, const T &value_identity, const E &operand_identity, const F &f, const G &g, const H &h) {
	lazy_segment_tree<T, E, F, G, H> tree(value_identity, operand_identity, f, g, h);
	tree.assign(first, last);
	return std::move(tree);
}