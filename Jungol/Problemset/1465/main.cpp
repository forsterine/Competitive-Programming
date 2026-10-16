#include "other/template.hpp"
#include "data-struct/segment/BinaryIndexedTree.hpp"
using namespace std;
struct Point {
    int x, y; 
}; 
struct Section {
    vector<int> x; 
    vector<pair<int, int>> seg;  
    int l, r; 
}; 
Section build (const vector<Point>& p, int fx, int fy) {
    vector<tuple<int, int, int>> ev; 
    vector<int> xs; 
    int n = p.size(); 
    rep(i, n) {
        auto a = p[i], b = p[(i + 1) % n]; 
        xs.push_back(a.x); 
        if (a.y != b.y) continue; 
        ev.emplace_back(min(a.x, b.x), 1, a.y); 
        ev.emplace_back(max(a.x, b.x), -1, a.y); 
    }
    sort(all(ev)); 
    sort(all(xs)); 
    xs.erase(unique(all(xs)), xs.end()); 
    vector<pair<int, int>> seg(xs.size() - 1, {-1, -1}); 
    set<int> s; 
    int j = 0, parity = 0; 
    rep(i, (int)seg.size()) {
        while (j < (int)ev.size() && get<0>(ev[j]) == xs[i]) {
            auto[x, d, y] = ev[j++]; 
            if (y < fy) parity ^= 1; 
            if (d == 1) s.insert(y); 
            else s.erase(y); 
        }
        auto it= s.lower_bound(fy); 
        if (it != s.end() && *it == fy) {
            if (parity) seg[i] = {*prev(it), fy}; 
            else seg[i]= {fy, *next(it)}; 
        } else if (parity) {
            seg[i] = {*prev(it), *it}; 
        }
    }
    int k = upper_bound(all(xs), fx) - xs.begin() - 1; 
    int l = k, r =k + 1; 
    while (l && seg[l - 1].first != -1) --l; 
    while (r < (int)seg.size() && seg[r].first != -1) ++r; 
    return {move(xs), move(seg), l, r}; 
}
bool check(const vector<Point>& p, int fx, int fy, ll area) {
    auto a = build(p, fx, fy); 
    vector<Point> q = p; 
    for (auto& v : q) swap(v.x, v.y); 
    auto b = build(q, fy, fx); 
    ll ah = 0, av = 0, both = 0; 
    rep(i, a.l, a.r) {
        auto [lo, hi] = a.seg[i]; 
        ah += 1LL * (a.x[i + 1] - a.x[i]) * (hi - lo); 
    }
    rep(i, b.l, b.r) {
        auto [lo, hi] = b.seg[i]; 
        av += 1LL * (b.x[i + 1] - b.x[i]) * (hi - lo); 
    }
    vector<tuple<int, int, int>> ev; 
    rep(i, b.l, b.r) {
        auto [lo, hi] = b.seg[i]; 
        ev.emplace_back(lo, i, 1);
        ev.emplace_back(hi, i, -1); 
    }
    sort(all(ev)); 
    BinaryIndexedTree<ll> fw(b.x.size() - 1); 
    int j = 0; 
    rep(i, a.l, a.r) {
        while (j < (int)ev.size() && get<0>(ev[j]) <= a.x[i]) {
            auto [x, k, d] = ev[j++]; 
            fw.add(k, 1LL * d * (b.x[k + 1] - b.x[k])); 
        }
        auto [lo, hi] = a.seg[i]; 
        int l = lower_bound(all(b.x), lo) - b.x.begin(); 
        int r = lower_bound(all(b.x), hi) - b.x.begin(); 
        both += 1LL * (a.x[i + 1] - a.x[i]) * fw.sum(l, r); 
    }
    return ah + av - both == area; 
}
int main() {
    int n; 
    scan(n); 
    vector<Point> p(n); 
    rep(i, n) scan(p[i].x, p[i].y); 
    ll area = 0; 
    rep(i, n) {
        auto a = p[i], b = p[(i + 1) % n]; 
        area += 1LL * (b.x - a.x) * (b.y + a.y); 
    }
    area = abs(area) / 2; 
    rep(_, 5) {
        int x, y; 
        scan(x, y); 
        prints(check(p, x, y, area) ? "YES" : "NO" ); 
    }
}
