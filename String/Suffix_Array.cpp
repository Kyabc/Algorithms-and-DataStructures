#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdint>

template<class InputIterator>
std::vector<size_t> suffix_array (InputIterator first, InputIterator last) {
	const size_t n = (last - first);
	std::vector<size_t> sa(n);
	std::iota(sa.begin(), sa.end(), 0);
	std::vector<std::int32_t> rank(first, last), buff(n);
	for (size_t k = 1; k < n; k <<= 1) {
		auto compare = [&] (const size_t &lhs, const size_t &rhs) {
			if (rank[lhs] != rank[rhs]) return (rank[lhs] < rank[rhs]);
			int32_t left = (lhs + k < n ? rank[lhs + k] : -1);
			int32_t right = (rhs + k < n ? rank[rhs + k] : -1);
			return (left < right);
		}; 
		std::sort(sa.begin(), sa.end(), compare);
		buff[sa[0]] = 0;
		for (size_t i = 0; i + 1 < n; i++) {
			buff[sa[i + 1]] = buff[sa[i]] + compare(sa[i], sa[i + 1]);
		}
		std::swap(rank, buff);
	}
	return sa;
}
