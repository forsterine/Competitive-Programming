#include "other/template.hpp"
#include "graph/Graph.hpp"
using namespace std;

int main() {
    int n; 
    scan(n); 
    Graph<int> g(n); 
    rep(i, 1, n) {
        int a, b; 
        scan(a, b); 
        g.add_edge(a - 1, b - 1, (int)i); 
    }
    vector<int> p(n, -1), sz(n, 1), ord{0}; 
    p[0] = 0; 
    for (int i = 0; i < (int)ord.size(); i++) {
        int v = ord[i]; 
        for (auto e : g[v]) {
            if (e.to == p[v]) continue; 
            p[e.to] = v; 
            ord.push_back(e.to); 
        }
    }
    rrep(i, n) {
        int v = ord[i]; 
        if (v) sz[p[v]] += sz[v]; 
    }
    int c = 0; 
    for (int v : ord) {
        if (sz[v] > n / 2) c = v; 
    }
    vector<ll> ans(n);
    ans[c] =1LL * n * (n - 1) / 2; 
    ord = {c}; 
    p[c] = -1; 
    for (int i= 0; i < (int)ord.size(); i++) {
        int v = ord[i]; 
        for(auto e : g[v]) {
            if (e.to == p[v]) continue; 
            p[e.to] = v; 
            ans[e.to] = ans[v] - e.cost; 
            ord.push_back(e.to); 
        }
    }
    prints(ans);
}
