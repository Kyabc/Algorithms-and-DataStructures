#include <queue>
#include <limits>
#include <utility>
#include <cassert>

template<class Tp>
class slope_trick {
public :
	using value_type = Tp;
	using const_reference = const Tp&;
private :
	using max_heap = std::priority_queue<Tp, std::vector<Tp>, std::less<Tp>>;
	using min_heap = std::priority_queue<Tp, std::vector<Tp>, std::greater<Tp>>;

	const value_type INF = std::numeric_limits<Tp>::max() / 2;
	value_type f_min, add_l, add_r;
	max_heap left;
	min_heap right;

	value_type left_top () const noexcept {
		if (left.empty()) return -INF;
		return left.top() + add_l;
	}

	value_type right_top () const noexcept {
		if (right.empty()) return INF;
		return right.top() + add_r;
	}

	void left_push (const_reference a) noexcept {
		left.push(a - add_l);
	}

	void right_push (const_reference a) noexcept {
		right.push(a - add_r);
	}

	value_type left_pop () noexcept {
		const auto value = left_top();
		if (not left.empty()) left.pop();
		return value;
	}

	value_type right_pop () noexcept {
		const auto value = right_top();
		if (not right.empty()) right.pop();
		return value;
	}

public :
	slope_trick () : f_min(0), add_l(0), add_r(0) { }

	// return {min f(x), {l_0, r_0}}
	std::pair<value_type, std::pair<value_type, value_type>> query () const noexcept {
		return std::make_pair(f_min, std::make_pair(left_top(), right_top()));
	}

	// f(x) = f(x) + a
	void add (const_reference a) noexcept {
		f_min += a;
	}

	// f(x) = f(x) + max(0, x - a)
	void add_x_minus_a (const_reference a) noexcept {
		f_min += std::max<Tp>(0, left_top() - a);
		left_push(a);
		right_push(left_pop());
	}

	// f(x) = f(x) + max(0, a - x)
	void add_a_minus_x (const_reference a) noexcept {
		f_min += std::max<Tp>(0, a - right_top());
		right_push(a);
		left_push(right_pop());
	}

	// f(x) = f(x) + |x - a|
	void add_abs_x_minus_a (const_reference a) noexcept {
		add_x_minus_a(a);
		add_a_minus_x(a);
	}

	// left cumulatice minimum
	void left_min () noexcept {
		min_heap buffer;
		right.swap(buffer);
	}

	// right cumulatice minimum
	void right_min () noexcept {
		max_heap buffer;
		left.swap(buffer);
	}

	// f_{new} (x) = min f(y) (x - b <= y <= x - a)
	void shift (const_reference a, const_reference b) noexcept {
		assert(a <= b);
		add_l += a;
		add_r += b;
	}

	// f(x) = f(x - a)
	void shift (const_reference a) noexcept {
		shift(a, a);
	}

	// f(x)
	// Warning : Destructive Operation
	value_type operator() (const_reference x) noexcept {
		value_type value = f_min;
		while (not left.empty()) value += std::max<Tp>(0, left_pop() - x);
		while (not right.empty()) value += std::max<Tp>(0, x - right_pop());
		return value;
	}

};