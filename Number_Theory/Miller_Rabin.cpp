struct MillerRabin {
    using lint = __uint128_t;
private : 
    const lint v1[3] = {2, 7, 61};
    const lint v2[7] = {2, 3, 5, 7, 11, 13, 17};
    const lint v3[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
public : 
    inline lint modpow(lint x, lint k, lint m){
        lint a = 1, p = x;
        while(k > 0) {
            if (k % 2 == 0) { p = p * p % m; k /= 2; }
            else { a = a * p % m; k--; }
        }
        return a;
    }

    inline bool sub (lint a, lint n, lint d, lint s) {
        bool flag = true;
        for (lint r = 0; r < s; r++) {
            if (modpow(a, d, n) == n - 1) {
                flag = false; break;
            }
            d <<= 1LL;
        }
        return flag;
    }

    bool isprime (lint n) {
        if (n < 2) return false;
        lint d = n - 1, s = 0;
        while (d % 2 == 0) { d /= 2; s++; }
        if (n < 4759123141LL) {
            for (const lint &a : v1) {
                if (a == n) return true;
                if (n < a) break;
                if (modpow(a, d, n) == 1) continue;
                if (sub(a, n, d, s)) return false;
            }
        } else if (n < 341550071728321LL) {
            for (const lint &a : v2) {
                if (a == n) return true;
                if (n < a) break;
                if (modpow(a, d, n) == 1) continue;
                if (sub(a, n, d, s)) return false;
            }
        } else {
            for (const lint &a : v3) {
                if (a == n) return true;
                if (n < a) break;
                if (modpow(a, d, n) == 1) continue;
                if (sub(a, n, d, s)) return false;
            }
        }
        return true;
    }

};