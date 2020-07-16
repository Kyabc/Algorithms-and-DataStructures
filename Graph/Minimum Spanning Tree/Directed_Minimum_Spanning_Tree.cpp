// skew heap must be included

#include <vector>
#include <numeric>
#include <algorithm>

template<class T>
struct directed_minimum_spanning_tree {
	using value_type = T;
private :
	struct edge {
	public : 
		int from, to;
		value_type cost;
		edge (int from, int to, const value_type &cost) :
			from(from), to(to), cost(cost) { }
	};

	std::vector<edge> edges;
	std::vector<int> uf, par;

	bool unite (int x, int y) {
		x = root(x); y = root(y);
		if (x == y) return false;
		uf[x] = y; return true;
	}

	int root (int x) {
		return (uf[x] == x ? x : uf[x] = root(uf[x]));
	}

public :

	directed_minimum_spanning_tree (int n) : par(n), uf(n) { }

	void add_edge (int from, int to, const value_type &cost) {
		edges.emplace_back(from, to, cost);
	}

	value_type solve (int r = 0) {
		using data = std::pair<value_type, int>;
		using heap = skew_heap<data, value_type, std::greater<data>>;
		const int n = int(par.size());
		value_type sum_cost = 0;
		std::vector<heap> heaps;
		std::vector<int> order, status(n, 0), incoming(n, -1), prev_edge(edges.size());
		auto f1 = [](const data &a, const value_type &b) { return (data(a.first - b, a.second)); };
		auto f2 = [](const value_type &a, const value_type &b) { return (a + b); };
		for (int i = 0; i < n; i++) heaps.push_back(heap(f1, f2, 0));
		for (int i = 0; i < int(edges.size()); i++) heaps[edges[i].to].emplace(edges[i].cost, i);

		status[r] = 2; par[r] = r;
		std::iota(uf.begin(), uf.end(), 0);

		for (int s = 0; s < n; s++) if (status[s] == 0) {
			std::vector<int> seen, processed;
			
			for (int u = s, v, cycle = 0; status[u] != 2; u = root(v)) {
				status[u] = 1;
				processed.push_back(u);
				
				if (heaps[u].empty()) return -1;
				auto [cost, i] = heaps[u].top();
				heaps[u].pop();
				const edge &e = edges[i];
				sum_cost += cost;
				heaps[u].add(cost);
				order.push_back(i);
				
				if (incoming[e.to] == -1) {
					incoming[e.to] = i;
				}
				
				while (cycle) {
					prev_edge[seen.back()] = i;
					seen.pop_back();
					cycle--;
				}
				
				seen.push_back(i);
				
				if (status[root(v = e.from)] == 1) {
					int w;
					heap buff(f1, f2, 0);
					do {
						w = processed.back();
						processed.pop_back();
						status[w] = 2;
						buff.meld(heaps[w]);
						cycle++;
					} while (unite(w, v));
					heaps[root(v)].meld(buff);
					status[root(v)] = 1;
				}
			}
			
			for (int i : processed) status[i] = 2;
		}

		std::reverse(order.begin(), order.end());
		std::vector<bool> used_edge(edges.size(), false);
		for (int i : order) if (not used_edge[i]) {
			const edge &e = edges[i];
			par[e.to] = e.from;
			for (int j = incoming[e.to]; j != i; j = prev_edge[j]) {
				used_edge[j] = true;
			}
		}
		
		return sum_cost;
	}

	const int& operator[] (int i) const {
		return (par[i]);
	}

};