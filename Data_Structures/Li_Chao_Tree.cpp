#include <vector>
#include <limits>
#include <algorithm>
#include <functional>

template<class T, class Compare = std::less<T>>
struct li_chao_tree {
	using value_type = T;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = std::size_t;

	static constexpr value_type infty = std::numeric_limits<value_type>::max();

private :
	struct line {
	private :
		value_type a, b;
	public :
		line (const_reference a, const_reference b) noexcept : a(a), b(b) { }
		value_type operator() (const_reference x) const noexcept {
			return (a * x + b);
		}
	};

	size_type n;
	const Compare comp;
	std::vector<line> node;
	std::vector<value_type> points;

	size_type index (const_reference x) const noexcept {
		return std::distance(points.begin(), std::lower_bound(points.begin(), points.end(), x));
	}

	void line_update (size_type i, line f) {
		size_type l = i, r = i + 1;
		while (l < n) { l <<= 1; r <<= 1; }
		while (l < r) {
			line &h = node[i];
			const size_type mid = ((l + r) >> 1);
			const_reference xl = points[l - n];
			const_reference xc = points[mid - n];
			const_reference xr = points[r - 1 - n];
			bool left = comp(f(xl), h(xl)), right = comp(f(xr), h(xr));
			if (left == right) { if (left) h = f; return; }
			if (comp(f(xc), h(xc))) { std::swap(f, h); std::swap(left, right); }
			if (left) { i = (i << 1 | 0); r = mid; }
			else { i = (i << 1 | 1); l = mid; }
		}
	}

	void update (size_type l, size_type r, const line &f) {
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) line_update(l++, f);
			if (r & 1) line_update(--r, f);
		}
	}

	void chmin (reference x, const_reference y) noexcept {
		if (x > y) x = y;
	}

public :
	constexpr li_chao_tree () noexcept : comp() { }

	void add_point (const_reference x) noexcept {
		points.push_back(x);
	}

	void build () noexcept {
		std::sort(points.begin(), points.end());
		points.erase(std::unique(points.begin(), points.end()), points.end());
		node.assign((n = points.size()) << 1, line(0, infty));
	}

	template<class Iterator>
	void build (Iterator first, Iterator last) noexcept {
		points.assign(first, last); build();
	}

	void add_line (const_reference a, const_reference b) {
		update(0, n, line(a, b));
	}

	void add_segment (const_reference a, const_reference b, size_type l, size_type r) {
		update(index(l), index(r), line(a, b));
	}

	value_type get (const_reference x) noexcept {
		value_type minimum = infty;
		for (size_type i = index(x) + n; i > 0; i >>= 1) {
			chmin(minimum, node[i](x));
		}
		return minimum;
	}

};