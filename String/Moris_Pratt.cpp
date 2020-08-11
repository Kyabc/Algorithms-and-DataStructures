#include <vector>

template<class RandomAccessIterator>
std::vector<int> moris_pratt (RandomAccessIterator first, RandomAccessIterator last) {
	const int n = (last - first);
	std::vector<int> mp(n + 1); mp[0] = -1;
	for (int i = 0, j = -1; i < n; i++) {
		while (j >= 0 and *(first + i) != *(first + j)) j = mp[j];
		mp[i + 1] = ++j;
	}
	mp.erase(mp.begin());
	return mp;
}