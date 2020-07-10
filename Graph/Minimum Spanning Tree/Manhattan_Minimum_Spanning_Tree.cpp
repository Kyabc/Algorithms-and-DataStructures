#include <vector>
#include <tuple>
#include <numeric>

// dependence on segment tree

template<class T>
struct manhattan_minimum_spanning_tree {
	using value_type = T;
	using point = std::pair<value_type, value_type>;
private :
	std::vector<int> uf;
	std::vector<point> points;
	std::vector<std::pair<value_type, std::pair<int, int>>> dat;

	int root (int x) {
		if (uf[x] < 0) return x;
		return uf[x] = root(uf[x]);
	}

	bool unite (int x, int y) {
		x = root(x); y = root(y);
		if (x == y) return false;
		if (uf[x] > uf[y]) std::swap(x, y);
		uf[x] += uf[y]; uf[y] = x;
		return true;
	}

	value_type minimum_spanning_tree () {
		value_type mst = 0;
		uf.assign(points.size(), -1);
		sort(dat.begin(), dat.end());
		
		for (const auto [cst, e] : dat) {
			if (unite(e.first, e.second)) {
				edges.push_back(e);
				mst += cst;
			}
		}
		
		return mst;
	}

	void plane_sweep () {
		const int n = int(points.size());
		std::vector<int> idx(n), yidx(n);
		
		std::iota(idx.begin(), idx.end(), 0);
		std::sort(idx.begin(), idx.end(), [&] (const int &lhs, const int &rhs) {
			const point &l = points[lhs], &r = points[rhs];
			const value_type vl = l.first - l.second, vr = r.first - r.second;
			if (vl == vr) return (l.first == r.first ? (lhs > rhs) : (l.first > r.first));
			return (vl < vr);
		} );
		
		std::vector<std::pair<point, int>> y(n);
		for (int i = 0; i < n; i++) y[i] = std::make_pair(points[i], i);
		std::sort(y.begin(), y.end());
		for (int i = 0; i < n; i++) yidx[y[i].second] = i;
		
		using node = std::pair<value_type, int>;
		auto rmq = [](const node &l, const node &r) { return std::min(l, r); };
		const node identity(std::numeric_limits<value_type>::max(), -1);
		segment_tree<node> seg(rmq, identity);
		seg.assign(n);

		for (int k = 0; k < n; k++) {
			const int i = idx[k];
			const point &p = points[i];
			auto [cst, j] = seg.fold(yidx[i], n);
			if (j != -1) {
				const point &q = points[j];
				value_type cst = std::abs(p.first - q.first) + std::abs(p.second - q.second);
				dat.emplace_back(cst, std::make_pair(i, j));
			}
			seg.set_val(yidx[i], node(p.first + p.second, i));
		}
	}

public :
	std::vector<std::pair<int, int>> edges;

	manhattan_minimum_spanning_tree () { }

	void add_point (value_type x, value_type y) noexcept {
		points.emplace_back(y, x);
	}

	value_type solve () {
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				for (int xy = 0; xy < 2; xy++) {
					plane_sweep();
					for (point &p : points) std::swap(p.first, p.second);
				}
				for (point &p : points) p.first = -p.first;
			}
			for (point &p : points) p.second = -p.second;
		}
		return minimum_spanning_tree();
	}

};