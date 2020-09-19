#include <memory>
#include <functional>
#include <cstdint>

template<class Tp>
struct dynamic_segment_tree {
	using const_reference = const Tp&;
	using binary_op = std::function<Tp(Tp, Tp)>;
	using size_type = std::uint_fast64_t;
	using value_type = Tp;
private :
	struct node_type {
	public :
		value_type value;
		std::unique_ptr<node_type> left, right;
		node_type (const value_type &value)
		noexcept : value(value), left(nullptr), right(nullptr) { }
	};

	using node_ptr = std::unique_ptr<node_type>;

	size_type n;
	node_ptr root;
	const binary_op op;
	const value_type identity;

	const value_type &get (const node_ptr &node) const noexcept {
		return (node ? (node->value) : identity);
	}

	void make_node (node_ptr &node) noexcept {
		if (not node) node = std::make_unique<node_type>(identity);
	}

	value_type update (size_type i, const_reference x, node_ptr &node, size_type l, size_type r) {
		if (r - l == 1) return node->value = x;
		const size_type mid = ((l + r) >> 1);
		if (i < mid) {
			make_node(node->left);
			return node->value = op(update(i, x, node->left, l, mid), get(node->right));
		} else {
			make_node(node->right);
			return node->value = op(get(node->left), update(i, x, node->right, mid, r));
		}
	}

	value_type fold (size_type a, size_type b, node_ptr &node, size_type l, size_type r) {
		if (a <= l and r <= b) return node->value;
		if (r <= a or  b <= l) return identity;
		const size_type mid = ((l + r) >> 1);
		value_type left_value  = (node->left ? fold(a, b, node->left,  l, mid) : identity);
		value_type right_value = (node->right ? fold(a, b, node->right, mid, r) : identity);
		return op(left_value, right_value);
	}

public :
	dynamic_segment_tree (const_reference identity, const binary_op &op)
	noexcept : n(0), root(std::make_unique<node_type>(identity)), op(op), identity(identity) { }

	dynamic_segment_tree (size_type _size, const_reference identity, const binary_op &op)
	noexcept : root(std::make_unique<node_type>(identity)), op(op), identity(identity) { assign(_size); }

	void assign (size_type _size) noexcept {
		n = size_type(1);
		while (n < _size) n <<= 1;
	}

	value_type fold (size_type first, size_type last) noexcept {
		return fold(first, last, root, 0, n);
	}

	void update (size_type index, const_reference value) noexcept {
		update(index, value, root, 0, n);
	}

	value_type operator[] (size_type index) noexcept {
		return fold(index, index + 1);
	}

};