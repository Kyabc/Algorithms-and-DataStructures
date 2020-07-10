#include <vector>
#include <tuple>
#include <algorithm>

struct low_link {
public :
	std::vector<int> articulation;
	std::vector<std::pair<int, int>> bridge;
private :
	struct edge {
		int to, idx;
		edge (int to, int idx) : to(to), idx(idx) { }
	};
	int edge_index = 0;
	std::vector<std::vector<edge>> g;
	std::vector<int> order, low;
	std::vector<bool> seen;

	int dfs (int v, int p, int ord) {
		seen[v] = true;
		order[v] = ord++;
		low[v] = order[v];
		int child = 0;
		bool isart = false;
		for (const edge &e : g[v]) if (p != e.idx) {
			if (seen[e.to]) {
				low[v] = std::min(low[v], order[e.to]);
			} else {
				child++;
				ord = dfs(e.to, e.idx, ord);
				low[v] = std::min(low[v], low[e.to]);
				if (p != -1) isart |= (order[v] <= low[e.to]);
				if (order[v] < low[e.to]) {
					bridge.push_back(std::minmax(v, e.to));
				}
			}
		}
		isart |= (p == -1 and child >= 2);
		if (isart) articulation.push_back(v);
		return ord;
	}

public :
	low_link (int n) : g(n), order(n), low(n), seen(n) { }

	void add_edge (int v, int u) {
		g[v].emplace_back(u, edge_index);
		g[u].emplace_back(v, edge_index);
		edge_index++;
	}

	void build () {
		bridge.clear(); articulation.clear();
		fill(seen.begin(), seen.end(), false);
		for (int s = 0; s < g.size(); s++) {
			if (not seen[s]) dfs(s, -1, 0);
		}
	}

};