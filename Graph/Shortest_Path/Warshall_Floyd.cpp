#include <vector>

template<typename T>
bool warshall_floyd (std::vector<std::vector<T>> &g, const T &inf) {
    const int n = (int)g.size();
    for (int i = 0; i < n; i++) g[i][i] = T(0);
    bool ret = false;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (g[i][k] == inf or g[k][j] == inf) continue;
                if (g[i][j] > g[i][k] + g[k][j]) g[i][j] = g[i][k] + g[k][j];
            }
        }
    }
    for (int i = 0; i < n; i++) ret |= (g[i][i] < 0);
    return ret;
}

template<class T>
using matrix = std::vector<std::vector<T>>;