#include <vector>
#include <stack>
#include <cstddef>
#include <algorithm>

struct strongly_connected_component {
	using size_type = std::size_t;
private :
	size_type num;
	std::vector<std::vector<size_type>> g, scc;
	std::vector<size_type> low, order, index;
	std::vector<bool> instack;
	std::stack<size_type> sta;

	void chmin (size_type &a, const size_type &b) noexcept {
		if (a > b) a = b;
	}

	void dfs (size_type v) noexcept {
		low[v] = order[v] = ++num;
		sta.push(v); instack[v] = true;
		for (const size_type &u : g[v]) {
			if (order[u] == 0) {
				dfs(u);
				chmin(low[v], low[u]);
			} else if (instack[u]) {
				chmin(low[v], order[u]);
			}
		}
		if (low[v] == order[v]) {
			scc.push_back(std::vector<size_type>());
			while (true) {
				const size_type u = sta.top();
				sta.pop(); instack[u] = false;
				scc.back().push_back(u);
				if (u == v) break;
			}
		}
	}

public :
	explicit strongly_connected_component (size_type n)
	: num(0), g(n), scc(), low(n, 0), order(n, 0), index(n), instack(n, false), sta() { }

	void add_edge (size_type v, size_type u) noexcept {
		g[v].push_back(u);
	}

	size_type solve () noexcept {
		for (size_type v = 0; v < g.size(); v++) if (not order[v]) dfs(v);
		std::reverse(scc.begin(), scc.end());
		for (size_type i = 0; i < scc.size(); i++) {
			for (const size_type &v : scc[i]) index[v] = i;
		}
		return scc.size();
	}

	const std::vector<std::vector<size_type>> &get_scc () const noexcept {
		return scc;
	}

	size_type size () const noexcept {
		return scc.size();
	}

	const size_type &operator[] (size_type v) const noexcept {
		return index[v];
	}

};