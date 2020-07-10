#include <memory>
#include <cassert>

template<class T = unsigned int, int B = 32>
struct binary_trie {
	using value_type = T;
	using size_type = std::size_t;
private :
	struct node {
		size_type count;
		value_type lazy;
		std::unique_ptr<node> next[2];
		node () : count(0), lazy(0), next{nullptr, nullptr} { }
	};

	using node_ptr = std::unique_ptr<node>;

	node_ptr root;

	constexpr size_type size (const node_ptr &x) const noexcept {
		return (x ? (x->count) : 0);
	}

	void propagate (node_ptr &x, int i) noexcept {
		if ((x->lazy) >> i & 1) (x->next[0]).swap(x->next[1]);
		if (x->next[0]) (x->next[0]->lazy) ^= (x->lazy);
		if (x->next[1]) (x->next[1]->lazy) ^= (x->lazy);
		(x->lazy) = 0;
	}

	size_type count (node_ptr &cur, const value_type &x, int i) {
		if (not cur) cur = std::make_unique<node>();
		if (i < 0) return (size(cur));
		if (cur->lazy) propagate(cur, i);
		return (count(cur->next[x >> i & 1], x, i - 1));
	}

	void insert (node_ptr &cur, const value_type &x, int i) {
		if (not cur) cur = std::make_unique<node>();
		(cur->count)++;
		if (i < 0) return;
		if (cur->lazy) propagate(cur, i);
		insert(cur->next[x >> i & 1], x, i - 1);
	}

	void erase (node_ptr &cur, const value_type &x, int i, const size_type &_size) {
		(cur->count) -= _size;
		if (i >= 0) erase(cur->next[x >> i & 1], x, i - 1, _size);
	}

	value_type min_element (node_ptr &cur, int i) {
		if (cur->lazy) propagate(cur, i);
		if (size(cur->next[0])) {
			if (i == 0) return value_type(0);
			return (min_element(cur->next[0], i - 1));
		} else {
			if (i == 0) return value_type(1);
			value_type min = (value_type(1) << i);
			return (min + min_element(cur->next[1], i - 1));
		}
	}

	value_type max_element (node_ptr &cur, int i) {
		if (cur->lazy) propagate(cur, i);
		if (size(cur->next[1])) {
			if (i == 0) return value_type(1);
			value_type max = (value_type(1) << i);
			return (max + max_element(cur->next[1], i - 1));
		} else {
			if (i == 0) return value_type(0);
			return max_element(cur->next[0], i - 1);
		}
	}

	value_type kth_element (node_ptr &cur, int i, size_type k) {
		if (cur->lazy) propagate(cur, i);
		const size_type _size = size(cur->next[0]);
		if (_size >= k) {
			if (i == 0) return value_type(0);
			return kth_element(cur->next[0], i - 1, k);
		} else {
			if (i == 0) return value_type(1);
			value_type kth = (value_type(1) << i);
			return (kth + kth_element(cur->next[1], i - 1, k - _size));
		}
	}

public : 
	explicit binary_trie () : root (std::make_unique<node>()) { }

	explicit binary_trie (binary_trie &&rhs) noexcept : root(std::move(rhs.root)) { }

	constexpr const size_type &size () const noexcept {
		return (root->count);
	}

	constexpr bool empty () const noexcept {
		return (root->count == 0);
	}

	size_type count (const value_type &x) {
		return (count(root, x, B - 1));
	}

	void insert (const value_type &x) {
		insert(root, x, B - 1);
	}

	bool erase (const value_type &x) {
		if (not count(x)) return false;
		erase(root, x, B - 1, 1);
		return true;
	}

	size_type erase_all (const value_type &x) {
		const size_type _size = count(x);
		if (_size) erase(root, x, B - 1, _size);
		return _size;
	}

	value_type min_element () {
		assert(not empty());
		return min_element(root, B - 1);
	}

	value_type max_element () {
		assert(not empty());
		return max_element(root, B - 1);
	}

	value_type kth_element (const size_type &k) {
		assert(not empty());
		return kth_element(root, B - 1, k);
	}

	void xor_all (const value_type &x) noexcept {
		(root->lazy) ^= x;
	}

};