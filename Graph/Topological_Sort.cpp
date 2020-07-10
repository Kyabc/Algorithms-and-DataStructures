#include <vector>
#include <queue>

std::vector<int> topological_sort (const std::vector<std::vector<int>> &g) {
	const int n = g.size();
	std::vector<int> sorted, indeg(n, 0);
	std::vector<bool> processed(n, false);

	for (int v = 0; v < n; v++) {
		for (int u : g[v]) indeg[u]++;
	}

	for (int s = 0; s < n; s++) {
		if (indeg[s] > 0 or processed[s]) continue;
		std::queue<int> que;
		que.push(s);
		processed[s] = true;
		while (not que.empty()) {
			int v = que.front();
			que.pop();
			sorted.push_back(v);
			for (const int &u : g[v]) {
				if (--indeg[u] == 0 and not processed[u]) {
					que.push(u);
					processed[u] = true;
				}
			}
		}
	}

	return sorted;

}