#include <vector>

template<class InputIterator>
std::vector<size_t> z_algorithm (InputIterator first, InputIterator last) {
	const size_t n = (last - first);
	std::vector<size_t> z(n, 0); z[0] = n;
	for (size_t i = 1, j = 0; i < n;) {
		while (i + j < n and *(first + j) == *(first + (i + j))) j++;
		z[i] = j;
		if (j) {
			size_t k = 1;
			while (k < j and k + z[k] < j) {
				z[i + k] = z[k]; k++;
			}
			i += k; j -= k;
		} else {
			i++;
		}
	}
	return z;
}
