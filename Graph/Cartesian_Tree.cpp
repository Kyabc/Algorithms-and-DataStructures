#include <vector>
#include <stack>
#include <functional>

template<class T, class Compare = std::less<T>>
std::vector<int> make_cartesian_tree (const std::vector<T> &a, Compare comp = Compare()) {
	const size_t n = a.size();
	std::vector<int> parent(n, -1);
	std::stack<size_t> right_path;
	for (size_t i = 0; i < n; i++) {
		int left = -1;
		while (not right_path.empty() and comp(a[i], a[right_path.top()])) {
			left = right_path.top();
			right_path.pop();
		}
		if (left != -1) parent[left] = i;
		if (not right_path.empty()) parent[i] = right_path.top();
		right_path.push(i);
	}
	return parent;
}