#include <iostream>
#include <cstdint>
#include <cassert>

template<std::int_fast32_t Modulus>
struct modint {
	using value_type = std::int_fast32_t;
	static constexpr value_type mod = Modulus;
	static_assert(mod > 1, "Modulus must be greater than 1");
private :
	value_type x;
	template<class T> static constexpr value_type normalize (T value) noexcept {
		if (value < 0) { value = (-value) % mod; return (value ? value : mod - value); }
		return (value % mod);
	}
public :
	constexpr modint () noexcept : x(0) { }
	template<class T> constexpr modint (T value) noexcept : x(normalize(value)) { }
	constexpr modint operator+ () const noexcept { return modint(*this); }
	constexpr modint operator- () noexcept { if (x != 0) x = mod - x; return modint(*this); }
	constexpr modint &operator+= (const modint &p) noexcept { if ((x += p.x) >= mod) x -= mod; return (*this); }
	constexpr modint &operator-= (const modint &p) noexcept { if ((x += mod - p.x) >= mod) x -= mod; return (*this); }
	constexpr modint &operator*= (const modint &p) noexcept { x = (int)(1LL * x * p.x % mod); return (*this); }
	constexpr modint &operator/= (const modint &p) noexcept { return ((*this) *= inverse(p)); }
	constexpr modint operator+ (const modint &p) const noexcept { return (modint(*this) += p); }
	constexpr modint operator- (const modint &p) const noexcept { return (modint(*this) -= p); }
	constexpr modint operator* (const modint &p) const noexcept { return (modint(*this) *= p); }
	constexpr modint operator/ (const modint &p) const noexcept { return (modint(*this) /= p); }
	constexpr bool operator== (const modint &p) const noexcept { return (x == p.x); }
	constexpr bool operator!= (const modint &p) const noexcept { return (x != p.x); }
	friend constexpr std::ostream &operator<< (std::ostream &os, const modint &p) { return (os << p.x); }
	friend constexpr std::istream &operator>> (std::istream &is, modint &p) { long long v; is >> v; p = modint(v); return (is); }
	friend constexpr modint inverse (const modint<Modulus> &value) noexcept { return pow(value, mod - 2); }
	friend constexpr modint<Modulus> pow (modint<Modulus> value, std::uint_fast64_t idx) noexcept {
		modint<Modulus> ret = 1;
		while (idx > 0) { if (idx & 1) ret *= value; value *= value; idx >>= 1; }
		return ret;
	}
};