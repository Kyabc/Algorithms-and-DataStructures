//
//  Ford Fulkerson
//
//  cf.
//    蟻本 p.188-195
//    HCPC: 北海道大学競技プログラミングサークル : 最大流(max flow)
//      https://www.slideshare.net/hcpc_hokudai/max-flow
//
//  verify
//    AIZU ONLINE JUDGE - Maximum Flow
//      http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4432410#1
//

/*
    フォード・ファルカーソンのアルゴリズム
    
    有向グラフ G = (V, E) に対する頂点 s から頂点 t への最大流を求める
    add_edge (v, u, c) : 重み c の辺 (v, u) を追加する O(1)
    find_flow (s, t, f) : dfs でフローを流す
    max_flow (s, t) : 頂点 s から頂点 t への最大流を求める
*/


template<class T>
struct Ford_Fulkerson {
private : 
    const T INF = numeric_limits<T>::max();
    struct Edge {
        int to, rev; T cap;
        Edge (int to, T cap, int rev)
            : to(to), cap(cap), rev(rev) { }
    };
    vector<bool> seen;
    vector<vector<Edge>> g;
public : 
    Ford_Fulkerson (int n) : g(n), seen(n) { }

    void add_edge (int from, int to, T cost) {
        g[from].emplace_back(to, cost, g[to].size());
        g[to].emplace_back(from, 0, g[from].size() - 1);
    }

    T find_flow (int v, int t, T f) {
        if (v == t) return f;
        seen[v] = true;
        for (Edge &e : g[v]) {
            if (seen[e.to] or e.cap == 0) continue;
            const T ret = find_flow(e.to, t, min(f, e.cap));
            if (ret > 0) {
                e.cap -= ret;
                g[e.to][e.rev].cap += ret;
                return ret;
            }
        }
        return 0;
    }

    T max_flow (int s, int t) {
        T ret = 0;
        while (true) {
            fill(seen.begin(), seen.end(), false);
            T flow = find_flow(s, t, INF);
            if (flow == 0) break;
            ret += flow;
        }
        return ret;
    }

};

int main() {
    int n, m;
    cin >> n >> m;
    Ford_Fulkerson<int> g(n);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }
    
    cout << g.max_flow(0, n - 1) << '\n';
    
    return 0;
}