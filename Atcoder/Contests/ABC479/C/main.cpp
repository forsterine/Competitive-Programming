
#include "other/template.hpp"
using namespace std;

int main() {
    int n;
    ll W;
    scan(n, W);
    vector<pair<ll, ll>> a(n);
    ll sa = 0;
    rep(i, n) {
        scan(a[i].first, a[i].second);
        sa = min(2000001LL, sa + min(2000001LL, a[i].first));
    }
    if (n <= 40) {
        auto gen = [&](int l, int r) {
            vector<pair<ll, ll>> v{{0, 0}};
            rep(i, l, r) {
                int m = v.size();
                rep(j, m) {
                    auto [w, s] = v[j];
                    if (a[i].second <= W - w) {
                        v.emplace_back(w + a[i].second, s + a[i].first);
                    }
                }
            }
            return v;
        };
        auto x = gen(0, n / 2);
        auto y = gen(n / 2, n);
        sort(all(y));
        int m = 0;
        ll best = -1;
        for (auto [w, s] : y) {
            if (s > best) {
                y[m++] = {w, s};
                best = s;
            }
        }
        y.resize(m);
        ll ans = 0;
        for (auto [w, s] : x) {
            auto it = upper_bound(all(y), make_pair(W - w, LLONG_MAX));
            if (it != y.begin()) {
                chmax(ans, s + prev(it)->second);
            }
        }
        prints(ans);
        return 0;
    }
    if (W <= 2000000 && (__int128)n * W <= 200000000) {
        int w = W;
        vector<ll> dp(w + 1);
        for (auto [v, b] : a) {
            if (b > w) continue;
            rrep(j, (int)b, w + 1) {
                chmax(dp[j], dp[j - b] + v);
            }
        }
        prints(dp[w]);
        return 0;
    }
    if (sa <= 2000000 && (__int128)n * sa <= 200000000) {
        int s = sa;
        vector<ll> dp(s + 1, INF<ll>);
        dp[0] = 0;
        for (auto [v, b] : a) {
            if (b > W) continue;
            rrep(j, (int)v, s + 1) {
                if (dp[j - v] <= W - b) {
                    chmin(dp[j], dp[j - v] + b);
                }
            }
        }
        int ans = 0;
        rep(j, s + 1) {
            if (dp[j] <= W) ans = j;
        }
        prints(ans);
        return 0;
    }
    vector<pair<ll, ll>> dp{{0, 0}};
    for (auto [v, b] : a) {
        if (b > W) continue;
        vector<pair<ll, ll>> add, nd;
        add.reserve(dp.size());
        for (auto [w, s] : dp) {
            if (b <= W - w) {
                add.emplace_back(w + b, s + v);
            }
        }
        int i = 0, j = 0;
        ll best = -1;
        while (i < (int)dp.size() || j < (int)add.size()) {
            ll w = min( i < (int)dp.size() ? dp[i].first : LLONG_MAX, j < (int)add.size() ? add[j].first : LLONG_MAX);
            ll s = -1;
            while (i < (int)dp.size() && dp[i].first == w) {
                chmax(s, dp[i++].second);
            }
            while (j < (int)add.size() && add[j].first == w) {
                chmax(s, add[j++].second);
            }
            if (s > best) {
                nd.emplace_back(w, s);
                best = s;
            }
        }
        dp.swap(nd);
    }
    prints(dp.back().second);
}
