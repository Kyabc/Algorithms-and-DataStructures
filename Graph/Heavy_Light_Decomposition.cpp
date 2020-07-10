#include <vector>
#include <stack>
#include <queue>
#include <functional>

struct heavy_light_decomposition {
	using query = std::function<void(int, int)>;
private :
    std::vector<std::vector<int>> g;
    std::vector<int> idx, xdi, par, hvy, head, sz, dep;

    void dfs (int r) {
        std::stack<int> sta; sta.push(r);
        dep[r] = 0; par[r] = -1;
        while (not sta.empty()) {
            int v = sta.top();
            if (not sz[v]) {
                sz[v] = 1;
                for (int u : g[v]) {
                    if (u == par[v]) continue;
                    dep[u] = dep[v] + 1; par[u] = v;
                    sta.push(u);
                }
            } else {
                sta.pop();
                int max_size = 0;
                for (int u : g[v]) if (u != par[v]) {
                    sz[v] += sz[u];
                    if (max_size < sz[u]) {
                        max_size = sz[u];
                        hvy[v] = u;
                    }
                }
            }
        }
    }

    void hld (int r, int &index) {
        std::queue<int> que; que.push(r);
        while (not que.empty()) {
            int h = que.front(); que.pop();
            for (int v = h; v != -1; v = hvy[v]) {
                head[v] = h;
                idx[v] = index++;
                xdi[idx[v]] = v;
                for (int u : g[v]) {
                    if (u != par[v] and u != hvy[v]) {
                        que.push(u);
                    }
                }
            }
        }
    }

public : 

    heavy_light_decomposition (int n) : 
        g(n), idx(n, -1), xdi(n), par(n), hvy(n, -1), head(n), sz(n, 0), dep(n) { }

    void add_edge (int u, int v) noexcept {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build () {
        const int n = g.size(); int i = 0;
        for (int v = 0; v < n; v++) if (not sz[v]) dfs(v);
        for (int v = 0; v < n; v++) if (idx[v] == -1) hld(v, i);
    }

    void build (std::vector<int> root) {
        int i = 0;
        for (int r : root) { dfs(r); hld(r, i); }
    }

    void each_vertex (int u, int v, const query &f) {
        while (true) {
            if (idx[v] < idx[u]) std::swap(u, v);
            if (head[u] == head[v]) {
                f(idx[u], idx[v]);
                break;
            } else {
                f(idx[head[v]], idx[v]);
                v = par[head[v]];
            }
        }
    }

    void each_edge (int u, int v, const query &f) {
        while (true) {
            if (idx[v] < idx[u]) std::swap(u, v);
            if (head[u] == head[v]) {
                if (u != v) f(idx[u] + 1, idx[v]);
                break;
            } else {
                f(idx[head[v]], idx[v]);
                v = par[head[v]];
            }
        }
    }

    int lca (int u, int v) {
        while (true) {
            if (idx[v] < idx[u]) std::swap(u, v);
            if (head[u] != head[v]) v = par[head[v]];
            else return (idx[u] < idx[v] ? u : v);
        }
    }

    int distance (int u, int v) {
        return (dep[u] + dep[v] - 2 * dep[lca(u, v)]);
    }

    const int operator[] (int v) const noexcept {
        return (idx[v]);
    }

};