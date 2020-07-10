#include <vector>

std::vector<bool> eratosthenes_sieve(int n) {
    std::vector<bool> v(n + 1, true);
    if (n >= 0) { v[0] = false; }
    if (n >= 1) { v[1] = false; }
    for (int i = 2; i * i <= n; i++) if (v[i]) {
        for (int j = i + i; j <= n; j += i) v[j] = false;
    }
    return v;
}