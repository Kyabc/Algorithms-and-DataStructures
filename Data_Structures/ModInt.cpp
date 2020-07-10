#include <iostream>

template<int mod>
struct modint {
    int x;
    modint() : x(0) {}
    modint(long long y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}
    modint &operator+=(const modint &p) { if((x += p.x) >= mod) x -= mod; return *this; }
    modint &operator-=(const modint &p) { if((x += mod-p.x) >= mod) x -= mod; return *this; }
    modint &operator*=(const modint &p) { x = (int)(1LL*x*p.x%mod); return *this; }
    modint &operator/=(const modint &p) { *this *= p.inverse(); return *this; }
    modint operator-() const { return modint(-x); }
    modint operator+(const modint &p) const { return modint(*this) += p; }
    modint operator-(const modint &p) const { return modint(*this) -= p; }
    modint operator*(const modint &p) const { return modint(*this) *= p; }
    modint operator/(const modint &p) const { return modint(*this) /= p; }
    bool operator==(const modint &p) const { return x == p.x; }
    bool operator!=(const modint &p) const { return x != p.x; }
    modint inverse() const {
        int a = x, b = mod, u = 1, v = 0, t;
        while(b > 0) {
			t = a / b;
			a -= t * b;
			std::swap(a, b);
			u -= t * v;
			std::swap(u, v);
		}
        return modint(u);
    }
    modint pow(long long e){
        long long a = 1, p = x;
        while(e > 0) {
            if (e & 1) {a = (a * p) % mod; e--;}
            else {p = (p * p) % mod; e /= 2;}
        }
        return modint(a);
    }
    friend std::ostream &operator<<(std::ostream &os, const modint<mod> &p) {
        return os << p.x;
    }
    friend std::istream &operator>>(std::istream &is, modint<mod> &a) {
        long long x;
        is >> x;
        a = modint<mod>(x);
        return (is);
    }
};