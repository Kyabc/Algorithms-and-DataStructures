#include <memory>
#include <limits>
#include <functional>
#include <algorithm>

template<class T, T Id = std::numeric_limits<T>::max(), class Compare = std::less<T>>
struct dynamic_li_chao_tree {
	using value_type = T;
private :
	struct line;
	struct node_type;
	using node_ptr = std::unique_ptr<node_type>;

	static constexpr value_type identity = Id;
	node_ptr root;
	const Compare comp;
	value_type range_left, range_right;

	class line {
		value_type _a, _b;
		bool _flag;
	public :
		line () noexcept : _a(0), _b(0), _flag(false) { }
		line (value_type a, value_type b) noexcept : _a(a), _b(b), _flag(true) { }
		value_type operator() (value_type p) const noexcept {
			return (_flag ? _a * p + _b : identity);
		}
	};

	struct node_type {
		line f;
		node_ptr left, right;
		node_type () noexcept : f(), left(nullptr), right(nullptr) { }
		node_type (value_type a, value_type b)
		noexcept : f(a, b), left(nullptr), right(nullptr) { }
	};

	void update (value_type a, value_type b, node_ptr &node, line g, value_type l, value_type r) noexcept {
		if (l >= r or r <= a or b <= l) return;
		if (not node) node = std::make_unique<node_type>();
		const value_type mid = (l + r) / 2;
		if (a <= l and r <= b) {
			line &h = node->f;
			bool left = comp(g(l), h(l)), right = comp(g(r - 1), h(r - 1));
			if (left == right) { if (left) h = g; return; }
			if (comp(g(mid), h(mid))) { std::swap(g, h); std::swap(left, right); }
			if (left) update(a, b, node->left, g, l, mid);
			else update(a, b, node->right, g, mid, r);
		} else {
			update(a, b, node->left, g, l, mid);
			update(a, b, node->right, g, mid, r);
		}
	}

	value_type get (value_type p, node_ptr &node, value_type l, value_type r) noexcept {
		if (not node or p < l or r <= p) return identity;
		const line &h = node->f;
		if (r - l == 1) return h(p);
		value_type mid = (l + r) / 2;
		if (l <= p and p < mid) return std::min(h(p), get(p, node->left, l, mid));
		else return std::min(h(p), get(p, node->right, mid, r));
	}

public :
	dynamic_li_chao_tree (value_type left, value_type right)
	noexcept : root(std::make_unique<node_type>()), comp(), range_left(left), range_right(right) { }

	void add_line (value_type a, value_type b) noexcept {
		update(range_left, range_right, root, line(a, b), range_left, range_right);
	}

	void add_segment (value_type a, value_type b, value_type first, value_type last) noexcept {
		update(first, last, root, line(a, b), range_left, range_right);
	}

	value_type get (value_type x) noexcept {
		return get(x, root, range_left, range_right);
	}

};