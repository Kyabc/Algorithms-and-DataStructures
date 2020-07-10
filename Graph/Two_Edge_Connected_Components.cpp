#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

struct two_edge_connected_components {
private : 
	struct edge {
		int to, idx;
		edge (int to, int idx) : to(to), idx(idx) { }
	};
	std::vector<std::pair<int, int>> bridges;
	int edge_index = 0;
	std::vector<int> order, low, index;
	std::vector<std::vector<edge>> g;
	std::vector<bool> seen;

	int dfs (int v, int p, int ord) {
		seen[v] = true;
		order[v] = ord++;
		low[v] = order[v];
		for (const edge &e : g[v]) if (e.idx != p) {
			if (seen[e.to]) {
				low[v] = std::min(low[v], order[e.to]);
			} else {
				ord = dfs(e.to, e.idx, ord);
				low[v] = std::min(low[v], low[e.to]);
				if (order[v] < low[e.to]) {
					bridges.push_back(std::minmax(v, e.to));
				}
			}
		}
		return ord;
	}

	void dfs2 (int v, int p, int &num) {
		if (p != -1 and order[p] >= low[v]) {
			index[v] = index[p];
		} else {
			index[v] = num++;
		}
		for (const edge &e : g[v]) {
			if (index[e.to] == -1) {
				dfs2(e.to, v, num);
			}
		}
	}

public : 
	two_edge_connected_components (int n) : 
		order(n), low(n), index(n), g(n), seen(n) { }

	void add_edge (int v, int u) {
		g[v].emplace_back(u, edge_index);
		g[u].emplace_back(v, edge_index);
		edge_index++;
	}

	int build () {
		int num = 0;
		std::fill(seen.begin(), seen.end(), false);
		std::fill(index.begin(), index.end(), -1);
		for (int s = 0; s < g.size(); s++) {
			if (not seen[s]) dfs(s, -1, 0);
		}
		for (int s = 0; s < g.size(); s++) {
			if (index[s] == -1) dfs2(s, -1, num);
		}
		return num;
	}

	std::vector<std::vector<int>> make_tree() {
		std::vector<std::vector<int>> g(build());
		for (const std::pair<int, int> &e : bridges) {
			int a = index[e.first];
			int b = index[e.second];
			g[a].push_back(b);
			g[b].push_back(a);
		}
		return g;
	}

	int operator[] (int i) {
		return index[i];
	}

};