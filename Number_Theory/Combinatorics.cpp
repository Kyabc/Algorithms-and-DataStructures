#include <vector>

template<int Modulus>
class combinatorics_t {
public :
	using size_type = std::size_t;
	using value_type = modint<Modulus>;
private :
	inline static std::vector<value_type> fact = {1, 1};
	inline static std::vector<value_type> ifact = {1, 1};
public :
	static void build (size_type n) {
		if (n < fact.size()) return;
		const size_type m = fact.size();
		fact.resize(n + 1); ifact.resize(n + 1);
		for (size_type i = m; i < n + 1; i++) {
			fact[i] = fact[i - 1] * i;
		}
		ifact[n] = inverse(fact[n]);
		for (size_type i = n; i > m; i--) {
			ifact[i - 1] = ifact[i] * i;
		}
	}

	static const value_type &factorial (size_type n) {
		build(n);
		return fact[n];
	}

	static const value_type &inverse_factorial (size_type n) {
		build(n);
		return ifact[n];
	}

	static value_type combination (size_type n, size_type r) {
		if (n < r) return value_type(0);
		build(n);
		return fact[n] * ifact[n - r] * ifact[r];
	}

	static value_type permutation (size_type n, size_type r) {
		if (n < r) return value_type(0);
		build(n);
		return fact[n] * ifact[n - r];
	}

	struct naive {
		static value_type combination (size_type n, size_type r) {
			if (n < r) return value_type(0);
			if (n - r < r) r = n - r;
			value_type value = 1;
			for (size_type i = n; i > n - r; i--) value *= i;
			for (size_type i = 2; i <= r; i++) value /= i;
			return value;
		}
	};

};

using combinatorics998244353 = combinatorics_t<998244353>;
using combinatorics1000000007 = combinatorics_t<1000000007>;
using combinatorics1000000009 = combinatorics_t<1000000009>;