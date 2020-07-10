#include <vector>
#include <algorithm>

struct strongly_connected_component {
private : 
	std::vector<std::vector<int>> g, revg;
	std::vector<bool> seen;
	std::vector<int> order, index;

	void dfs (int v) {
		seen[v] = true;
		for (int u : g[v]) {
			if (not seen[u]) dfs(u);
		}
		order.push_back(v);
	}

	void dfs2 (int v, const int idx) {
		seen[v] = true;
		index[v] = idx;
		for (int u : revg[v]) {
			if (not seen[u]) dfs2(u, idx);
		}
	}

public : 
	strongly_connected_component (int n) : 
		g(n), revg(n), index(n), seen(n) { }

	void add_edge (int u, int v) noexcept {
		g[u].push_back(v);
		revg[v].push_back(u);
	}

	int build() {
		order.clear();
		std::fill(seen.begin(), seen.end(), false);
		for (int v = 0; v < g.size(); v++) {
			if (not seen[v]) dfs(v);
		}
		
		int idx = 0;
		std::fill(seen.begin(), seen.end(), false);
		std::reverse(order.begin(), order.end());
		for (int v : order) if (not seen[v]) dfs2(v, idx++);
		return idx;
	}

	int operator[] (int v) {
		return (index[v]);
	}

};