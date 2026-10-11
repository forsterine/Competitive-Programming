#include "other/template.hpp"
#include "graph/Graph.hpp"
#include "data-struct/segment/BinaryIndexedTree.hpp"
#include "math/Rational.hpp"
using namespace std;

int main() {
    int n; 
    scan(n); 
    vector<ll> a(n), b(n); 
    scan(a, b); 
    auto C = [&](const vector<ll>& a, const vector<ll>& b) {
        vector<pair<Fraction, ll>> v; 
        ll sum = 0; 
        rep(i, n) {
            if (a[i] == 0) continue; 
            v.emplace_back(Fraction(b[i], a[i]), abs(a[i])); 
            sum += abs(a[i]); 
        }
        Fraction t(0); 
        if (sum) {
            sort(all(v), [](const auto& x, const auto& y) {
                return x.first < y.first; 
            }); 
            ll cur = 0; 
            for(auto [f, w] : v) {
                cur += w; 
                if (cur * 2 >= sum) {
                    t = f; 
                    break; 
                }
            }
            chmax(t, Fraction(-1)); 
            chmin(t, Fraction(1)); 
        }
        auto[p, q] = t.get_pair(); 
        ll res =0; 
        rep(i, n) res += abs(b[i] * q-  a[i] * p); 
        return Fraction(res, q); 
    }; 
    auto ans = min(C(a, b), C(b, a)); 
    prints(ans.get_num(), ans.get_den()); 
}
