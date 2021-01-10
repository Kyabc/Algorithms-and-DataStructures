#include <vector>

namespace number_theoretic_transform {

// Compile time primitive root
constexpr int constexpr_primitive_root (int m) {
	if (m == 998244353) return 3;   // 2^23 * 119 + 1
	if (m == 469762049) return 3;   // 2^26 * 7 + 1
	if (m == 167772161) return 3;   // 2^25 * 5 + 1
	if (m == 754974721) return 11;  // 2^24 * 45 + 1
	if (m == 1224736769) return 3;  // 2^24 * 73 + 1
	if (m == 163577857) return 23;  // 2^22 * 39 + 1
	if (m == 935329793) return 3;   // 2^22 * 223 + 1
	if (m == 943718401) return 7;   // 2^22 * 225 + 1
	return -1;
}

template<int m> constexpr int primitive_root = constexpr_primitive_root(m);

template<class mint>
inline std::vector<mint> zeta_pow (size_t n, const mint &zeta) {
	std::vector<mint> zetas((n >> 1), 1);
	for (size_t i = 0; i + 1 < size(zetas); i++) {
		zetas[i + 1] = zetas[i] * zeta;
	}
	return zetas;
}

template<int mod, int g, class mint>
void ntt (std::vector<mint> &f) {
	const size_t n = size(f);
	for (size_t m = n; m > 1; m >>= 1) {
		const auto zetas = zeta_pow(m, pow(mint(g), (mod - 1) / m));
		for (size_t s = 0; s < n; s += m) {
			for (size_t i = 0, j = s; i < (m >> 1); ++i, ++j) {
				const mint lhs = f[j];
				const mint rhs = f[j + (m >> 1)];
				f[j] = lhs + rhs;
				f[j + (m >> 1)] = (lhs - rhs) * zetas[i];
			}
		}
	}
}

template<int mod, int g, class mint>
void inverse_ntt (std::vector<mint> &f) {
	const size_t n = size(f);
	for (size_t m = 2; m <= n; m <<= 1) {
		const auto zetas = zeta_pow(m, pow(mint(g), (mod - 1) - (mod - 1) / m));
		for (size_t s = 0; s < n; s += m) {
			for (size_t i = 0, j = s; i < (m >> 1); ++i, ++j) {
				const mint lhs = f[j];
				const mint rhs = f[j + (m >> 1)] * zetas[i];
				f[j] = lhs + rhs;
				f[j + (m >> 1)] = lhs - rhs;
			}
		}
	}
}

template<class mint>
std::vector<mint> convolution (std::vector<mint> f, std::vector<mint> g) {
	if (f.empty() or g.empty()) return {};
	if (std::min(size(f), size(g)) <= 60) {
		size_t n = size(f), m = size(g);
		if (n < m) { std::swap(n, m); std::swap(f, g); }
		std::vector<mint> ans(n + m - 1, 0);
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) {
				ans[i + j] += f[i] * g[j];
			}
		}
		return ans;
	}
	size_t n = 1;
	static constexpr int mod = mint::mod;
	static constexpr int root = primitive_root<mod>;
	const size_t default_size = size(f) + size(g);
	while (n < default_size) n <<= 1;
	f.resize(n, 0); g.resize(n, 0);
	ntt<mod, root>(f); ntt<mod, root>(g);
	for (size_t i = 0; i < n; i++) f[i] *= g[i];
	inverse_ntt<mod, root>(f);
	f.resize(default_size - 1);
	const mint inv_n = inverse(mint(n));
	for (auto &f_i : f) f_i *= inv_n;
	return f;
}

template<int MOD, class Tp>
inline std::vector<modint<MOD>> sub_multiply (const std::vector<Tp> &f, const std::vector<Tp> &g) {
	std::vector<modint<MOD>> a(size(f)), b(size(g));
	for (size_t i = 0; i < size(f); i++) a[i] = modint<MOD>(get_value(f[i]));
	for (size_t i = 0; i < size(g); i++) b[i] = modint<MOD>(get_value(g[i]));
	return convolution(a, b);
}

// 乗算後の値が 59501818244292734739283969 (> (10^9 + 7)^2 * 5 * 10^7) まで対応 
template<class mint>
std::vector<mint> arbitary_convolution (const std::vector<mint> &f, const std::vector<mint> &g) {
	if (std::min(size(f), size(g)) <= 60) return sub_multiply<mint::get_mod()>(f, g);
	static constexpr int MOD0 = 167772161;           // 2^25 * 5 + 1
	static constexpr int MOD1 = 469762049;           // 2^26 * 7 + 1
	static constexpr int MOD2 = 754974721;           // 2^24 * 45 + 1
	static constexpr mint M0 = 167772161;            // MOD0             (mod MOD)
	static constexpr mint M0M1 = 564826938;          // MOD0 * MOD1      (mod MOD)
	static constexpr modint<MOD1> I0 = 104391568;    // MOD0^-1          (mod MOD1)
	static constexpr modint<MOD2> I1 = 399692502;    // MOD1^-1          (mod MOD2)
	static constexpr modint<MOD2> I0I1 = 190329765;  // (MOD0 * MOD1)^-1 (mod MOD2)
	const auto r0 = sub_multiply<MOD0>(f, g);
	const auto r1 = sub_multiply<MOD1>(f, g);
	const auto r2 = sub_multiply<MOD2>(f, g);
	std::vector<mint> h(r0.size());
	for (size_t i = 0; i < size(h); i++) {
		const int x0 = get_value(r0[i]);
		const int x1 = get_value(I0 * (r1[i] - x0));
		const int x2 = get_value(I0I1 * (r2[i] - x0) - I1 * x1);
		h[i] = M0M1 * x2 + M0 * x1 + x0;
	}
	return h;
}


} // namespace number_theoretic_transform