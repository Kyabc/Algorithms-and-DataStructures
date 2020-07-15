#include <vector>
#include <cassert>

template<class T>
struct matrix {
    using value_type = T;
    using size_type = std::size_t;
private :
    size_type h, w;
    std::vector<std::vector<value_type>> mat;

public : 
    matrix () { }

    matrix (size_type n) : 
        h(n), w(n), mat(n, std::vector<value_type>(n, value_type())) { }

    matrix (size_type n, const value_type &x) : 
        h(n), w(n), mat(n, std::vector<value_type>(n, x)) { }

    matrix (size_type h, size_type w) : 
        h(h), w(w), mat(h, std::vector<value_type>(w, value_type())) { }

    matrix (size_type h, size_type w, const value_type &x) :
        h(h), w(w), mat(h, std::vector<value_type>(w, x)) { }

    const std::vector<value_type> &operator[] (size_type i) const {
        return mat[i];
    }

    std::vector<value_type> &operator[] (size_type i) {
        return mat[i];
    }

    matrix &operator+= (const matrix &rhs) {
        assert(h == rhs.h and w == rhs.w);
        for (size_type i = 0; i < h; i++) {
            for (size_type j = 0; j < w; j++) {
                (*this)[i][j] += rhs[i][j];
            }
        }
        return (*this);
    }

    matrix &operator-= (const matrix &rhs) {
        assert(h == rhs.h and w == rhs.w);
        for (size_type i = 0; i < h; i++) {
            for (size_type j = 0; j < w; j++) {
                (*this)[i][j] -= rhs[i][j];
            }
        }
        return (*this);
    }

    matrix &operator*= (const matrix &rhs) {
        assert(h == rhs.h);
        std::vector<std::vector<value_type>> buff(h, std::vector<value_type>(rhs.w));
        for (size_type i = 0; i < h; i++) {
            for (size_type k = 0; k < rhs.h; k++) {
                for (size_type j = 0; j < rhs.w; j++) {
                    buff[i][j] += (mat[i][k] * rhs[k][j]);
                }
            }
        }
        mat.swap(buff);
        return (*this);
    }

    matrix &operator^= (long long x) {
        assert(h == w);
        matrix<value_type> buff(h);
        for (size_type i = 0; i < h; i++) {
            buff[i][i] = value_type(1);
        }
        while (x) {
            if (x & 1) buff *= (*this);
            (*this) *= (*this);
            x >>= 1;
        }
        mat.swap(buff.mat);
        return (*this);
    }

    matrix &operator+ (const matrix &rhs) {
        return ((*this) += rhs);
    }

    matrix &operator- (const matrix &rhs) {
        return ((*this) -= rhs);
    }

    matrix &operator* (const matrix &rhs) {
        return ((*this) *= rhs);
    }

    matrix &operator^ (long long x) {
        return ((*this) ^= x);
    }

};