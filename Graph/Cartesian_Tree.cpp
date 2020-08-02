#include <vector>
#include <stack>
#include <functional>

template<class T, class Compare = std::less<T>>
std::vector<int> make_cartesian_tree (const std::vector<T> &a) {
	const Compare comp = Compare();
	const int n = int(a.size());
	std::vector<int> parent(n, -1);
	std::stack<int> stack;
	for (int i = 0; i < n; i++) {
		int left = -1;
		while (not stack.empty() and comp(a[i], a[stack.top()])) {
			left = stack.top();
			stack.pop();
		}
		if (left != -1) parent[left] = i;
		if (not stack.empty()) parent[i] = stack.top();
		stack.push(i);
	}
	return parent;
}