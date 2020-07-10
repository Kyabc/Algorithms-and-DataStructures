long long euler_phi (long long n) {
	long long ret = n;
	for (long long i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			ret -= (ret / i);
			while (n % i == 0) {
				n /= i;
			}
		}
	}
	if (n > 1) ret -= (ret / n);
	return ret;
}