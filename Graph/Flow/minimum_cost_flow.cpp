#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>
#include <limits>

template<class T>
struct min_cost_flow {
	using value_type = T;
	using P = std::pair<value_type, int>;
private : 
	const value_type INF = std::numeric_limits<value_type>::max();
	struct edge {
	public : 
		int to, rev;
		value_type cap;
		value_type cost;
		edge (int to, value_type cap, value_type cost, int rev) : 
			to(to), cap(cap), cost(cost), rev(rev) { }
	};
	int n;
	std::vector<std::vector<edge>> g;
	std::vector<value_type> potential;
	std::vector<value_type> dist;
	std::vector<int> prev_vertex;
	std::vector<int> prev_edge;

	template<class E>
	bool chmin (E &a, const E &b) {
		if (a > b) { a = b; return true; }
		return false;
	}

public : 
	min_cost_flow (int n) :
		n(n), g(n), potential(n), dist(n), prev_vertex(n), prev_edge(n) { }

	void add_edge (int from, int to, value_type cap, value_type cost) {
		g[from].emplace_back(to, cap, cost, g[to].size());
		g[to].emplace_back(from, 0, -cost, g[from].size() - 1);
	}

	value_type solve (int s, int t, value_type f) {
		value_type ret = 0;
		std::fill(potential.begin(), potential.end(), 0);
		while (f > 0) {
			std::priority_queue<P, std::vector<P>, std::greater<P>> que;
			std::fill(dist.begin(), dist.end(), INF);
			dist[s] = 0;
			que.emplace(dist[s], s);
			while (not que.empty()) {
				value_type d; int v;
				std::tie(d, v) = que.top(); que.pop();
				if (dist[v] < d) continue;
				for (int i = 0; i < int(g[v].size()); i++) {
					const edge &e = g[v][i];
					if (e.cap > 0 and chmin(dist[e.to], dist[v] + e.cost + potential[v] - potential[e.to])) {
						prev_vertex[e.to] = v;
						prev_edge[e.to] = i;
						que.emplace(dist[e.to], e.to);
					}
				}
			}
			if (dist[t] == INF) return -1;
			for (int v = 0; v < n; v++) potential[v] += dist[v];
			value_type val = f;
			for (int v = t; v != s; v = prev_vertex[v]) {
				chmin(val, g[prev_vertex[v]][prev_edge[v]].cap);
			}
			f -= val;
			ret += (val * potential[t]);
			for (int v = t; v != s; v = prev_vertex[v]) {
				edge &e = g[prev_vertex[v]][prev_edge[v]];
				e.cap -= val;
				g[v][e.rev].cap += val;
			}
		}
		return ret;
	}

};