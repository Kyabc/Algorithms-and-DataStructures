#include <vector>

template<class T>
using matrix = std::vector<std::vector<T>>;

template<class T>
matrix<T> make_matrix(const T &x, std::size_t h, std::size_t w) {
	return (matrix<T>(h, std::vector<T>(w, x)));
}

template<class T>
void mat_mul(matrix<T> &a, const matrix<T> &b) {
	auto mat = make_matrix(T(0), a.size(), b[0].size());
	for (std::size_t i = 0; i < a.size(); i++) {
		for (std::size_t k = 0; k < b.size(); k++) {
			for (std::size_t j = 0; j < b[0].size(); j++) {
				mat[i][j] += (a[i][k] * b[k][j]);
			}
		}
	}
	mat.swap(a);
}

template<class T>
void mat_pow (matrix<T> &mat, long long k) {
	const std::size_t n = mat.size();
	auto a = make_matrix(T(0), n, n);
	for (int i = 0; i < n; i++) a[i][i] = ie;
	while (k > 0) {
		if (k & 1) mat_mul(a, mat);
		mat_mul(mat, mat);
		k >>= 1LL;
	}
	a.swap(mat);
}