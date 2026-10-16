#include "other/template.hpp"
#include "data-struct/unionfind/UnionFind.hpp"
using namespace std;
struct Edge {
    int u, v; 
}; 
struct Event {
    int y, x, id, d; 
    bool operator<(const Event& e) const {
        return y < e.y; 
    }
}; 
struct Query {
    int y, x, f; 
    bool operator<(const Query& q) const {
        return y < q.y; 
    } 
}; 
int main() {
    int n; 
    scan(n); 
    vector<int> x(n), y(n); 
    rep(i, n) scan(x[i], y[i]); 
    int m; 
    scan(m); 
    vector<Edge> e(m); 
    vector<array<int, 4>> out(n); 
    for (auto& v : out) v.fill(-1); 
    vector<int> dir(2 * m), nxt(2 * m), face(2 * m , -1); 
    vector<int> up(m, -1); 
    vector<Event> ev; 
    ev.reserve(2 * m); 
    rep(i, m) {
        int u, v; 
        scan(u, v); 
        --u, --v; 
        e[i] = {u, v}; 
        int d; 
        if (x[v] > x[u]) d = 0; 
        else if (y[v] > y[u]) d = 1; 
        else if (x[v] < x[u]) d = 2; 
        else d = 3; 
        dir[2 * i] = d; 
        dir[2 * i + 1] = (d + 2) % 4; 
        out[u][d] = 2 * i; 
        out[v][(d + 2) % 4] = 2 * i + 1; 
        if (x[u] == x[v]) {
            up[i] = y[u] < y[v] ? 2 * i : 2 * i + 1; 
            int a = min(y[u], y[v]); 
            int b = max(y[u], y[v]); 
            ev.push_back({a, x[u], (int)i, 1}); 
            ev.push_back({b, x[u], (int)i, -1}); 
        }
    }
    rep(k, 2 * m) {
        int v = (k & 1) ? e[k / 2].u : e[k / 2].v;
        int d = (dir[k] + 2) % 4; 
        for (int j = 1; j <= 4;j++ ){
            int q = out[v][(d + 4 - j) % 4]; 
            if (q == -1) continue; 
            nxt[k] = q; 
            break; 
        }
    }
    vector<Query> queries; 
    int fc = 0; 
    rep(i, 2 * m) {
        if (face[i] != -1) continue; 
        ll area = 0; 
        int best = -1; 
        for (int k = i; face[k] == -1; k = nxt[k]) {
            face[k] = fc; 
            int u = (k & 1) ? e[k / 2].v : e[k / 2].u; 
            int v = (k & 1) ? e[k / 2].u : e[k / 2].v; 
            area += 1LL * x[u] * y[v] - 1LL * x[v] * y[u]; 
            if (dir[k] == 3 && (best == -1 || x[u] > x[(best & 1) ? e[best / 2].v : e[best / 2].u])) {
                best = k; 
            }
        }
        if (area <=.0 && best != -1) {
            int u = (best & 1) ? e[best / 2].v : e[best / 2].u; 
            int v = (best & 1) ? e[best / 2].u : e[best / 2].v; 
            queries.push_back({min(y[u], y[v]), x[u], fc}); 
        }
        fc++; 
    }
    UnionFind uf(fc + 1); 
    sort(all(ev)); 
    sort(all(queries)); 
    set<pair<int, int>> active; 
    int j = 0; 
    for (auto [qy, qx, f] : queries) {
        while(j < (int)ev.size() && ev[j].y <= qy) {
            auto[ey, ex, id, d] = ev[j++]; 
            if (d == 1) active.insert({ex, id}); 
            else active.erase({ex, id}); 
        }
        auto it = active.upper_bound({qx, INT_MAX}); 
        if (it == active.end()) {
            uf.merge(f, fc); 
        } else {
            uf.merge(f, face[up[it -> second]]); 
        }
    }
    vector<vector<int>> adj(fc + 1); 
    rep(i, m) {
        int a = uf.find(face[2 * i]); 
        int b = uf.find(face[2 * i + 1]); 
        if (a == b) continue; 
        adj[a].push_back(b); 
        adj[b].push_back(a); 
    }
    vector<int> dist(fc + 1, -1); 
    queue<int> q; 
    int rt = uf.find(fc); 
    dist[rt] = 0; 
    q.push(rt); 
    while (!q.empty()) {
        int v =q.front(); 
        q.pop(); 
        for (int u : adj[v]) {
            if (dist[u] != -1) continue; 
            dist[u] = dist[v] + 1; 
            q.push(u); 
        }
    }
    int ans = 0; 
    rep(i, m) {
        int a = uf.find(face[2 * i]); 
        int b = uf.find(face[2 * i + 1]); 
        ans += dist[a] == dist[b]; 
    }
    prints(ans); 
}
