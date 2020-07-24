#include <functional>
#include <memory>
#include <cassert>

template<class T, class E, class Compare = std::less<T>>
struct skew_heap {
	using F = std::function<T(T, E)>;
	using G = std::function<E(E, E)>;
	using value_type = T;
	using operator_type = E;
	using size_type = std::size_t;
	using const_reference = const value_type &;
private : 
	Compare comp;
	F f;
	G g;
	operator_type ie;
	size_type _size;

	struct node {
	public : 
		value_type value;
		operator_type lazy;
		std::unique_ptr<node> left, right;
		node (const value_type &x, const operator_type &e) :
			value(x), lazy(e), left(nullptr), right(nullptr) { }
	};

	using node_ptr = std::unique_ptr<node>;

	node_ptr root;

	node_ptr propagate (node_ptr x) {
		if (not x) return (std::move(x));
		if ((x->lazy) == ie) return (std::move(x));
		if (x->left) {
			(x->left->lazy) = g(x->left->lazy, x->lazy);
		}
		if (x->right) {
			(x->right->lazy) = g(x->right->lazy, x->lazy);
		}
		(x->value) = f(x->value, x->lazy);
		(x->lazy) = ie;
		return (std::move(x));
	}

	node_ptr meld (node_ptr a, node_ptr b) {
		a = propagate(std::move(a));
		b = propagate(std::move(b));
		if (not a) return (std::move(b));
		if (not b) return (std::move(a));
		if (comp((a->value), (b->value))) a.swap(b);
		(a->right) = meld(std::move(a->right), std::move(b));
		(a->right).swap(a->left);
		return (std::move(a));
	}

	void heap_copy (node_ptr &cur, const node_ptr &_copy) {
		if (not _copy) return;
		if (cur) cur.reset();
		cur = std::make_unique<node>(_copy->value, _copy->lazy);
		if (_copy->left) heap_copy(cur->left, _copy->left);
		if (_copy->right) heap_copy(cur->right, _copy->right);
	}

public : 
	explicit skew_heap () : f([](const value_type &a, const operator_type &b) { return a; }),
							g([](const operator_type &a, const operator_type &b) { return a; }),
							comp(), ie(operator_type()), _size(0), root(nullptr) { }

	explicit skew_heap (const F &f, const G &g, const operator_type &ie) : 
		f(f), g(g), comp(), ie(ie), _size(0), root(nullptr) { }

	explicit skew_heap (skew_heap&& rhs) noexcept :
		f(rhs.f), g(rhs.g), comp(rhs.comp), ie(rhs.ie), _size(rhs._size), root(std::move(rhs.root)) { }

	skew_heap (const skew_heap &rhs) : f(rhs.f), g(rhs.g), comp(rhs.comp), ie(rhs.ie), _size(rhs._size) {
		clear();
		heap_copy(root, rhs.root);
	}

	~skew_heap () { }

	constexpr size_type size () const noexcept {
		return (_size);
	}

	constexpr bool empty () const noexcept {
		return (not _size);
	}

	const_reference top () {
		assert (not empty());
		root = propagate(std::move(root));
		return (root->value);
	}

	void pop () {
		assert(not empty());
		root = propagate(std::move(root)); _size--;
		root = meld(std::move(root->left), std::move(root->right));
	}

	void push (const value_type &x) {
		root = meld(std::move(root), std::make_unique<node>(node(x, ie)));
		_size++;
	}

	template<class... Args>
	void emplace (Args &&... args) {
		push(value_type(args...));
	}

	void clear () noexcept {
		while (not empty()) pop();
	}

	void add (const operator_type &x) {
		if (root) (root->lazy) = g(root->lazy, x);
	}

	void meld (skew_heap &rhs) {
		(this->_size) += (rhs._size); rhs._size = 0;
		(this->root) = meld(std::move(this->root), std::move(rhs.root));
	}

	void swap (skew_heap &rhs) {
		(this->root).swap(rhs.root);
		swap((this->_size), rhs._size);
	}

};