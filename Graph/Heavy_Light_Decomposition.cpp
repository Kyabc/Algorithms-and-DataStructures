#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <cassert>

struct heavy_light_decomposition {
	using size_type = size_t;
private :
	size_type count;
	std::vector<std::vector<size_type>> g;
	std::vector<size_type> index, parent, head;

	void dfs (size_type r, std::vector<size_type> &heavy) noexcept {
		std::vector<size_type> subtree(size(), size());
		std::stack<size_type> sta({r});
		while (not sta.empty()) {
			const size_type v = sta.top();
			if (subtree[v] == size()) {
				subtree[v] = 1;
				for (const auto &u : g[v]) {
					if (u == parent[v]) continue;
					parent[u] = v; sta.push(u);
				}
			} else {
				sta.pop();
				size_type max_size = 0;
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

	void decompose (size_type r, size_type &idx, const std::vector<size_type> &heavy) noexcept {
		std::queue<size_type> que({r});
		while (not que.empty()) {
			const size_type h = que.front(); que.pop();
			for (size_type v = h; v != size(); v = heavy[v]) {
				head[v] = h; index[v] = idx++;
				for (const auto &u : g[v]) {
					if (u == parent[v] or u == heavy[v]) continue;
					que.push(u);
				}
			}
		}
	}

public :
	heavy_light_decomposition (size_type n)
	noexcept : count(0), g(n), index(n, n), parent(n, n), head(n, n) { }

	size_type size () const noexcept {
		return g.size();
	}

	void add_edge (size_type v, size_type u) noexcept {
		assert(v < size() and u < size() and ++count < size());
		g[v].push_back(u); g[u].push_back(v);
	}

	void build () noexcept {
		size_type idx = 0;
		std::vector<size_type> heavy(size(), size());
		for (size_type r = 0; r < size(); r++) {
			if (parent[r] == size()) dfs(r, heavy);
		}
		for (size_type r = 0; r < size(); r++) {
			if (index[r] == size()) decompose(r, idx, heavy);
		}
	}

	void build (const std::vector<size_type> &roots) noexcept {
		size_type idx = 0;
		std::vector<size_type> heavy(size(), size());
		for (const auto &r : roots) {
			if (parent[r] == size()) dfs(r, heavy);
		}
		for (const auto &r : roots) {
			if (index[r] == size()) decompose(r, idx, heavy);
		}
	}

	size_type lca (size_type u, size_type v) const noexcept {
		assert(u < size() and v < size());
		for (; head[u] != head[v]; u = parent[head[u]]) {
			if (index[u] < index[v]) std::swap(u, v);
		}
		return (index[u] < index[v] ? u : v);
	}

	// [l, r]
	template<class Op>
	void vertex_path (size_type u, size_type v, const Op &op) const noexcept {
		assert(u < size() and v < size());
		std::vector<std::pair<size_type, size_type>> right;
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
		for (size_type i = right.size(); i > 0; i--) {
			auto [l, r] = right[i - 1]; op(l, r);
		}
	}

	template<class Op>
	void edge_path (size_type u, size_type v, const Op &op) const noexcept {
		assert(u < size() and v < size());
		std::vector<std::pair<size_type, size_type>> right;
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
		for (size_type i = right.size(); i > 0; i--) {
			auto [l, r] = right[i - 1]; op(l, r);
		}
	}

	const size_type &operator[] (size_type v) const noexcept {
		assert(v < size());
		return index[v];
	}

	const size_type &operator() (size_type u, size_type v) const noexcept {
		assert(parent[u] == v or parent[v] == u);
		return (index[u] > index[v] ? index[u] : index[v]);
	}

};