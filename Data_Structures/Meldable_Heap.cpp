#include <memory>
#include <cassert>

template<class T, class Compare = std::less<T>>
struct meldable_heap {
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
protected :
	Compare comp;
private :
	struct node_type;
	using node_ptr = std::unique_ptr<node_type>;

	node_ptr root;
	size_type _size;

	struct node_type {
		value_type key;
		node_ptr left, right;
		node_type (const_reference key)
		noexcept : key(key), left(nullptr), right(nullptr) { }
	};

	node_ptr meld (node_ptr lhs, node_ptr rhs) noexcept {
		if (not lhs) return std::move(rhs);
		if (not rhs) return std::move(lhs);
		if (comp(lhs->key, rhs->key)) lhs.swap(rhs);
		lhs->right = meld(std::move(lhs->right), std::move(rhs));
		(lhs->right).swap(lhs->left);
		return std::move(lhs);
	}

public :
<<<<<<< Updated upstream
	meldable_heap () noexcept : root(nullptr), _size(0) { }
=======
	explicit meldable_heap (const Compare &x = Compare())
	noexcept : comp(x), root(nullptr), _size(0) { }
>>>>>>> Stashed changes

	bool empty () const noexcept {
		return (_size == 0);
	}

<<<<<<< Updated upstream
	const size_type &size () const noexcept {
=======
	const size_type& size () const noexcept {
>>>>>>> Stashed changes
		return _size;
	}

	const_reference top () const noexcept {
		assert(not empty());
		return root->key;
	}

	void push (const_reference x) noexcept {
		++_size;
		root = meld(std::move(root), std::make_unique<node_type>(x));
	}

<<<<<<< Updated upstream
	void push (reference x) noexcept {
=======
	void push (value_type&& x) noexcept {
>>>>>>> Stashed changes
		++_size;
		root = meld(std::move(root), std::make_unique<node_type>(x));
	}

	template<class... Args>
<<<<<<< Updated upstream
	void emplace(Args... args) noexcept {
=======
	void emplace(Args&&... args) noexcept {
>>>>>>> Stashed changes
		push(value_type(args...));
	}

	void pop () noexcept {
		assert(not empty());
		--_size;
		root = meld(std::move(root->left), std::move(root->right));
	}

<<<<<<< Updated upstream
	void swap (meldable_heap &rhs) noexcept {
		root.swap(rhs.root);
		std::swap(_size, rhs._size);
	}

	void meld (meldable_heap &rhs) noexcept {
=======
	void swap (meldable_heap& rhs) noexcept {
		root.swap(rhs.root);
		std::swap(_size, rhs._size);
		std::swap(comp, rhs.comp);
	}

	void meld (meldable_heap& rhs) noexcept {
>>>>>>> Stashed changes
		_size += rhs.size();
		rhs._size = 0;
		root = meld(std::move(root), std::move(rhs.root));
	}

};