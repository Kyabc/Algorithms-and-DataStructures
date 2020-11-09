#include <memory>
#include <cstdint>
#include <functional>

template<class Tp>
struct dynamic_segment_tree {
	using reference = Tp&;
	using const_reference = const Tp &;
	using size_type = std::uint_fast64_t;
	using binary_op = std::function<Tp(Tp, Tp)>;
	using value_type = Tp;
private :
	struct node_type;
	using node_ptr = std::unique_ptr<node_type>;

	size_type n;
	const Tp identity;
	const binary_op op;
	node_ptr root;

	struct node_type {
		value_type value;
		node_ptr left, right;
		node_type (const_reference value)
		noexcept : value(value), left(nullptr), right(nullptr) { }
	};

	const_reference get (const node_ptr &node) const noexcept {
		return (node ? node->value : identity);
	}

	void make_node (node_ptr &node) noexcept {
		node = std::make_unique<node_type>(identity);
	}

	value_type update (size_type i, const_reference x, node_ptr &node, size_type l, size_type r) noexcept {
		if (not node) make_node(node);
		if (r - l == 1) return node->value = x;
		const size_type mid = (l + r) >> 1;
		if (i < mid) return node->value = op(update(i, x, node->left, l, mid), get(node->right));
		else return node->value = op(get(node->left), update(i, x, node->right, mid, r));
	};

	value_type fold (size_type a, size_type b, node_ptr &node, size_type l, size_type r) const noexcept {
		if (a <= l and r <= b) return get(node);
		if (r <= a or  b <= l) return identity;
		const size_type mid = (l + r) >> 1;
		const value_type vl = (node->left ? fold(a, b, node->left, l, mid) : identity);
		const value_type vr = (node->right ? fold(a, b, node->right, mid, r) : identity);
		return op(vl, vr);
	}

public :
	dynamic_segment_tree (size_type n, const_reference identity, const binary_op &op)
	noexcept : n(n), identity(identity), op(op), root(std::make_unique<node_type>(identity)) { }

	void update (size_type index, const_reference x) noexcept {
		update(index, x, root, 0, n);
	}

	value_type fold (size_type first, size_type last) noexcept {
		return fold(first, last, root, 0, n);
	}

	value_type operator[] (const size_type &index) noexcept {
		return fold(index, index + 1);
	}

};