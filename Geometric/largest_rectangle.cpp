#include <vector>
#include <stack>
#include <tuple>

template<class T>
T largest_rectangle (std::vector<T> a) {
	using value_type = T;
    a.emplace_back(value_type(0));
    int n = int(a.size());
	value_type ret = 0;
    std::stack<std::pair<value_type, int>> sta;
    for (int i = 0; i < n; i++) {
        if (sta.empty()) {
			sta.emplace(a[i], i);
        } else if (sta.top().first < a[i]) {
			sta.emplace(a[i], i);
        } else if (sta.top().first > a[i]) {
            int idx = i;
            while (!sta.empty()) {
                if (sta.top().first < a[i]) break;
                value_type h; int t;
				std::tie(h, t) = sta.top();
				sta.pop();
                value_type s = h * (i - t);
				idx = t;
                if (ret < s) ret = s;
            }
            sta.emplace(a[i], idx);
        }
    }
    return ret;
}