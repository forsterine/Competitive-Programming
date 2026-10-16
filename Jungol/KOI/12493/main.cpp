#include "other/template.hpp"
#include "data-struct/segment/BinaryIndexedTree.hpp"
using namespace std;
int main() {
    int n, d; 
    scan(n, d); 
    vector<int> x(n); 
    scan(x); 
    vector<int> ans(n); 
    auto calc = [&](const vector<int>& a, bool strict) {
        map<int, pair<int, int>> g; 
        vector<int> pre(n + 1), nxt(n + 1), pos(n); 
        pre[n] = nxt[n] = n; 
        rep(i, n) {
            int x= a[i]; 
            auto it = g.lower_bound(x - d); 
            auto jt = g.upper_bound(x); 
            int v = it == jt ? (jt == g.end() ? n : jt -> second.first) : it -> second.first; 
            int last = it == jt ? (int)i : prev(jt) -> second.second; 
            int u = pre[v]; 
            nxt[u] = i; 
            pre[i] = u; 
            nxt[i] = v; 
            pre[v] =i; 
            g.erase(it, jt); 
            g.emplace_hint(jt, x, make_pair((int)i, last)); 
        }
        int p = 0; 
        for (int i= nxt[n]; i != n; i = nxt[i]) {
            pos[i] = p++; 
        }
        vector<int> ord(n); 
        iota(all(ord), 0); 
        sort(all(ord), [&](int i,int j) {
            return a[i] > a[j]; 
        }); 
        BinaryIndexedTree<int> fw(n); 
        for (int l = 0; l < n; ) {
            int r = l + 1; 
            while (r < n && a[ord[l]]== a[ord[r]]) r++; 
            if (!strict) {
                rep(k, l, r) fw.add(pos[ord[k]], 1); 
            }
            rep(k, l, r) {
                ans[ord[k]] += fw.sum(pos[ord[k]] + 1); 
            }
            if (strict) {
                rep(k, l, r) fw.add(pos[ord[k]], 1); 
            }
            l = r; 
        }
    }; 
    calc(x, false); 
    for (int& v : x) v = -v; 
    calc(x, true); 
    prints(ans); 
}
