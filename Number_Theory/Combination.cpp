#include <vector>

template<std::int_fast32_t mod>
struct combination {
private :
	std::vector<modint<mod>> fact, ifact;
public : 
	combination(std::size_t n) : fact(n + 1), ifact(n + 1) {
		fact[0] = 1;
		for (std::size_t i = 1; i < n + 1; i++) fact[i] = (fact[i - 1] * i);
		ifact[n] = inverse(fact[n]);
		for (std::size_t i = n; i > 0; i--) ifact[i - 1] = ifact[i] * i;
	}

	modint<mod> operator() (std::size_t n, std::size_t r) {
		if (r < 0 or r > n) return 0;
		return fact[n] * ifact[n - r] * ifact[r];
	}

	constexpr const modint<mod> &operator[] (std::size_t i) const {
		return fact[i];
	}

};



template<int mod>
modint<mod> binom(int n, int r) {
    if (r < 0 or r > n) return 0;
    if (n - r < r) r = n - r;
    modint<MOD> ret = 1;
    for (int i = n; i > n - r; i--) ret *= i;
    for (int i = 2; i <= r; i++) ret /= i;
    return ret;
}