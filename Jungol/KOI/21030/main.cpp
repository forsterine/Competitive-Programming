#include "other/template.hpp"
#include "data-struct/segment/BinaryIndexedTree.hpp"
#include "graph/tree/CartesianTree.hpp"
#include "data-struct/segment/SparseTable.hpp"
#include "data-struct/wavelet/WaveletMatrix.hpp"
using namespace std;
int main() {
    int n, q; 
    scan(n, q); 
    vector<int> a(n), pos(n + 1); 
    scan(a); 
    rep(i, n) pos[a[i]] = i; 
    auto p = CartesianTree<int, greater<int>>(a).get_vec(); 
    vector<int> h(n, 1); 
    rep(x, 1, n + 1) {
        int v = pos[x]; 
        if (p[v] != -1) chmax(h[p[v]], h[v] + 1); 
    }
    constexpr int LOG = 20; 
    vector<array<int, LOG>> lp(n + 1), rp(n + 1); 
    lp[n].fill(n); 
    rp[n].fill(n); 
    vector<int> st; 
    rep(i, n) {
        while (!st.empty() && a[st.back()] < a[i]) {
            st.pop_back(); 
        }
        lp[i][0] = st.empty() ? n : st.back(); 
        st.push_back(i); 
    }
    st.clear(); 
    rrep(i, n) {
        while (!st.empty() && a[st.back()] < a[i]) {
            st.pop_back(); 
        }
        rp[i][0] = st.empty() ? n : st.back(); 
        st.push_back(i); 
    }
    rep(k, 1, LOG) rep(i, n) {
        lp[i][k] = lp[lp[i][k - 1]][k - 1]; 
        rp[i][k] = rp[rp[i][k - 1]][k - 1]; 
    }
    SparseTable<Monoid::Max<int>> rmq(a); 
    WaveletMatrix<int> wm(h); 
    while (q--) {
        int l, r, t; 
        scan(l, r, t); 
        --l, --r; 
        int m = pos[rmq.prod(l, r + 1)]; 
        int ans = r - l + 1 - wm.range_freq(l, r + 1, t + 1); 
        if (h[l] <= t) {
            int v = l, cnt = 1; 
            rrep(k, LOG) {
                int u = rp[v][k]; 
                if (u <= r && h[u] <= t) {
                    cnt += 1 << k; 
                    v = u; 
                }
            }
            ans += cnt; 
        }
        if (h[r] <= t) {
            int v = r ,cnt = 1; 
            rrep(k, LOG) {
                int u = lp[v][k]; 
                if (u < n && u >= l && h[u] <= t) {
                    cnt += 1 << k; 
                    v = u; 
                }
            }
            ans += cnt; 
        }
        ans -= h[m] <= t; 
        prints(ans); 
    }
}
