#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <cassert>

struct heavy_light_decomposition {
private :
	size_t count;
	std::vector<std::vector<size_t>> g;
	std::vector<size_t> index, parent, head;

	void dfs (size_t r, std::vector<size_t> &heavy) noexcept {
		std::vector<size_t> subtree(size(), size());
		std::stack<size_t> sta({r});
		while (not sta.empty()) {
			const size_t v = sta.top();
			if (subtree[v] == size()) {
				subtree[v] = 1;
				for (const auto &u : g[v]) {
					if (u == parent[v]) continue;
					parent[u] = v; sta.push(u);
				}
			} else {
				sta.pop();
				size_t max_size = 0;
				for (const auto &u : g[v]) {
					if (u == parent[v]) continue;
					subtree[v] += subtree[u];
					if (max_size < subtree[u]) {
						max_size = subtree[u];
						heavy[v] = u;
					}
				}
			}
		}
	}

	void decompose (size_t r, size_t &idx, const std::vector<size_t> &heavy) noexcept {
		std::queue<size_t> que({r});
		while (not que.empty()) {
			const size_t h = que.front(); que.pop();
			for (size_t v = h; v != size(); v = heavy[v]) {
				head[v] = h; index[v] = idx++;
				for (const auto &u : g[v]) {
					if (u == parent[v] or u == heavy[v]) continue;
					que.push(u);
				}
			}
		}
	}

public :
	heavy_light_decomposition (size_t n)
	noexcept : count(0), g(n), index(n, n), parent(n, n), head(n, n) { }

	size_t size () const noexcept {
		return g.size();
	}

	void add_edge (size_t v, size_t u) noexcept {
		assert(v < size() and u < size() and ++count < size());
		g[v].push_back(u); g[u].push_back(v);
	}

	void build () noexcept {
		size_t idx = 0;
		std::vector<size_t> heavy(size(), size());
		for (size_t r = 0; r < size(); r++) {
			if (parent[r] == size()) dfs(r, heavy);
		}
		for (size_t r = 0; r < size(); r++) {
			if (index[r] == size()) decompose(r, idx, heavy);
		}
	}

	void build (const std::vector<size_t> &roots) noexcept {
		size_t idx = 0;
		std::vector<size_t> heavy(size(), size());
		for (const auto &r : roots) {
			if (parent[r] == size()) dfs(r, heavy);
		}
		for (const auto &r : roots) {
			if (index[r] == size()) decompose(r, idx, heavy);
		}
	}

	size_t lca (size_t u, size_t v) const noexcept {
		assert(u < size() and v < size());
		for (; head[u] != head[v]; u = parent[head[u]]) {
			if (index[u] < index[v]) std::swap(u, v);
		}
		return (index[u] < index[v] ? u : v);
	}

	template<class Op>
	void vertex_path (size_t u, size_t v, const Op &op) const noexcept {
		assert(u < size() and v < size());
		std::vector<std::pair<size_t, size_t>> right;
		while (true) {
			if (head[u] == head[v]) { op(index[u], index[v]); break; }
			if (index[u] > index[v]) {
				op(index[u], index[head[u]]);
				u = parent[head[u]];
			} else {
				right.emplace_back(index[head[v]], index[v]);
				v = parent[head[v]];
			}
		}
		for (size_t i = right.size(); i > 0; i--) {
			auto [l, r] = right[i - 1]; op(l, r);
		}
	}

	template<class Op>
	void edge_path (size_t u, size_t v, const Op &op) const noexcept {
		assert(u < size() and v < size());
		std::vector<std::pair<size_t, size_t>> right;
		while (true) {
			if (head[u] == head[v]) {
				if (u == v) break;
				if (index[u] > index[v]) op(index[u], index[v] + 1);
				else op(index[u] + 1, index[v]);
				break;
			}
			if (index[u] > index[v]) {
				op(index[u], index[head[u]]);
				u = parent[head[u]];
			} else {
				right.emplace_back(index[head[v]], index[v]);
				v = parent[head[v]];
			}
		}
		for (size_t i = right.size(); i > 0; i--) {
			auto [l, r] = right[i - 1]; op(l, r);
		}
	}

	const size_t &operator[] (size_t v) const noexcept {
		assert(v < size());
		return index[v];
	}

	const size_t &operator() (size_t u, size_t v) const noexcept {
		assert(parent[u] == v or parent[v] == u);
		return (index[u] > index[v] ? index[u] : index[v]);
	}

};
