#include <iostream>
#include <cmath>
#include <cassert>
#include <ios>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <limits>
#include <functional>
#include <utility>


namespace geo {


// change as appropriate
using default_type = double;
static constexpr default_type EPS = 1e-12;
static constexpr default_type pi = std::acos(-1);
static constexpr default_type infty = std::numeric_limits<default_type>::max() / 2.0;
// class
template<class Tp> struct point;
template<class Tp> struct line;
template<class Tp> struct segment;
template<class Tp> struct circle;

//alias
template<class Tp = default_type> using points = std::vector<point<Tp>>;
template<class Tp = default_type> using polygon = std::vector<point<Tp>>;
template<class Tp = default_type> using lines = std::vector<line<Tp>>;
template<class Tp = default_type> using segments = std::vector<segment<Tp>>;
template<class Tp = default_type> using circles = std::vector<circle<Tp>>;

// basic function
void set_format (int n = 10) { std::cout << std::fixed << std::setprecision(n); }
template<class Tp> inline bool eq (const Tp &a, const Tp &b) { return (std::fabs(a - b) <= EPS); }
template<class Tp> inline bool chmax (Tp &a, const Tp &b) { if (a < b) { a = b; return 1; } return 0; }
template<class Tp> inline bool chmin (Tp &a, const Tp &b) { if (a > b) { a = b; return 1; } return 0; }
inline size_t next (const size_t &index, const size_t &n) { return (index + 1 == n ? 0 : index + 1); }
inline size_t prev (const size_t &index, const size_t &n) { return (index == 0 ? n - 1 : index - 1); }

// point class
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
	bool operator!= (const point &p) const noexcept { return (not (point(*this) == p)); }
	friend std::istream &operator>> (std::istream &is, point &p) { value_type a, b; is >> a >> b; p = point(a, b); return (is); }
	friend std::ostream &operator<< (std::ostream &os, const point &p) { return (os << p.x << ' ' << p.y); }
};

// line class
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
	bool operator!= (const line &l) const noexcept { return (not (line(*this) == l)); }
};

//segment class
template<class Tp = default_type>
struct segment : public line<Tp> {
	using const_reference = const Tp&;
	segment () = default;
	segment (const_reference a, const_reference b, const_reference c, const_reference d) noexcept : line<Tp>(a, b, c, d) { }
	segment (const point<Tp> &p1, const point<Tp> &p2) noexcept : line<Tp>(p1, p2) { }
};

// circle class
template<class Tp = default_type>
struct circle {
	using reference = Tp&;
	using const_reference = const Tp&;
	using value_type = Tp;
	point<Tp> center;
	value_type radius;
	circle () = default;
	circle (const point<Tp> &p, const_reference r) noexcept : center(p), radius(r) { }
	circle (reference x, reference y, reference r) noexcept : center(x, y), radius(r) { }
	circle (const_reference x, const_reference y, const_reference r) noexcept : center(x, y), radius(r) { }
	bool operator== (const circle &rhs) const noexcept { return (center == rhs.center and eq(radius, rhs.radius)); }
	bool operator!= (const circle &rhs) const noexcept { return (not (circle(*this) == rhs)); }
};

// functions
template<class Tp> inline point<Tp> conj (const point<Tp> &p) { return point<Tp>(p.x, -p.y); }                             // 共役な複素数
template<class Tp> inline point<Tp> normalize (const point<Tp> &p) { return (p / abs(p)); }                                // 単位ベクトル
template<class Tp> inline Tp norm (const point<Tp> &p) { return p.x * p.x + p.y * p.y; }                                   // 複素数のノルム(原点からの距離の二乗)
template<class Tp> inline Tp abs (const point<Tp> &p) { return std::sqrt(norm(p)); }                                       // 複素数の絶対値(原点からの距離)
template<class Tp> inline Tp arg (const point<Tp> &p) { Tp d = std::atan2(p.y, p.x); return (d < -EPS ? d + pi * 2 : d); } // 偏角 (0 ≤ Θ ≤ 2π)
template<class Tp> inline Tp dot (const point<Tp> &p1, const point<Tp> &p2) { return p1.x * p2.x + p1.y * p2.y; }          // 内積
template<class Tp> inline Tp cross (const point<Tp> &p1, const point<Tp> &p2) { return p1.x * p2.y - p2.x * p1.y; }        // 外積
template<class Tp> inline bool compare_x (const point<Tp> &p1, const point<Tp> &p2) { return (eq(p1.x, p2.x) ? p1.y < p2.y : p1.x < p2.x); }
template<class Tp> inline bool compare_y (const point<Tp> &p1, const point<Tp> &p2) { return (eq(p1.y, p2.y) ? p1.x < p2.x : p1.y < p2.y); }
template<class Tp> inline bool compare (const point<Tp> &p1, const point<Tp> &p2) { return (eq(p1.x, p2.x) ? compare_y(p1, p2) : compare_x(p1, p2)); }

// 点の回転
template<class Tp>
point<Tp> rotate (const point<Tp> &p, const Tp &theta) {
	const Tp a = p.x * std::cos(theta) - p.y * std::sin(theta);
	const Tp b = p.x * std::sin(theta) + p.x * std::cos(theta);
	return point<Tp>(a, b);
}

template<class Tp>
point<Tp> rotate90 (const point<Tp> &p) {
	return point<Tp>(-p.y, p.x);
}

template<class Tp>
point<Tp> rotate180 (const point<Tp> &p) {
	return -p;
}

template<class Tp>
point<Tp> rotate270 (const point<Tp> &p) {
	return point<Tp>(p.y, -p.x);
}

// 円の回転
template<class Tp>
circle<Tp> rotate (const circle<Tp> &p, const Tp &theta) {
	return circle<Tp>(rotate(p.center, theta), p.radius);
}

// 点から直線への射影(点からの垂線と直線の交点)
template<class Tp>
point<Tp> projection (const line<Tp> &l, const point<Tp> &p) {
	point<Tp> base = l.p2 - l.p1;
	Tp r = dot(p - l.p1, base) / norm(base);
	return l.p1 + base * r;
}

// 点から直線への反射(直線に対して対称な点)
template<class Tp>
point<Tp> reflection (const line<Tp> &l, const point<Tp> &p) {
	return (p + (projection(l, p) - p) * 2.0);
}

// 3 点の位置関係
template<class Tp>
int counter_clockwise (const point<Tp> &p0, const point<Tp> &p1, const point<Tp> &p2) {
	const point<Tp> a = p1 - p0, b = p2 - p0;
	if (cross(a, b) > EPS) return 1;     // COUNTER_CLOCKWISE
	if (cross(a, b) < -EPS) return -1;   // CLOCKWISE
	if (dot(a, b) < -EPS) return 2;      // p2 -> p0 -> p1 (ONLINE_BACK)
	if (norm(a) < norm(b)) return -2;    // p0 -> p1 -> p2 (ONLINE_FRONT)
	return 0;                            // p0 -> p2 -> p1 (ON_SEGMENT)
}

// 平行判定
template<class Tp>
bool parallel (const point<Tp> &lhs, const point<Tp> &rhs) {
	return eq(cross(lhs, rhs), 0.0);
}

template<class Tp>
bool parallel (const line<Tp> &lhs, const line<Tp> &rhs) {
	return parallel(lhs.p1 - lhs.p2, rhs.p1 - rhs.p2);
}

// 直交判定
template<class Tp>
bool orthogonal (const point<Tp> &lhs, const point<Tp> &rhs) {
	return eq(dot(lhs, rhs), 0.0);
}

template<class Tp>
bool orthogonal (const line<Tp> &lhs, const line<Tp> &rhs) {
	return orthogonal(lhs.p1 - lhs.p2, rhs.p1 - rhs.p2);
}

// 交差判定
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
bool intersect(const line<Tp> &lhs, const segment<Tp> &rhs) {
	return intersect(lhs.p1, lhs.p2, rhs.p1, rhs.p2);
}

template<class Tp>
bool intersect(const segment<Tp> &lhs, const line<Tp> &rhs) {
	return intersect(lhs.p1, lhs.p2, rhs.p1, rhs.p2);
}

template<class Tp>
int intersect (circle<Tp> lhs, circle<Tp> rhs) {
	if (lhs.radius < rhs.radius) std::swap(lhs, rhs);
	const Tp dist = distance(lhs.center, rhs.center);
	const Tp rs = lhs.radius + rhs.radius;
	if (eq(dist, rs)) return 3;                       // 外接
	if (dist > rs) return 4;                          // 離れている
	if (eq(dist + rhs.radius, lhs.radius)) return 1;  // 内接
	if (dist + rhs.radius < lhs.radius) return 0;     // 内部
	return 2;                                         // 重なっている
}

template<class Tp>
bool intersect (const circle<Tp> &lhs, const line<Tp> &rhs) {
	return (distance(lhs.center, projection(rhs, lhs.center)) < lhs.radius + EPS);
}

template<class Tp>
bool intersect (const line<Tp> &lhs, const circle<Tp> &rhs) {
	return intersect(rhs, lhs);
}

// 交点
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
std::pair<point<Tp>, point<Tp>> cross_point (const circle<Tp> &lhs, const line<Tp> &rhs) {
	assert(intersect(lhs, rhs));
	const point<Tp> pr = projection(rhs, lhs.center);
	if (eq(distance(pr, lhs.center), lhs.radius)) return std::make_pair(pr, pr);
	const point<Tp> e = normalize(rhs.p1 - rhs.p2);
	const Tp s = std::sqrt(lhs.radius * lhs.radius - norm(pr - lhs.center));
	return std::make_pair(pr + e * s, pr - e * s);
}

template<class Tp>
std::pair<point<Tp>, point<Tp>> cross_point (const line<Tp> &lhs, const circle<Tp> &rhs) {
	return cross_point(rhs, lhs);
}

template<class Tp>
std::pair<point<Tp>, point<Tp>> cross_point (const circle<Tp> &lhs, const circle<Tp> &rhs) {
	assert(intersect(lhs, rhs) != 4 and lhs != rhs);
	const Tp dist = abs(lhs.center - rhs.center);
	const Tp rc = (dist * dist + lhs.radius * lhs.radius - rhs.radius * rhs.radius) / (dist * 2.0);
	const Tp rs = std::sqrt(lhs.radius * lhs.radius - rc * rc);
	const point<Tp> e1 = normalize(rhs.center - lhs.center);
	const point<Tp> a = lhs.center + e1 * rc + rotate90(e1) * rs;
	const point<Tp> b = lhs.center + e1 * rc + rotate270(e1) * rs;
	return std::make_pair(a, b);
}

// 距離
template<class Tp>
Tp distance (const point<Tp> &lhs, const point<Tp> &rhs) {
	return abs(lhs - rhs);
}

template<class Tp>
Tp distance (const line<Tp> &l, const point<Tp> &p) {
	return std::fabs(cross(l.p2 - l.p1, p - l.p1) / abs(l.p2 - l.p1));
}

template<class Tp>
Tp distance (const point<Tp> &p, const line<Tp> &l) {
	return distance(l, p);
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

// 多角形の面積
template<class Tp>
Tp area (const polygon<Tp> &p) {
	if (p.size() < 3) return 0.0;
	Tp res = 0;
	for (size_t i = 0; i < p.size(); i++) {
		res += cross(p[i], p[(i + 1 < p.size() ? i + 1 : 0)]);
	}
	return res / 2.0;
}

// 凸多角形判定
template<class Tp>
bool isconvex (const polygon<Tp> &p) {
	const size_t n = p.size();
	if (n < 3) return false;
	bool f = true, g = true;
	for (size_t i = 0; i < p.size() and (f or g); i++) {
		const Tp cs = cross(p[prev(i, n)] - p[i], p[next(i, n)] - p[i]);
		if (not eq(cs, 0.0)) ((cs > 0.0) ? f : g) = false;
	}
	return (f or g);
}

// 多角形と点の位置関係
template<class Tp>
int include_point (const polygon<Tp> &p, const point<Tp> &s) {
	// inside -> 2
	// on the line -> 1
	// outside  -> 0
	bool flag = false;
	enum { outside, on, inside };
	for (size_t i = 0; i < p.size(); i++) {
		point<Tp> a = p[i] - s, b = p[next(i, p.size())] - s;
		if (a.y > b.y) std::swap(a, b);
		if (a.y < EPS and b.y > EPS and cross(a, b) < -EPS) flag ^= true;
		if (eq(cross(a, b), 0.0) and dot(a, b) < EPS) return on;
	}
	return (flag ? inside : outside);
}

// 凸包
template<class Tp>
polygon<Tp> convex_hull (polygon<Tp> p) {
	std::sort(p.begin(), p.end(), compare_x<Tp>);
	if (p.size() < 3) return p;
	size_t k = 0;
	polygon<Tp> ch(p.size() + 2);
	auto crs = [&] (const size_t &i) {
		return cross(ch[k - 2] - ch[k - 1], p[i] - ch[k - 1]);
	};
	for (size_t i = 0; i < p.size(); i++) {
		while (k > 1 and crs(i) > EPS) k--;
		ch[k++] = p[i];
	}
	for (size_t i = p.size() - 1, s = k; i > 0; i--) {
		while (k > s and crs(i - 1) > EPS) k--;
		ch[k++] = p[i - 1];
	}
	ch.resize(k - 1);
	return ch;
}

// 最遠点対
template<class Tp>
Tp convex_diameter (polygon<Tp> p) {
	const auto c = convex_hull(p);
	const size_t n = c.size();
	if (n < 2) return 0.0;
	if (n == 2) return abs(c[0] - c[1]);
	Tp diameter = 0.0;
	size_t i = 0, j = 0;
	for (size_t k = 0; k < n; k++) {
		if (not compare_x(c[i], c[k])) i = k;
		if (compare_x(c[j], c[k])) j = k;
	}
	const size_t si = i, sj = j;
	while (i != sj or j != si) {
		chmax(diameter, abs(c[i] - c[j]));
		if (cross(c[next(i, n)] - c[i], c[next(j, n)] - c[j]) < -EPS) i = next(i, n);
		else j = next(j, n);
	}
	return diameter;
}

// 多角形の直線によるカット
template<class Tp>
polygon<Tp> convex_cut (const polygon<Tp> &pol, const line<Tp> &cut) {
	polygon<Tp> left;
	for (size_t i = 0; i < pol.size(); i++) {
		const point<Tp> &p = pol[i];
		const point<Tp> &q = pol[next(i, pol.size())];
		const int ccw = counter_clockwise(cut.p1, cut.p2, p);
		if (ccw != -1) left.push_back(p);
		if (ccw * counter_clockwise(cut.p1, cut.p2, q) < 0) {
			if (parallel(cut, line<Tp>(p, q))) left.push_back(p);
			else left.push_back(cross_point(cut, line<Tp>(p, q)));
		}
	}
	return left;
}

// 三角形の内接円
template<class Tp>
circle<Tp> incircle (const point<Tp> &a, const point<Tp> &b, const point<Tp> &c) {
	const Tp s = abs(c - b), t = abs(a - c), u = abs(b - a);
	point<Tp> p = (a * s + b * t + c * u) / (s + t + u);
	return circle<Tp>(p, distance(line<Tp>(a, b), p));
}

// 三角形の外接円
template<class Tp>
circle<Tp> circumscribed_circle (const point<Tp> &a, const point<Tp> &b, const point<Tp> &c) {
	const line<Tp> s((a + b) / 2.0, (a + b) / 2.0 + rotate90(b - a));
	const line<Tp> t((b + c) / 2.0, (b + c) / 2.0 + rotate90(c - b));
	const point<Tp> p = cross_point(s, t);
	return circle<Tp>(p, distance(p, a));
}
	

} // end of namespace geo