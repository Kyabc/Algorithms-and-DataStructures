struct factorization {
private :
	std::vector<int> table;
 
public :
	factorization(std::size_t n) : table(n + 2, 0) {
		for (int i = 2; i <= n; i++) if (table[i] == 0) {
			for (int j = i; j <= n; j += i) {
				if (table[j] == 0) table[j] = i;
			}
		}
	}
 
	std::map<int, int> operator() (int x) {
		std::map<int, int> fact;
		while (x > 1) {
			const int &factor = table[x];
			fact[factor]++; x /= factor;
		}
		return fact;
	}
 
};