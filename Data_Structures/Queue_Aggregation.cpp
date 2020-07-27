#include <stack>
#include <functional>
#include <cassert>

template<class T>
struct queue_aggregation {
	using value_type = T;
	using F = std::function<T(T, T)>;
private :
	const F f;
	const value_type identity;
	value_type back_sum;
	std::stack<value_type> front_stack, back_stack;

public :
	queue_aggregation (const F &f, const value_type &ie) : f(f), identity(ie), back_sum(ie) { }

	constexpr bool empty () const noexcept {
		return (front_stack.empty() and back_stack.empty());
	}

	constexpr std::size_t size () const noexcept {
		return (front_stack.size() + back_stack.size());
	}

	void push (const value_type &x) {
		back_sum = f(back_sum, x);
		back_stack.push(x);
	}

	template<class... Args>
	void emplace(Args &&... args) {
		push(value_type(args...));
	}

	void pop () {
		assert(not empty());
		if (front_stack.empty()) {
			front_stack.push(back_stack.top());
			back_stack.pop();
			while (not back_stack.empty()) {
				value_type sum = f(back_stack.top(), front_stack.top());
				front_stack.push(sum);
				back_stack.pop();
			}
			back_sum = identity;
		}
		front_stack.pop();
	}

	value_type fold () {
		if (front_stack.empty()) return back_sum;
		return f(front_stack.top(), back_sum);
	}

};