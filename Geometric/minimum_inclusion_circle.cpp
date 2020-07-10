#include <complex>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>

std::pair<std::complex<double>, double> min_circle (std::vector<std::complex<double>> points, int seed = 2020) {
    using point = std::complex<double>;
    using circle = std::pair<point, double>;
    const double eps = 1e-10; int n = int(points.size());
    if (n == 1) { return {points[0], 0}; };
    auto inner = [&] (const point &a, const circle &c) -> bool { 
        return std::norm(a - c.first) <= c.second + eps;
    };
    auto make_circle_2 = [&](const point &a, const point &b) -> circle {
        point c = (a + b) / 2.0; double r = std::norm(a - c);
        return {c, r};
    };
    auto make_circle_3 = [&](const point &a, const point &b, const point &c) -> circle {
        double A = std::norm(b - c), B = std::norm(c - a), C = std::norm(a - b);
        double S = std::norm((a - c) * std::conj(b - c) - std::conj(a - c) * (b - c));
        point p = (A * (B + C - A) * a + B * (C + A - B) * b + C * (A + B - C) * c) / S;
        double r = std::norm(p - a);
        return {p, r};
    };
    
    // MinDisk
    std::shuffle (points.begin(), points.end(), std::mt19937(seed));
    circle d = make_circle_2(points[0], points[1]);
    for (int i = 2; i < n; i++) {
        if (not inner(points[i], d)) {
            // MinDiskWithPoints
            d = make_circle_2(points[0], points[i]);
            for (int j = 1; j < i; j++) {
                if (not inner(points[j], d)) {
                    // MinDiskWith2Points
                    d = make_circle_2(points[i], points[j]);
                    for (int k = 0; k < j; k++) {
                        if (not inner(points[k], d)) {
                            d = make_circle_3(points[k], points[j], points[i]);
                        }
                    }
                }
            }
        }
    }
    return d;
}