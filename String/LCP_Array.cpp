template<class InputIterator>
std::vector<size_t> lcp_array (InputIterator first, InputIterator last) {
	if (first == last) return std::vector<size_t>();
	const size_t n = std::distance(first, last);
	const std::vector<size_t> sa = suffix_array(first, last);
	const std::vector<char> s(first, last);
	std::vector<size_t> rank(n), lcp(n - 1);
	for (size_t i = 0; i < n; i++) rank[sa[i]] = i;
	for (size_t i = 0, h = 0; i < n; i++) {
		if (h > 0) h--;
		if (rank[i] == 0) continue;
		const size_t &j = sa[rank[i] - 1];
		while (j + h < n and i + h < n and s[j + h] == s[i + h]) h++;
		lcp[rank[i] - 1] = h;
	}
	return lcp;
}