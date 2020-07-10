#include <vector>
#include <queue>
#include <functional>
#include <tuple>

template <typename T>
std::vector<T> dijkstra(const graph<T> &g, int s){
    using P = std::pair<T, int>;
    std::vector<T> ret(g.size(), -1);
    std::priority_queue<P, std::vector<P>, std::greater<P>> que;
    que.emplace(ret[s], s); ret[s] = 0;
    while (not que.empty()) {
        int v; T c;
        std::tie(c, v) = que.top();
        que.pop();
        if (ret[v] < c) continue;
        for (const auto &e : g[v]) {
            if (ret[e.to] > ret[v] + e.cost or ret[e.to] == -1) {
                ret[e.to] = ret[v] + e.cost;
                que.emplace(ret[e.to], e.to);
            }
        }
    }
    return ret;
}