#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>

template<class Tp = char, std::uint_fast64_t B = 524287>
struct rolling_hash {
	using value_type = Tp;
	using u32 = std::uint_fast32_t;
	using u64 = std::uint_fast64_t;
	using i128 = __int128_t;
	using size_type = std::size_t;

private :
	static constexpr u64 base = B;
	static constexpr u64 mod = (1UL << 61) - 1;

	std::vector<u64> hash_table, pow_base;

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
	constexpr rolling_hash () : hash_table(1, 0), pow_base(1, 1) { }

	template<class InputIterator>
	constexpr rolling_hash (InputIterator first, InputIterator last) noexcept {
		build(first, last);
	}

	template<class InputIterator>
	void build (InputIterator first, InputIterator last) noexcept {
		const size_type n = std::distance(first, last);
		hash_table.assign(1, 0); hash_table.reserve(n + 1);
		pow_base.assign(1, 1); pow_base.reserve(n + 1);
		while (first != last) push_back(*first++);
	}

	size_type size () const noexcept {
		return hash_table.size() - 1;
	}

	void push_back (value_type c) noexcept {
		hash_table.push_back(mul(hash_table.back(), base) + xorshift(c + 1));
		pow_base.push_back(mul(pow_base.back(), base));
	}

	void pop_back() noexcept {
		assert(size() > 0);
		hash_table.pop_back();
		pow_base.pop_back();
	}

	u64 get(size_type first, size_type last) const noexcept {
		if (first > last) std::swap(first, last);
		const u64 value = hash_table[last] + mod - mul(hash_table[first], pow_base[last - first]);
		return (value < mod ? value : value - mod);
	}

	u64 all () const noexcept {
		return get(0, size());
	}

};