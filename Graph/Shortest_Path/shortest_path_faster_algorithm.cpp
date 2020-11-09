#include <vector>
#include <queue>
#include <limits>

template<class Tp>
std::vector<Tp> shortest_path_faster_algorithm (const graph<Tp> &g, size_t s) {
	const Tp infty = std::numeric_limits<Tp>::max();
	std::vector<Tp> dist(g.size(), infty); dist[s] = 0;
	std::vector<size_t> times(g.size(), 0); times[s]++;
	std::vector<bool> inque(g.size(), false); inque[s] = true;
	std::queue<size_t> que({s});

	while (not que.empty()) {
		const size_t v = que.front(); que.pop();
		inque[v] = false;
		for (const auto &e : g[v]) {
			const Tp cost = e.cost + dist[v];
			if (cost >= dist[e.to]) continue;
			dist[e.to] = cost;
			if (not inque[e.to]) {
				if (++times[e.to] >= g.size()) return std::vector<Tp>();
				que.push(e.to); inque[e.to] = true;
			}
		}
	}
	
	return dist;
}