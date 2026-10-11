#include "other/template.hpp"
#include "graph/Graph.hpp"
#include "data-struct/segment/BinaryIndexedTree.hpp"
using namespace std;

int main() {
    int n; 
    scan(n); 
    vector<ll> s(n), t(n); 
    scan(s, t); 
    vector<int> a(n), b(n), rs(n), rt(n); 
    iota(all(a), 0); 
    iota(all(b), 0); 
    sort(all(a), [&](int i, int j) {
        return s[i] < s[j]; 
    });
    sort(all(b), [&](int i, int j) {
        return t[i] < t[j]; 
    });
    rep(i, n) {
        rs[a[i]] = i; 
        rt[b[i]] = i; 
    }
    ll ans = 0; 
    rep(i, n) {
        ans += abs((s[i] - rs[i]) - (t[i] - rt[i])); 
    }
    BinaryIndexedTree<int> fw(n); 
    rep(i, n) {
        int r = rt[a[i]]; 
        ans += i - fw.sum(r + 1); 
        fw.add(r, 1); 
    }
    prints(ans);
}
