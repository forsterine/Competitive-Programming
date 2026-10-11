#include "other/template.hpp"
using namespace std; 
int main() {
    int n; 
    scan(n); 
    ll ans = 0; 
    rep(i, n) {
        ll x; scan(x); 
        if (i % 2 == 0) ans += x; 
    }
    prints(ans);
}