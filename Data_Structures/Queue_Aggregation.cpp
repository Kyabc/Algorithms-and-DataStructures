#include <stack>
#include <functional>
#include <cassert>

template<class T>
struct queue_aggregation {
	using F = std::function<T(T, T)>;
	using value_type = T;
private : 
	struct node {
		value_type val, sum;
		node (const value_type &val, const value_type &sum) : val(val), sum(sum) { }
	};
	const F f;
	std::stack<node> front_stack, back_stack;

public : 
	queue_aggregation (const F &f) : f(f), front_stack(), back_stack() { }

	constexpr bool empty () const noexcept {
		return (front_stack.empty() and back_stack.empty());
	}

	constexpr size_t size () const noexcept {
		return (front_stack.size() + back_stack.size());
	}

	void push (const value_type &x) {
		if (back_stack.empty()) {
			back_stack.emplace(x, x);
		} else {
			value_type s = f(back_stack.top().sum, x);
			back_stack.emplace(x, s);
		}
	}

	void pop () {
		assert(not empty());
		if (front_stack.empty()) {
			front_stack.emplace(back_stack.top().val, back_stack.top().val);
			back_stack.pop();
			while (not back_stack.empty()) {
				value_type s = f(back_stack.top().val, front_stack.top().sum);
				front_stack.emplace(back_stack.top().val, s);
				back_stack.pop();
			}
		}
		front_stack.pop();
	}

	value_type fold () {
		assert(not empty());
		if (front_stack.empty()) return (back_stack.top().sum);
		else if (back_stack.empty()) return (front_stack.top().sum);
		else return (f(front_stack.top().sum, back_stack.top().sum));
	}

};