#include "other/template.hpp"
#include "graph/Graph.hpp"
#include "data-struct/unionfind/UnionFind.hpp"
using namespace std;
int main() {
    int n; 
    scan(n); 
    vector<ll> a(n); 
    scan(a); 
    vector<int> c(n - 1); 
    vector<ll> l(n - 1); 
    scan(c, l); 
    for (int& v : c) --v; 
    Graph<ll> g(n); 
    rep(i, n - 1) g.add_edge(i, c[i], l[i]); 
    constexpr int LOG = 20; 
    vector<array<int, LOG>> up(n); 
    vector<int> dep(n); 
    vector<ll> dis(n); 
    up[n - 1].fill(n - 1); 
    rrep(i, n - 1) {
        up[i][0] = c[i]; 
        rep(j, 1, LOG) {
            up[i][j] = up[up[i][j - 1]][j - 1]; 
        }
        dep[i] = dep[c[i]] + 1; 
        dis[i] = dis[c[i]] + l[i]; 
    }
    auto dist =[&](int u,int v) -> ll {
        int x = u, y = v; 
        if (dep[x] < dep[y]) swap(x, y); 
        int d = dep[x] -dep[y]; 
        rep(k, LOG) {
            if ((d >> k) & 1) x = up[x][k]; 
        }
        if (x != y) {
            rrep(k, LOG) {
                if (up[x][k] != up[y][k]) {
                    x = up[x][k]; 
                    y = up[y][k]; 
                }
            }
            x = up[x][0]; 
        }
        return dis[u] + dis[v] - 2 * dis[x];
    }; 
    vector<int> ord(n); 
    iota(all(ord), 0); 
    sort(all(ord), [&](int i, int j) {
        return a[i] < a[j]; 
    }); 
    UnionFind uf(n); 
    vector<int> p(n), q(n); 
    vector<ll> diam(n); 
    vector<bool> on(n); 
    ll ans = LLONG_MIN; 
    int x = 0, y = 1; 
    for (int v : ord) {
        on[v] = true; 
        int s = v, t = v, z = v; 
        ll d = 0, far = 0; 
        for (const auto& e : g[v]) {
            int u = e.to; 
            if (!on[u]) continue; 
            int r = uf.find(u); 
            if (diam[r] > d) {
                d = diam[r]; 
                s = p[r]; 
                t = q[r]; 
            }
            ll a1 = dist(v, p[r]); 
            ll a2 = dist(v, q[r]); 
            ll f= max(a1, a2); 
            int w = a1 >= a2 ? p[r] : q[r]; 
            if (far + f > d) {
                d= far + f; 
                s = z; 
                t = w; 
            }
            if (f > far) {
                far = f; 
                z =w; 
            }
            uf.merge(v, u); 
        }
        int r = uf.find(v); 
        p[r] = s; 
        q[r] = t; 
        diam[r] = d; 
        if (d && d - a[v] > ans) {
            ans = d - a[v]; 
            x = s; 
            y = t; 
        }   
    }
    prints(x + 1, y + 1); 
}
