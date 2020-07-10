//
//  Dinic
//
//  cf.
//    蟻本 p.194-195
//
//  verify
//    AIZU ONLINE JUDGE - Maximum Flow
//      http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4434197#1
//	  Library Checker - Matching on Bipartite Graph
//		https://judge.yosupo.jp/submission/10798
//


template<class T>
struct Dinic {
	using value_type = T;
private :
	const value_type INF = numeric_limits<value_type>::max();
	struct Edge {
	public : 
		int to, rev; value_type cap;
		Edge (int to, value_type cap, int rev) :
			to(to), cap(cap), rev(rev) { }
    };
    vector<int> level, idx;
    vector<vector<Edge>> g;
public : 
	Dinic (int n) : g(n), level(n), idx(n) { }

	Dinic (int l, int r, vector<pair<int, int>> &e) : g(l + r + 2), level(l + r + 2), idx(l + r + 2) {
		for (int i = 1; i <= l; i++) add_edge(0, i, 1);
		for (int i = 1; i <= r; i++) add_edge(l + i, l + r + 1, 1);
		for (const auto [u, v] : e) add_edge(u + 1, v + l + 1, 1);
	}

	void add_edge (int from, int to, value_type cost) {
		g[from].emplace_back(to, cost, g[to].size());
		g[to].emplace_back(from, 0, g[from].size() - 1);
	}

	void init (int s) {
		fill(level.begin(), level.end(), -1); level[s] = 0;
		queue<int> que; que.emplace(s);
		while (not que.empty()) {
			const int v = que.front(); que.pop();
			for (const Edge &e : g[v]) {
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
			Edge &e = g[v][i];
			if (e.cap > 0 and level[v] < level[e.to]) {
				value_type flow = find_flow(e.to, t, min(f, e.cap));
				if (flow > 0) {
					e.cap -= flow;
					g[e.to][e.rev].cap += flow;
					return flow;
				}
			}
		}
		return 0;
	}

	value_type max_flow (int s, int t) {
		value_type ret = 0;
		while (true) {
			init(s);
			if (level[t] < 0) break;
			fill(idx.begin(), idx.end(), 0);
			while (true) {
				value_type flow = find_flow(s, t, INF);
				if (flow <= 0) break;
				ret += flow;
			}
		}
		return ret;
	}

	vector<pair<int, int>> bipartite_matching (int l, int r) {
		max_flow(0, l + r + 1);
		vector<pair<int, int>> ret;
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



int main() {
	ios::sync_with_stdio(false);
    cin.tie(nullptr);
	int l, r, m;
	cin >> l >> r >> m;
	vector<pair<int, int>> e(m);
	for (int i = 0; i < m; i++) {
		cin >> e[i].first >> e[i].second;
	}
	
	Dinic<int> g(l, r, e);
	
	vector<pair<int, int>> ans = g.bipartite_matching(l, r);
	
	cout << ans.size() << '\n';
	for (auto [u, v] : ans) cout << u << ' ' << v << '\n';
	
	return 0;
}