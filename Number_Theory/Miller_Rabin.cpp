#include <array>
#include <cstdint>

struct miller_rabin {
	using u128 = __uint128_t;
	using u64 = std::uint_fast64_t;
	using u32 = std::uint_fast32_t;
private :
	inline constexpr static std::array<u64, 7> a0 = {2, 3, 5, 7, 11, 13, 17};
	inline constexpr static std::array<u64, 7> a1 = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	inline constexpr static u64 min_value = 341'550'071'728'321;

	void mul (u64 &x1, u64 x2, const u64 &mod) {
		u128 buff = x1;
		buff = (buff * x2) % mod;
		x1 = (u64)(buff);
	}

	u64 modpow (u64 x, u64 exp, const u64 &mod) {
		u64 cur = 1;
		while (exp) {
			if (exp & 1) mul(cur, x, mod);
			mul(x, x, mod); exp >>= 1;
		}
		return cur;
	}

	bool test (const u64 &a, const u64 &n, const u64 &d, const u32 &s) {
		u64 cur = modpow(a, d, n);
		if (cur == 1) return false;
		for (u32 r = 0; r < s; r++) {
			if (cur == n - 1) return false;
			mul(cur, cur, n);
		}
		return true;
	}

public :
	constexpr miller_rabin () = default;

	template<class T>
	bool operator() (const T &n) {
		if (n < 2) return false;
		if (n < 4) return true;
		if (not (n & 1)) return false;
		if (n < min_value) {
			u64 d = n - 1; u32 s = 0;
			while (not (d & 1)) { d >>= 1; s++; }
			for (const u64 &a : a0) {
				if (n <= a) return true;
				if (test(a, n, d, s)) return false;
			}
		} else {
			u64 d = n - 1; u32 s = 0;
			while (not (d & 1)) { d >>= 1; s++; }
			for (const u64 &a : a1) {
				if (n <= a) return true;
				if (test(a, n, d, s)) return false;
			}
		}
		return true;
	}

};