#include <vector>

template<class InputIterator>
std::vector<std::ptrdiff_t> moris_pratt (InputIterator first, InputIterator last) {
	const std::ptrdiff_t n = std::distance(first, last);
	std::vector<std::ptrdiff_t> mp(n + 1); mp[0] = -1;
	for (std::ptrdiff_t i = 0, j = -1; i < n; i++) {
		while (j >= 0 and *(first + i) != *(first + j)) j = mp[j];
		mp[i + 1] = ++j;
	}
	mp.erase(mp.begin());
	return mp;
}