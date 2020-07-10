#include <vector>

template<typename T>
bool warshall_floyd (std::vector<std::vector<T>> &g) {
    const int n = (int)g.size();
    bool ret = false;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (g[i][j] > g[i][k] + g[k][j]) g[i][j] = g[i][k] + g[k][j];
            }
        }
    }
    for (int i = 0; i < n; i++) ret |= (g[i][i] < 0);
    return ret;
}