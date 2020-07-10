#include <vector>
#include <limits>


template<typename T>
std::vector<T> bellmanford (const graph<T> &g, int s) {
    const T inf = std::numeric_limits<T>::max();
    int n = int(g.size()), loop = n;
    std::vector<T> dist(n, inf); dist[s] = 0;
    while (loop--) for (int v = 0; v < n; v++) {
        for (const auto &e : g[v]) {
            if (dist[v] == inf) continue;
            if (dist[e.to] > dist[v] + e.cost) {
                if (not loop) return std::vector<T>();
                dist[e.to] = dist[v] + e.cost;
            }
        }
    }
    return dist;
}