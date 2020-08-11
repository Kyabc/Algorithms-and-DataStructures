#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>

template<std::uint_fast64_t B = 524287>
struct rolling_hash {
	using u32 = std::uint_fast32_t;
	using u64 = std::uint_fast64_t;
	using i128 = __int128_t;
	using size_type = std::size_t;

private :
	static constexpr u64 base = B;
	static constexpr u64 mod = (1UL << 61) - 1;

	std::vector<u64> hash, pow;

	u64 mul (const i128 &a, const i128 &b) const noexcept {
		i128 value = a * b;
		value = (value >> 61) + (value & mod);
		return (value < mod ? value : value - mod);
	}

	u32 xorshift (u32 value) const noexcept {
		value ^= value << 13;
		value ^= value >> 17;
		value ^= value << 5;
		return value;
	}

public :
	constexpr rolling_hash () = default;

	template<class InputIterator>
	constexpr rolling_hash (InputIterator first, InputIterator last) noexcept {
		build(first, last);
	}

	template<class InputIterator>
	void build (InputIterator first, InputIterator last) noexcept {
		const size_type n = (last - first);
		hash.assign(n + 1, 0);
		pow.assign(n + 1, 1);
		for (size_type i = 0; first != last; first++, i++) {
			hash[i + 1] = mul(hash[i], base) + xorshift((*first) + 1);
			pow[i + 1] = mul(pow[i], base);
		}
	}

	u64 get(size_type first, size_type last) const noexcept {
		if (first > last) std::swap(first, last);
		u64 value = hash[last] + mod - mul(hash[first], pow[last - first]);
		return (value < mod ? value : value - mod);
	}

};