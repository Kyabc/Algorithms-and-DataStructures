#include <bits/stdc++.h>
// config
namespace config {
__attribute__((constructor)) void _setup_ () noexcept {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout << std::fixed << std::setprecision(10);
}
} // namespace config
// iostream of std::vector
template<class Tp> std::istream &operator>> (std::istream &is, std::vector<Tp> &v) noexcept { for (Tp &x : v) is >> x; return is; }
template<class Tp> std::ostream &operator<< (std::ostream &os, const std::vector<Tp> &v) noexcept { std::cerr << '['; for (size_t i = 0; i < size(v); i++) { os << v[i]; if (i + 1 != size(v)) os << ' '; } std::cerr << ']'; return os; }
// chmin/max 
template<class Tp, class Compare = std::greater<Tp>> constexpr inline bool chmin (Tp &a, const Tp &b, Compare comp = Compare{}) noexcept { if (comp(a, b)) { a = b; return true; } return false; }
template<class Tp, class Compare = std::less<Tp>> constexpr inline bool chmax (Tp &a, const Tp &b, Compare comp = Compare{}) noexcept { return chmin<Tp, Compare>(a, b, comp); }
// zero judgement 
template<class Tp, Tp ZERO = Tp(0)> constexpr bool zero (const Tp &x) noexcept { return x == ZERO; }
template<class Tp, Tp ZERO = Tp(0), class... Args> constexpr bool zero (const Tp &x, const Args& ...args) noexcept { return zero(x) and zero(args...); }
// min/max 
template<class... Args> constexpr auto min (const Args&... args) noexcept { return std::min(std::initializer_list<std::common_type_t<Args...>>{args...}); }
template<class... Args> constexpr auto max (const Args&... args) noexcept { return std::max(std::initializer_list<std::common_type_t<Args...>>{args...}); }
// standard input 
template<class... Args> void input (Args&... args) noexcept { (std::cin >> ... >> args); }
// standard output
void print () noexcept { std::cout << '\n'; }
template<class Tp, class... Args> void print (const Tp &first, const Args&... args) noexcept { std::cout << first; (std::cout << ... << (std::cout << ' ', args)); std::cout << '\n'; }
template<class Tp> void drop (const Tp &x) noexcept { std::cout << x << '\n'; std::exit(0); }
// error output 
void error () noexcept { std::cerr << '\n'; }
template<class Tp, class... Args> void error (const Tp &first, const Args&... args) noexcept { std::cerr << first; (std::cout << ... << (std::cerr << ' ', args)); std::cerr << '\n'; }
template<class Tp> void dumpout (const Tp &x) noexcept { std::cerr << x << '\n'; std::exit(0); }
// infinity class
struct { template<class Tp> constexpr operator Tp() noexcept { return std::numeric_limits<Tp>::max() / 2; } constexpr auto operator- () noexcept; } infty;
struct { template<class Tp> constexpr operator Tp() noexcept { return std::numeric_limits<Tp>::lowest() / 2; } constexpr auto operator- () noexcept; } negative_infty;
constexpr auto decltype(infty)::operator- () noexcept { return negative_infty; }
constexpr auto decltype(negative_infty)::operator- () noexcept { return infty; }
// min/max class
template<class Tp> struct minimum { constexpr Tp operator() (const Tp &x, const Tp &y) const { return (x < y ? x : y); } };
template<class Tp> struct maximum { constexpr Tp operator() (const Tp &x, const Tp &y) const { return (x > y ? x : y); } };
// python-like enumerate
template<class C> auto enumerate (const C &a) noexcept { std::vector<std::pair<typename C::value_type, size_t>> b; for (size_t i = 0; i < a.size(); i++) b.emplace_back(a[i], i); return b; }
// make index
template<class F> std::vector<size_t> make_index (size_t n, const F &f) noexcept { std::vector<size_t> v(n); std::iota(v.begin(), v.end(), 0); std::sort(v.begin(), v.end(), f); return v; }
// make vector
template<class F> auto make_vector (size_t n, const F &f) noexcept { std::vector<decltype(f(0))> v(n); for (size_t i = 0; i < n; i++) v[i] = f(i); return v; }
// alias
template<class Tp> using min_heap = std::priority_queue<Tp, std::vector<Tp>, std::greater<Tp>>;
using i32 = std::int_fast32_t;
using u32 = std::uint_fast32_t;
using i64 = std::int_fast64_t;
using u64 = std::uint_fast64_t;
using i128 = __int128_t;
using u128 = __uint128_t;
using isize = std::ptrdiff_t;
using usize = std::size_t;

// ---------------------------------------------------------------------------------------------------------------< template >-------------------------------------------------------------------------------------------------------------- 

int main() {
	print("This is kya's templates");
	print("Must be compile with C++17 or C++20");
	return 0;
}