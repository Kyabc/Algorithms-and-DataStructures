#include <vector>
#include <limits>
#include <algorithm>
#include <cstddef>

template<class Tp>
std::vector<std::ptrdiff_t> hungarian (const std::vector<std::vector<Tp>> &g) {
	using isize = std::ptrdiff_t;
	using value_type = Tp;
	if (g.empty()) return std::vector<isize>();
	const isize n = g.size(), m = g[0].size();
	std::vector<isize> lmatch(n, -1), rmatch(m, -1);
	std::vector<value_type> lweight(n, 0), rweight(m, 0);
	auto cost = [&] (isize i, isize j) { return g[i][j] + lweight[i] + rweight[j]; };
	for (isize s = 0; s < n; s++) {
		std::vector<value_type> match(m, -1), ptr(m, s);
		std::vector<bool> lseen(n, false), rseen(m, false); lseen[s] = true;
		while (true) {
			isize right = -1;
			value_type weight = std::numeric_limits<Tp>::max();
			for (isize j = 0; j < m; j++) {
				if (not rseen[j]) weight = std::min(weight, cost(ptr[j], j));
			}
			for (isize i = 0; i < n; i++) if (lseen[i]) lweight[i] -= weight;
			for (isize j = 0; j < m; j++) if (rseen[j]) rweight[j] += weight;
			for (isize j = 0; j < m; j++) {
				if (not rseen[j] and cost(ptr[j], j) == 0) right = j;
			}
			isize index = rmatch[right];
			match[right] = ptr[right];
			if (index < 0) {
				while (right >= 0) {
					const isize left = match[right], buff = lmatch[left];
					rmatch[right] = left; lmatch[left] = right; right = buff;
				}
				break;
			}
			lseen[index] = rseen[right] = true;
			for (isize j = 0; j < m; j++) {
				if (cost(index, j) < cost(ptr[j], j)) ptr[j] = index;
			}
		}
	}
	return lmatch;
}