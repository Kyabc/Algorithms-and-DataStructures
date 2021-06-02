#include <iostream>
#include <vector>
#include <cstddef>
#include <limits>

// edge class
template<class Tp>
class edge {
public :
	using const_reference = const Tp&;
	using reference = Tp&;
	using value_type = Tp;
	using size_type = std::size_t;
	size_type to;
	value_type cost;
	constexpr edge () = default;
	explicit edge (size_type to, const_reference cost = 1) noexcept : to(to), cost(cost) { }
	constexpr bool operator== (const edge &rhs) const noexcept { return (cost == rhs.cost); }
	constexpr bool operator!= (const edge &rhs) const noexcept { return (not operator==(rhs)); }
	constexpr bool operator< (const edge &rhs) const noexcept { return (cost < rhs.cost); }
	constexpr bool operator> (const edge &rhs) const noexcept { return (cost > rhs.cost); }
	constexpr bool operator<= (const edge &rhs) const noexcept { return (not operator>(rhs)); }
	constexpr bool operator>= (const edge &rhs) const noexcept { return (not operator<(rhs)); }
};


// graph class
template<class Tp, class EdgeType = edge<Tp>>
class graph : private std::vector<std::vector<EdgeType>> {
public :
	using edge_type = EdgeType;
	using const_reference = typename edge_type::const_reference;
	using reference = typename edge_type::reference;
	using value_type = typename edge_type::value_type;
	using size_type = typename edge_type::value_type;
	using Self = std::vector<std::vector<edge_type>>;
	using Self::vector;
	using Self::operator=;
	using Self::begin;
	using Self::end;
	using Self::rbegin;
	using Self::rend;
	using Self::size;
	using Self::resize;
	using Self::empty;
	using Self::reserve;
	using Self::operator[];
	using Self::at;
	using Self::assign;
	using Self::push_back;
	using Self::emplace_back;
	using Self::pop_back;
	using Self::swap;
	using Self::clear;

	void add_directed_edge (size_type src, size_type dst, const_reference cost) noexcept {
		(*this)[src].emplace_back(dst, cost);
	}

	void add_edge (size_type u, size_type v, const_reference cost) noexcept {
		add_directed_edge(u, v, cost);
		add_directed_edge(v, u, cost);
	}

	void read_graph (size_type m, bool weighted = false, bool directed = false, size_type index_type = 1) noexcept {
		for (size_type i = 0; i < m; i++) {
			size_type u, v;
			std::cin >> u >> v;
			u -= index_type;
			v -= index_type;
			value_type cost = value_type(1);
			if (weighted) std::cin >> cost;
			if (directed) add_directed_edge(u, v, cost);
			else add_edge(u, v, cost);
		}
	}

};