#include <memory>
#include <cassert>

template<class T = unsigned int, int B = 32>
struct binary_trie {
	using value_type = T;
	using size_type = std::size_t;
private :
	struct node_type {
		size_type count;
		value_type lazy;
		std::unique_ptr<node_type> next[2];
		node_type () : count(0), lazy(0), next{nullptr, nullptr} { }
	};

	using node_ptr = std::unique_ptr<node_type>;

	node_ptr root;

	size_type size (const node_ptr &node) const noexcept {
		return (node ? (node->count) : 0);
	}

	void propagate (node_ptr &node, int i) noexcept {
        if (node->lazy == 0) return;
		if ((node->lazy) >> i & 1) (node->next[0]).swap(node->next[1]);
		if (node->next[0]) (node->next[0]->lazy) ^= (node->lazy);
		if (node->next[1]) (node->next[1]->lazy) ^= (node->lazy);
		(node->lazy) = 0;
	}

	size_type count (node_ptr &cur, const value_type &x, int i) {
		if (not cur) cur = std::make_unique<node_type>();
		if (i < 0) return (size(cur));
		if (cur->lazy) propagate(cur, i);
		return (count(cur->next[x >> i & 1], x, i - 1));
	}

	void insert (node_ptr &cur, const value_type &x, int i) {
		if (not cur) cur = std::make_unique<node_type>();
		(cur->count)++;
		if (i < 0) return;
		if (cur->lazy) propagate(cur, i);
		insert(cur->next[x >> i & 1], x, i - 1);
	}

	void erase (node_ptr &cur, const value_type &x, int i, const size_type &_size) {
		(cur->count) -= _size;
		if (i >= 0) erase(cur->next[x >> i & 1], x, i - 1, _size);
	}

	value_type min_element (node_ptr &cur, int i, const value_type &x) {
		if (cur->lazy) propagate(cur, i);
        const int b = (x >> i & 1);
		if (size(cur->next[b])) {
			if (i == 0) return value_type(0);
			return (min_element(cur->next[b], i - 1, x));
		} else {
			if (i == 0) return value_type(1);
			value_type min = (value_type(1) << i);
			return (min + min_element(cur->next[b ^ 1], i - 1, x));
		}
	}

	value_type max_element (node_ptr &cur, int i, const value_type &x) {
		if (cur->lazy) propagate(cur, i);
        const int b = (x >> i & 1);
		if (size(cur->next[b])) {
			if (i == 0) return value_type(0);
			return max_element(cur->next[b], i - 1, x);
		} else {
            if (i == 0) return value_type(1);
			value_type max = (value_type(1) << i);
			return (max + max_element(cur->next[b ^ 1], i - 1, x));
        }
	}

	value_type kth_element (node_ptr &cur, int i, size_type k, const value_type &x) {
		if (cur->lazy) propagate(cur, i);
        const int b = (x >> i & 1);
		const size_type _size = size(cur->next[b]);
		if (_size >= k) {
			if (i == 0) return value_type(0);
			return kth_element(cur->next[b], i - 1, k, x);
		} else {
			if (i == 0) return value_type(1);
			value_type kth = (value_type(1) << i);
			return (kth + kth_element(cur->next[b ^ 1], i - 1, k - _size, x));
		}
	}

public : 
	explicit binary_trie () : root (std::make_unique<node_type>()) { }

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

	value_type min_element (const value_type &x = 0) {
		assert(not empty());
		return min_element(root, B - 1, x);
	}

	value_type max_element (const value_type &x = 0) {
		assert(not empty());
		return max_element(root, B - 1, x);
	}

	value_type kth_element (const size_type &k, const value_type &x = 0) {
		assert(not empty());
		return kth_element(root, B - 1, k, x);
	}

	void xor_all (const value_type &x) noexcept {
		(root->lazy) ^= x;
	}

};