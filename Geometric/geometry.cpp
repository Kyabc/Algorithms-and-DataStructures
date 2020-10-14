#include <iostream>
#include <cmath>
#include <cassert>
#include <ios>
#include <iomanip>
#include <vector>


namespace geo {


// change as appropriate
using default_type = double;
static constexpr default_type EPS = 1e-10;
static constexpr default_type pi = std::acos(-1);

template<class Tp> struct point;
template<class Tp> struct line;
template<class Tp> struct segment;
template<class Tp> struct circle;

template<class Tp> using points = std::vector<point<Tp>>;
template<class Tp> using polygon = std::vector<point<Tp>>;
template<class Tp> using lines = std::vector<line<Tp>>;
template<class Tp> using segments = std::vector<segment<Tp>>;
template<class Tp> using circles = std::vector<circle<Tp>>;

void set_format (int n = 10) { std::cout << std::fixed << std::setprecision(n); }
template<class Tp> inline bool eq (const Tp &a, const Tp &b) { return (std::fabs(a - b) <= EPS); }

template<class Tp = default_type>
struct point {
	using reference = Tp&;
	using const_reference = const Tp&;
	using value_type = Tp;
	value_type x, y;
	point () = default;
	point (reference x, reference y) noexcept : x(x), y(y) { }
	point (const_reference x, const_reference y) noexcept : x(x), y(y) { }
	constexpr point operator+ () const noexcept { return point(*this); }
	constexpr point operator- () noexcept { x = -x; y = -y; return point(*this); }
	constexpr point &operator+= (const point &p) noexcept { x += p.x; y += p.y; return (*this); }
	constexpr point &operator-= (const point &p) noexcept { x -= p.x; y -= p.y; return (*this); }
	constexpr point &operator*= (const point &p) noexcept {
		const value_type real = x * p.x - y * p.y;
		const value_type imag = x * p.y + p.x * y;
		x = real; y = imag; return (*this);
	}
	constexpr point &operator/= (const point &p) noexcept {
		const value_type d = p.x * p.x + p.y * p.y;
		const value_type real = x * p.x + y * p.y;
		const value_type imag = y * p.x - x * p.y;
		x = real / d; y = imag / d; return (*this);
	}
	constexpr point operator+ (const point &p) const noexcept { return (point(*this) += p); }
	constexpr point operator- (const point &p) const noexcept { return (point(*this) -= p); }
	constexpr point operator* (const point &p) const noexcept { return (point(*this) *= p); }
	constexpr point operator/ (const point &p) const noexcept { return (point(*this) /= p); }
	template<class Scalar> constexpr point &operator*= (const Scalar &k) noexcept { x *= k; y *= k; return (*this); }
	template<class Scalar> constexpr point &operator/= (const Scalar &k) noexcept { x /= k; y /= k; return (*this); }
	template<class Scalar> constexpr point operator* (const Scalar &k) const noexcept { return (point(*this) *= k); }
	template<class Scalar> constexpr point operator/ (const Scalar &k) const noexcept { return (point(*this) /= k); }
	bool operator== (const point &p) const noexcept { return (eq(x, p.x) and eq(y, p.y)); }
	bool operator!= (const point &p) const noexcept { return (eq(x, p.x) or eq(y, p.y)); }
	friend std::istream &operator>> (std::istream &is, point &p) { value_type a, b; is >> a >> b; p = point(a, b); return (is); }
	friend std::ostream &operator<< (std::ostream &os, const point &p) { return (os << p.x << ' ' << p.y); }
};

template<class Tp = default_type>
struct line {
	using const_reference = const Tp&;
	using value_type = Tp;
	point<Tp> p1, p2;
	line () = default;
	line (const point<Tp> &p1, const point<Tp> &p2) noexcept : p1(p1), p2(p2) { }
	line (const_reference a, const_reference b, const_reference c, const_reference d) noexcept : p1(a, b), p2(c, d) { }
	line (const_reference a, const_reference b, const_reference c) noexcept {
		if (eq(a, 0)) { p1 = point<Tp>(0, c / b); p2 = point<Tp>(1, c / b); }
		else if (eq(b, 0)) { p1 = point<Tp>(c / a, 0); p2 = point<Tp>(c / a, 1); }
		else { p1 = point<Tp>(0, c / b); p2 = point<Tp>(c / a, 0); }
	}
	bool operator== (const line &l) const noexcept { return ((p1 == l.p1 and p2 == l.p2) or (p1 == l.p2 and p2 == l.p1)); }
	bool operator!= (const line &l) const noexcept { return ((p1 != l.p1 or p2 != l.p2) and (p1 != l.p2 or p2 != l.p1)); }
};

template<class Tp = default_type>
struct segment : public line<Tp> {
	segment () = default;
	segment (const point<Tp> &p1, const point<Tp> &p2) noexcept : line<Tp>(p1, p2) { }
};

template<class Tp> inline point<Tp> conj (const point<Tp> &p) { return point<Tp>(p.x, -p.y); }
template<class Tp> inline Tp norm (const point<Tp> &p) { return p.x * p.x + p.y * p.y; }
template<class Tp> inline Tp abs (const point<Tp> &p) { return std::sqrt(norm(p)); }
template<class Tp> inline Tp arg (const point<Tp> &p) { return std::atan2(p.y, p.x); }
template<class Tp> inline Tp dot (const point<Tp> &p1, const point<Tp> &p2) { return p1.x * p2.x + p1.y * p2.y; }
template<class Tp> inline Tp cross (const point<Tp> &p1, const point<Tp> &p2) { return p1.x * p2.y - p2.x * p1.y; }

template<class Tp>
point<Tp> projection (const line<Tp> &l, const point<Tp> &p) {
	point<Tp> base = l.p2 - l.p1;
	Tp r = dot(p - l.p1, base) / norm(base);
	return l.p1 + base * r;
}

template<class Tp>
point<Tp> reflection (const line<Tp> &l, const point<Tp> &p) {
	return (p + (projection(l, p) - p) * 2.0);
}

template<class Tp>
int counter_clockwise (const point<Tp> &p0, const point<Tp> &p1, const point<Tp> &p2) {
	const point<Tp> a = p1 - p0, b = p2 - p0;
	if (cross(a, b) > EPS) return 1;     // COUNTER_CLOCKWISE
	if (cross(a, b) < -EPS) return -1;   // CLOCKWISE
	if (dot(a, b) < -EPS) return 2;      // p2 -> p0 -> p1 (ONLINE_BACK)
	if (norm(a) < norm(b)) return -2;    // p0 -> p1 -> p2 (ONLINE_FRONT)
	return 0;                            // p0 -> p2 -> p1 (ON_SEGMENT)
}

template<class Tp>
bool parallel (const point<Tp> &lhs, const point<Tp> &rhs) {
	return eq(cross(lhs, rhs), 0.0);
}

template<class Tp>
bool parallel (const line<Tp> &lhs, const line<Tp> &rhs) {
	return parallel(lhs.p1 - lhs.p2, rhs.p1 - rhs.p2);
}

template<class Tp>
bool orthogonal (const point<Tp> &lhs, const point<Tp> &rhs) {
	return eq(dot(lhs, rhs), 0.0);
}

template<class Tp>
bool orthogonal (const line<Tp> &lhs, const line<Tp> &rhs) {
	return orthogonal(lhs.p1 - lhs.p2, rhs.p1 - rhs.p2);
}

template<class Tp>
bool intersect (const point<Tp> &a, const point<Tp> &b, const point<Tp> &c, const point<Tp> &d) {
	const int s = counter_clockwise(a, b, c) * counter_clockwise(a, b, d);
	const int t = counter_clockwise(c, d, a) * counter_clockwise(c, d, b);
	return (s <= 0 and t <= 0);
}

template<class Tp>
bool intersect (const segment<Tp> &lhs, const segment<Tp> &rhs) {
	return intersect(lhs.p1, lhs.p2, rhs.p1, rhs.p2);
}

template<class Tp>
bool intersect (const line<Tp> &lhs, const line<Tp> &rhs) {
	return (not parallel(lhs, rhs));
}

template<class Tp>
point<Tp> cross_point (const line<Tp> &lhs, const line<Tp> &rhs) {
	assert(intersect(lhs, rhs));
	const Tp a = cross(lhs.p2 - lhs.p1, rhs.p2 - rhs.p1);
	const Tp b = cross(lhs.p2 - lhs.p1, lhs.p2 - rhs.p1);
	if (eq(a, 0.0) and eq(b, 0.0)) return rhs.p1;
	return rhs.p1 + (rhs.p2 - rhs.p1) * b / a;
}

template<class Tp>
point<Tp> cross_point (const segment<Tp> &lhs, const segment<Tp> &rhs) {
	assert(intersect(lhs, rhs));
	return cross_point(line<Tp>(lhs), line<Tp>(rhs));
}

template<class Tp>
Tp distance (const point<Tp> &lhs, const point<Tp> &rhs) {
	return abs(lhs - rhs);
}

template<class Tp>
Tp distance (const line<Tp> &l, const point<Tp> &p) {
	return std::fabs(cross(l.p2 - l.p1, p - l.p1) / abs(l.p2 - l.p1));
}

template<class Tp>
Tp distance (const segment<Tp> &s, const point<Tp> &p) {
	if (dot(s.p2 - s.p1, p - s.p1) < -EPS) return abs(p - s.p1);
	if (dot(s.p1 - s.p2, p - s.p2) < -EPS) return abs(p - s.p2);
	return distance(line<Tp>(s), p);
}

template<class Tp>
Tp distance (const segment<Tp> &lhs, const segment<Tp> &rhs) {
	if (intersect(lhs, rhs)) return 0.0;
	const Tp a = std::min(distance(lhs, rhs.p1), distance(lhs, rhs.p2));
	const Tp b = std::min(distance(rhs, lhs.p1), distance(rhs, lhs.p2));
	return std::min(a, b);
}


}