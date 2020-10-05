#include <memory>
#include <cstdint>

template<class Tp>
struct treap {
	using const_reference = const Tp&;
	using value_type = Tp;
	using size_type = size_t;
	using priority_type = std::uint32_t;
private :
	struct node_type;
	using node_ptr = std::unique_ptr<node_type>;

	size_type _size;
	node_ptr root;

	struct node_type {
		value_type key;
		priority_type priority;
		node_ptr left, right;
		node_type (const_reference key, priority_type priority)
		noexcept : key(key), priority(priority), left(nullptr), right(nullptr) { }
	};

	priority_type xor_shift () noexcept {
		static priority_type x = 123456789, y = 362436069, z = 521288629, w = 88675123;
		priority_type t = (x ^ (x << 11));
		x = y; y = z; z = w;
		return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
	}

	node_ptr right_rotate (node_ptr node) noexcept {
		node_ptr buff = std::move(node->left);
		node->left = std::move(buff->right);
		buff->right = std::move(node);
		return std::move(buff);
	}

	node_ptr left_rotate (node_ptr node) noexcept {
		node_ptr buff = std::move(node->right);
		node->right = std::move(buff->left);
		buff->left = std::move(node);
		return std::move(buff);
	}

	node_ptr insert (node_ptr node, const_reference key, priority_type priority) noexcept {
		if (node == nullptr) {
			return std::move(std::make_unique<node_type>(key, priority));
		}
		if (key == node->key) {
			return std::move(node);
		}
		if (key < node->key) {
			node->left = insert(std::move(node->left), key, priority);
			if (node->priority < node->left->priority) {
				node = right_rotate(std::move(node));
			}
		} else {
			node->right = insert(std::move(node->right), key, priority);
			if (node->priority < node->right->priority) {
				node = left_rotate(std::move(node));
			}
		}
		return std::move(node);
	}

	node_ptr erase (node_ptr node, const_reference key) noexcept {
		if (node == nullptr) return std::move(node);
		if (key < node->key) node->left = erase(std::move(node->left), key);
		else if (key > node->key) node->right = erase(std::move(node->right), key);
		else {
			if (node->left == nullptr and node->right == nullptr) {
				node.release();
				return std::move(node);
			} else if (node->left == nullptr) {
				node = left_rotate(std::move(node));
			} else if (node->right == nullptr) {
				node = right_rotate(std::move(node));
			} else {
				if (node->left->priority > node->right->priority) {
					node = right_rotate(std::move(node));
				} else {
					node = left_rotate(std::move(node));
				}
			}
			return erase(std::move(node), key);
		}
		return std::move(node);
	}

	bool find (const node_ptr &node, const_reference key) noexcept {
		if (node == nullptr) return false;
		if (key == node->key) return true;
		if (key < node->key) return find(node->left, key);
		else return find(node->right, key);
	}

public :
	constexpr treap () noexcept : root(nullptr), _size(0) { }

	bool empty () const noexcept {
		return (_size == 0);
	}

	const size_type& size () const noexcept {
		return _size;
	}

	bool find (const_reference key) noexcept {
		return find(root, key);
	}

	bool insert (const_reference key, priority_type priority) noexcept {
		if (find(key)) return false;
		root = insert(std::move(root), key, priority);
		++_size;
		return true;
	}

	bool insert (const_reference key) noexcept {
		return insert(key, xor_shift());
	}

	bool erase (const_reference key) noexcept {
		if (not find(key)) return false;
		root = erase(std::move(root), key);
		--_size;
		return true;
	}

};