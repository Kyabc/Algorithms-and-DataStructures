#include <vector>

template<typename T>
struct edge {
    int to; T cost;
    Edge (int to, T cost = 1) : to(to), cost(cost) { }
    bool operator< (const Edge &r) const { return (cost < r.cost); }
};

template<typename T>
using graph = std::vector<std::vector<edge<T>>>;