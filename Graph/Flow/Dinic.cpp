#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <tuple>

template<class T>
struct dinic {
	using value_type = T;
private :
	const value_type INF = std::numeric_limits<value_type>::max();
	struct edge {
	public : 
		int to, rev; value_type cap;
		edge (int to, value_type cap, int rev) :
			to(to), cap(cap), rev(rev) { }
    };
    std::vector<int> level, idx;
    std::vector<std::vector<edge>> g;

	void init (int s) {
		std::fill(level.begin(), level.end(), -1); level[s] = 0;
		std::queue<int> que; que.emplace(s);
		while (not que.empty()) {
			const int v = que.front(); que.pop();
			for (const edge &e : g[v]) {
				if (e.cap > 0 and level[e.to] < 0) {
					level[e.to] = level[v] + 1;
					que.emplace(e.to);
				}
			}
		}
	}

	value_type find_flow (int v, int t, value_type f) {
		if (v == t) return f;
		for (int &i = idx[v]; i < g[v].size(); i++) {
			edge &e = g[v][i];
			if (e.cap > 0 and level[v] < level[e.to]) {
				value_type flow = find_flow(e.to, t, std::min(f, e.cap));
				if (flow > 0) {
					e.cap -= flow;
					g[e.to][e.rev].cap += flow;
					return flow;
				}
			}
		}
		return 0;
	}

public : 
	dinic (int n) : g(n), level(n), idx(n) { }

	dinic (int l, int r, std::vector<std::pair<int, int>> &e) :
		g(l + r + 2), level(l + r + 2), idx(l + r + 2) {
			for (int i = 1; i <= l; i++) add_edge(0, i, 1);
			for (int i = 1; i <= r; i++) add_edge(l + i, l + r + 1, 1);
			for (const auto [u, v] : e) add_edge(u + 1, v + l + 1, 1);
	}

	void add_edge (int from, int to, value_type cost) {
		g[from].emplace_back(to, cost, g[to].size());
		g[to].emplace_back(from, 0, g[from].size() - 1);
	}

	value_type max_flow (int s, int t) {
		value_type ret = 0;
		while (true) {
			init(s);
			if (level[t] < 0) break;
			std::fill(idx.begin(), idx.end(), 0);
			while (true) {
				value_type flow = find_flow(s, t, INF);
				if (flow <= 0) break;
				ret += flow;
			}
		}
		return ret;
	}

	std::vector<std::pair<int, int>> bipartite_matching (int l, int r) {
		max_flow(0, l + r + 1);
		std::vector<std::pair<int, int>> ret;
		for (int v = 1; v <= l; v++) {
			for (const auto &e : g[v]) {
				if (e.to == 0 or e.cap > 0) continue;
				ret.emplace_back(v - 1, e.to - l - 1);
				break;
			}
		}
		return ret;
	}

};