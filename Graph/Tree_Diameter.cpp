#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <functional>

template<class T>
std::pair<T, std::vector<int>> tree_diametor (const graph<T> &g) {
	int n = int(g.size());
	std::vector<int> par(n);
	std::vector<T> dist(n);

	std::function<void(int)> bfs = [&] (int s) -> void {
		std::fill(par.begin(), par.end(), -1);
		std::fill(dist.begin(), dist.end(), 0);
		std::queue<int> que; que.push(s);
		while (not que.empty()) {
			int v = que.front(); que.pop();
			for (auto &e : g[v]) {
				if (e.to == par[v]) continue;
				dist[e.to] = dist[v] + e.cost;
				par[e.to] = v;
				que.push(e.to);
			}
		}
	};

	bfs(0);
	int vertex1 = (std::max_element(dist.begin(), dist.end()) - dist.begin());

	bfs(vertex1);
	int vertex2 = (std::max_element(dist.begin(), dist.end()) - dist.begin());

	std::vector<int> path;
	for (int v = vertex2; v != -1; v = par[v]) {
		path.push_back(v);
	}
	
	return std::make_pair(dist[vertex2], path);

}