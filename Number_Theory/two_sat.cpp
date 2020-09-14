#include <vector>
#include <cstddef>

struct two_sat {
	using size_type = std::size_t;
private :
	strongly_connected_component scc;
	std::vector<bool> answer;

public :
	two_sat (size_type n) noexcept : scc(n << 1), answer(n) { }

	void add_clause (size_type a, bool f, size_type b, bool g) noexcept {
		scc.add_edge(a << 1 | not f, b << 1 | g);
		scc.add_edge(b << 1 | not g, a << 1 | f);
	}

	bool satisfiable () noexcept {
		scc.solve();
		for (size_type i = 0; i < answer.size(); i++) {
			if (scc[i << 1 | 0] == scc[i << 1 | 1]) return false;
			answer[i] = (scc[i << 1 | 0] < scc[i << 1 | 1]);
		}
		return true;
	}

	bool get (const size_type i) const noexcept {
		return answer[i];
	}

};