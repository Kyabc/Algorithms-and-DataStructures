#include <vector>
#include <cassert>

template<std::int_fast32_t Modulus, std::int_fast32_t P_Root>
struct number_theoretic_transform {
	using value_type = std::int_fast32_t;
	using mint = modint<Modulus>;
	using polynomial = std::vector<modint<Modulus>>;
private :
	static constexpr value_type mod = Modulus;
	static constexpr mint root = P_Root;

	static void ntt (polynomial &f) noexcept {
		const size_t n = size(f);
		for (size_t m = n; m > 1; m >>= 1) {
			const size_t block_size = n / m;
			const mint zeta = pow(root, (mod - 1) / m);
			for (size_t s = 0; s < block_size; s++) {
				const size_t base = s * m;
				mint power = 1;
				for (size_t i = 0; i < (m >> 1); i++) {
					const mint lhs = f[base + i];
					const mint rhs = f[base + i + (m >> 1)];
					f[base + i] = lhs + rhs;
					f[base + i + (m >> 1)] = (lhs - rhs) * power;
					power *= zeta;
				}
			}
		}
	}

	static void inverse_ntt (polynomial &f) noexcept {
		const size_t n = size(f);
		for (size_t m = 2; m <= n; m <<= 1) {
			const size_t block_size = n / m;
			const mint zeta = pow(root, (mod - 1) - (mod - 1) / m);
			for (size_t s = 0; s < block_size; s++) {
				const size_t base = s * m;
				mint power = 1;
				for (size_t i = 0; i < (m >> 1); i++) {
					const mint lhs = f[base + i];
					const mint rhs = f[base + i + (m >> 1)] * power;
					f[base + i] = lhs + rhs;
					f[base + i + (m >> 1)] = lhs - rhs;
					power *= zeta;
				}
			}
		}
	}

public :

	static polynomial convolution (const polynomial &f, const polynomial &g) {
		if (f.empty() or g.empty()) return std::vector<mint>();
		size_t n = 1;
		while (n < f.size() + g.size()) n <<= 1;
		std::vector<mint> nf(n, 0), ng(n, 0);
		for (size_t i = 0; i < f.size(); i++) nf[i] = f[i];
		for (size_t i = 0; i < g.size(); i++) ng[i] = g[i];
		ntt(nf); ntt(ng);
		for (size_t i = 0; i < n; i++) nf[i] *= ng[i];
		inverse_ntt(nf);
		nf.resize(f.size() + g.size() - 1);
		const mint inverse_n = inverse(mint(n));
		for (auto &value : nf) value *= inverse_n;
		return nf;
	}

};