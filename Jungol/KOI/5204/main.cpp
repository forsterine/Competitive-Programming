#include "other/template.hpp"
#include "graph/Graph.hpp"
using namespace std;
struct Query {
    int u, v, id; 
}; 
int main() {
    int n; 
    scan(n); 
    int m = n; 
    Graph<ll> g(2 * n); 
    vector<vector<int>> tr(2 * n); 
    vector<int> cur(n), used(n); 
    iota(all(cur), 0); 
    auto add = [&](int u, int v, ll w, bool tree) {
        g.add_edge(u, v, w); 
        if (tree) {
            tr[u].push_back(v); 
            tr[v].push_back(u); 
        }
    }; 
    rep(i, 1, n) {
        int p; 
        ll w; 
        scan(p, w); 
        --p; 
        if (used[p]) {
            add(cur[p], m, 0, true); 
            cur[p] = m++; 
            used[p] = true; 
        }
        add(cur[p], i, w, true); 
        used[p] = true; 
    }
    vector<int> leaves; 
    rep(i, n) {
        if (g[i].size() == 1) leaves.push_back(i); 
    }
    rep(i, (int)leaves.size()) {
        ll w; 
        scan(w);
        add(leaves[i], leaves[(i + 1) % leaves.size()], w, false); 
    }
    int q; 
    scan(q); 
    vector<Query> queries(q); 
    rep(i, q) {
        scan(queries[i].u, queries[i].v); 
        --queries[i].u; 
        --queries[i].v; 
        queries[i].id = i; 
    }
    const ll INF = (1LL << 60); 
    vector<ll> ans(q, INF), dist(m, INF); 
    vector<int> par(m), sz(m), label(m); 
    vector<bool> removed(m); 
    struct Task {
        int rt; 
        vector<Query> qs; 
    }; 
    queue<Task> tasks; 
    tasks.push({0, move(queries)}); 
    while (!tasks.empty()) {
        auto [rt, qs] = move(tasks.front()); 
        tasks.pop(); 
        if (qs.empty()) continue; 
        vector<int> comp{rt}; 
        par[rt] = -1; 
        for (int i= 0; i < (int)comp.size(); i++) {
            int v = comp[i]; 
            sz[v] = 1; 
            for (int u : tr[v]) {
                if (removed[u] || u == par[v]) continue; 
                par[u] = v; 
                comp.push_back(u); 
            }
        }
        for (int i= (int)comp.size() - 1; i > 0; i--) {
            int v = comp[i]; 
            sz[par[v]] += sz[v]; 
        }
        int c = rt, best = m; 
        for (int v : comp) {
            int mx = (int)comp.size() - sz[v]; 
            for (int u : tr[v]) {
                if (!removed[u] && par[u] == v) {
                    chmax(mx, sz[u]); 
                }
            }
            if (mx < best) {
                best = mx; 
                c = v; 
            }
        }
        label[c] = -1; 
        int cnt = 0; 
        vector<int> order; 
        for (int v : tr[c]) {
            if (removed[v]) continue; 
            label[v] = ++cnt; 
            par[v] = c; 
            order.push_back(v); 
        }
        for (int i = 0; i < (int)order.size(); i++) {
            int v = order[i]; 
            for (int u : tr[v]) {
                if (removed[u] || u == par[v]) continue; 
                label[u] = label[v]; 
                par[u] = v; 
                order.push_back(u); 
            }
        }
        vector<int> sources{c}; 
        for (int v : comp) {
            if (label[v] <= 0) continue; 
            for (const auto& e : g[v]) {
                if (label[e.to] > label[v]) {
                    sources.push_back(v); 
                }
            }
        }
        for (int s : sources) {
            for (int v : comp) dist[v] = INF; 
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq; 
            dist[s] = 0; 
            pq.push({0, s}); 
            while (!pq.empty()) {
                auto[d, v] = pq.top(); 
                pq.pop(); 
                if (dist[v] != d) continue; 
                for (const auto& e : g[v]) {
                    if (!label[e.to]) continue; 
                    ll nd = d + e.cost; 
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd ;
                        pq.push({nd, e.to}); 
                    }
                }
            }
            for (auto[u, v, id] : qs) {
                chmin(ans[id], dist[u] + dist[v]); 
            }
        }
        vector<vector<Query>> split(cnt); 
        for (auto qu : qs) {
            int u = label[qu.u], v = label[qu.v]; 
            if (u > 0 && u == v) {
                split[u - 1].push_back(qu); 
            }

        }
        removed[c] = true; 
        int j = 0; 
        for (int v : tr[c]) {
            if (removed[v]) continue; 
            if (!split[j].empty()) {
                tasks.push({v, move(split[j])}); 
            }
            j++; 
        }
        for (int v : comp) label[v] = 0; 

    }
    for (ll v : ans) prints(v); 
}
