#include <vector>

template<int mod>
struct permutation {
private : 
    std::vector<modint<mod>> fact, ifact;
public :
    permutation(int n) : fact(n + 1), ifact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i < n + 1; i++) fact[i] = (fact[i - 1] * i);
        ifact[n] = fact[n].inverse();
        for (int i = n; i > 0; i--) ifact[i - 1] = ifact[i] * i;
    }

    modint<mod> operator() (int n, int r) {
        if (r < 0 or r > n) return 0;
        return fact[n] * ifact[n - r];
    }

    constexpr const modint<mod> &operator[] (int i) const {
        return fact[i];
    }
};