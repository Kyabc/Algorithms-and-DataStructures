#include <vector>

std::vector<int> maximum_independent_set (const std::vector<std::vector<bool>> &g) {
	auto f = [](int x) { return (1 << x); };
	const int n = int(g.size()), n1 = n / 2, n2 = n - n1;
	std::vector<bool> dp1(f(n1), true), dp3(f(n2), true);
	std::vector<int> dp2(f(n1), 0), dp4(f(n2), 0), dp5(f(n2), 0), ret;

	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < i; j++) {
			if (g[i][j]) dp1[f(i) | f(j)] = false;
		}
	}
	for (int s = 0; s < f(n1); s++) {
		if (dp1[s] == false) {
			for (int v = 0; v < n1; v++) {
				dp1[s | f(v)] = false;
			}
		}
	}

	dp2[0] = f(n2) - 1;
	for (int v = 0; v < n1; v++) {
		for (int u = 0; u < n2; u++) {
			if (not g[v][u + n1]) {
				dp2[f(v)] = (dp2[f(v)] | f(u));
			}
		}
	}
	for (int s = 0; s < f(n1); s++) {
		for (int v = 0; v < n1; v++) {
			if (not (s & f(v))) {
				dp2[s | f(v)] = (dp2[s] & dp2[f(v)]);
			}
		}
	}

	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < i; j++) {
			if (g[i + n1][j + n1]) dp3[f(i) | f(j)] = false;
		}
	}
	for (int s = 0; s < f(n2); s++) {
		if (dp3[s] == false) {
			for (int v = 0; v < n2; v++) {
				dp3[s | f(v)] = false;
			}
		}
	}

	for (int s = 0; s < f(n2); s++) {
		if (dp3[s]) {
			dp4[s] = __builtin_popcount(s);
			dp5[s] = s;
		}
	}
	for (int s = 0; s < f(n2); s++) {
		for (int v = 0; v < n2; v++) {
			if (dp4[s | f(v)] < dp4[s]) {
				dp4[s | f(v)] = dp4[s];
				dp5[s | f(v)] = dp5[s];
			}
		}
	}

	for (int s1 = 0; s1 < f(n1); s1++) {
		if (not dp1[s1]) continue;
		int s = dp2[s1], s2 = dp5[s];
		int sz1 = __builtin_popcount(s1), sz2 = dp4[s];
		if (ret.size() < sz1 + sz2) {
			ret.clear();
			for (int v = 0; v < n1; v++) {
				if (s1 & f(v)) ret.push_back(v);
			}
			for (int v = 0; v < n2; v++) {
				if (s2 & f(v)) ret.push_back(v + n1);
			}
		}
	}

	return ret;

}