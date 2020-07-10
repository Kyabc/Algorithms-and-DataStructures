#include <vector>
#include <limits>
#include <algorithm>

template<class T>
struct ford_fulkerson {
    using value_type = T;
private : 
    const value_type INF = std::numeric_limits<value_type>::max();
    struct edge {
        int to, rev; value_type cap;
        edge (int to, value_type cap, int rev)
            : to(to), cap(cap), rev(rev) { }
    };
    std::vector<bool> seen;
    std::vector<std::vector<edge>> g;

    value_type find_flow (int v, int t, value_type f) {
        if (v == t) return f;
        seen[v] = true;
        for (edge &e : g[v]) {
            if (seen[e.to] or e.cap == 0) continue;
            const value_type ret = find_flow(e.to, t, std::min(f, e.cap));
            if (ret > 0) {
                e.cap -= ret;
                g[e.to][e.rev].cap += ret;
                return ret;
            }
        }
        return 0;
    }

public : 
    ford_fulkerson (int n) : g(n), seen(n) { }

    void add_edge (int from, int to, value_type cost) {
        g[from].emplace_back(to, cost, g[to].size());
        g[to].emplace_back(from, 0, g[from].size() - 1);
    }


    value_type max_flow (int s, int t) {
        value_type ret = 0;
        while (true) {
            std::fill(seen.begin(), seen.end(), false);
            value_type flow = find_flow(s, t, INF);
            if (flow == 0) break;
            ret += flow;
        }
        return ret;
    }

};