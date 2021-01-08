// Cooley-Tukey FFT

#include <vector>
#include <complex>
#include <cmath>

template<class Tp = double>
struct fast_fourier_transform {
	using complex_type = std::complex<Tp>;
	using value_type = Tp;
private :
	static constexpr value_type pi = std::acos(-1);

	static void dft (std::vector<complex_type> &f) {
		const size_t n = size(f);
		for (size_t m = n; m > 1; m >>= 1) {
			const size_t block_size = n / m;
			const complex_type zeta = std::polar(1.0, 2.0 * pi / m);
			for (size_t s = 0; s < block_size; s++) {
				const size_t base = s * m;
				complex_type power = 1;
				for (size_t i = 0; i < (m >> 1); i++) {
					const complex_type lhs = f[base + i];
					const complex_type rhs = f[base + i + (m >> 1)];
					f[base + i] = lhs + rhs;
					f[base + i + (m >> 1)] = (lhs - rhs) * power;
					power *= zeta;
				}
			}
		}
	}

	static void inverse_dft (std::vector<complex_type> &f) {
		const size_t n = size(f);
		for (size_t m = 2; m <= n; m <<= 1) {
			const size_t block_size = n / m;
			const complex_type zeta = std::polar(1.0, -2.0 * pi / m);
			for (size_t s = 0; s < block_size; s++) {
				const size_t base = s * m;
				complex_type power = 1;
				for (size_t i = 0; i < (m >> 1); i++) {
					const complex_type lhs = f[base + i];
					const complex_type rhs = f[base + i + (m >> 1)] * power;
					f[base + i] = lhs + rhs;
					f[base + i + (m >> 1)] = lhs - rhs;
					power *= zeta;
				}
			}
		}
	}

public :
	template<class U>
	static std::vector<Tp> convolution (const std::vector<U> &f, const std::vector<U> &g) {
		size_t n = 1;
		while (n < f.size() + g.size()) n <<= 1;
		std::vector<complex_type> nf(n, 0), ng(n, 0);
		for (size_t i = 0; i < f.size(); i++) nf[i] = f[i];
		for (size_t i = 0; i < g.size(); i++) ng[i] = g[i];
		dft(nf); dft(ng);
		for (size_t i = 0; i < n; i++) nf[i] *= ng[i];
		inverse_dft(nf);
		std::vector<Tp> ans(n);
		for (size_t i = 0; i < n; i++) ans[i] = (nf[i].real() / n);
		return ans;
	}

};