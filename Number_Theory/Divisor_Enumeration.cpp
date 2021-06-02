#include <vector>
#include <algorithm>

template<typename Tp>
std::vector<Tp> divisor(Tp n){
    std::vector<Tp> res;
    for (Tp i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i * i != n) { res.push_back(n / i); }
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}