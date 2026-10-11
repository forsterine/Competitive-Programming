
#include "other/template.hpp"
using namespace std;

int main() {
    int n, q; 
    scan(n, q); 
    vector<ll> l(n + 1), r(n + 1); 
    rep(i, 2, n + 1) scan(l[i]); 
    rep(i, 1, n) scan(r[i]); 
    vector<ll> a(n  + 1), b(n + 1); 
    ll mn = r[1]; 
    a[1] = r[1]; 
    rep(i, 2, n) {
        a[i] = min(r[i], l[i] + mn); 
        chmin(mn, r[i]); 
    }
    mn = l[n]; 
    b[n] = l[n]; 
    rrep(i, 2, n) {
        b[i] = min(l[i], r[i] + mn); 
        chmin(mn, l[i]); 
    }
    while(q--) {
        int s, t; 
        scan(s, t); 
        prints(s < t ? a[s] : b[s]); 
    }
}
