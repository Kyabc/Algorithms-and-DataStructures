#include <vector>
#include <queue>

std::vector<int> complement_bfs (const std::vector<std::vector<int>> &g, int s) {
	const int n = int(g.size());
	std::vector<int> dist(n, -1), ver; dist[s] = 0;
	for (int i = 0; i < n; i++) if (i != s) ver.push_back(i);
	std::queue<int> que; que.push(s);
	while (not que.empty()) {
		const int v = que.front();
		que.pop();
		int i = 0;
		std::vector<int> next;
		for (const int &u : ver) {
			while (i < g[v].size() and g[v][i] < u) i++;
			if (i < g[v].size() and g[v][i] == u) {
				next.push_back(u);
			} else {
				dist[u] = dist[v] + 1;
				que.push(u);
			}
		}
		ver.swap(next);
	}
	return dist;
}