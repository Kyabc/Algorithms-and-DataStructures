#include <vector>

struct lowest_common_ancestor {
private : 
	std::size_t h;
	std::vector<std::vector<int>> g, par;
	std::vector<int> dep;

public :
	lowest_common_ancestor (std::size_t n) : g(n), dep(n) {
		h = 0;
		while ((1 << h) < n) h++;
		par.assign(h, std::vector<int>(n));
	}

	lowest_common_ancestor (const std::vector<std::vector<int>> &g, int r = 0) :
		g(g), dep(g.size()) {
			h = 1;
			while ((1 << h) < g.size()) h++;
			par.assign(h, std::vector<int>(g.size()));
			build(r);
	}

	void add_edge (int v, int u) {
		g[v].push_back(u);
		g[u].push_back(v);
	}

	void build (int r = 0) {
		par[0][r] = r; dep[r] = 0;
		std::vector<int> sta; sta.push_back(r);
		while (not sta.empty()) {
			int v = sta.back(); sta.pop_back();
			for (int u : g[v]) if (u != par[0][v]) {
				par[0][u] = v;
				dep[u] = dep[v] + 1;
				sta.push_back(u);
			}
		}

		for (std::size_t i = 1; i < h; i++) {
			for (std::size_t j = 0; j < g.size(); j++) {
				par[i][j] = par[i - 1][par[i - 1][j]];
			}
		}
	}

	int level_ancester (int v, int k) {
		for (int i = 0; i < h; i++) {
			if ((k >> i) & 1) v = par[i][v];
		}
		return v;
	}

	int lca (int v, int u) {
		if (dep[v] < dep[u]) std::swap(v, u);
		v = level_ancester(v, dep[v] - dep[u]);
		if (v == u) return v;
		for (std::size_t i = h; i --> 0;) {
			if (par[i][v] != par[i][u]) {
				v = par[i][v];
				u = par[i][u];
			}
		}
		return par[0][v];
	}

	int distance (int v, int u) {
		int p = lca(v, u);
		return (dep[v] + dep[u] - 2 * dep[p]);
	}

};