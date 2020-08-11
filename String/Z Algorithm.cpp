#include <vector>

template<class RandomAccessIterator>
std::vector<int> z_algorithm (RandomAccessIterator first, RandomAccessIterator last) {
	const int n = (last - first);
	std::vector<int> z(n, 0); z[0] = n;
	for (int i = 1, j = 0; i < n;) {
		while (i + j < n and *(first + j) == *(first + (i + j))) j++;
		z[i] = j;
		if (j) {
			int k = 1;
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