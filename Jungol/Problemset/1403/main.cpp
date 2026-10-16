#include "other/template.hpp"
using namespace std;
struct Snail {
    ll x, y, dx, dy; 
    ld len, wall; 
}; 
struct Event {
    ld t, a, b; 
    int i, j; 
    bool operator<(const Event& e) const {
        return t > e.t; 
    }
};
ll cross(ll x, ll y, ll u, ll v) {
    return x * v - y * u; 
}
int main() {
    int n, L; 
    scan(n, L); 
    vector<Snail> a(n); 
    priority_queue<Event> pq; 
    rep(i, n) {
        ll x, y, p, q; 
        scan(x, y, p, q); 
        auto& s = a[i]; 
        s = {x, y, p - x, q - y}; 
        s.len = hypotl(s.dx, s.dy); 
        ld t = 1e100L; 
        if (s.dx > 0) t = min(t, (L - x) * s.len / s.dx); 
        if (s.dx < 0) t = min(t, x * s.len / -s.dx); 
        if (s.dy > 0) t = min(t, (L - y) * s.len / s.dy); 
        if (s.dy < 0) t = min(t, y * s.len / -s.dy); 
        s.wall = t; 
        pq.push({t, t, 0, (int)i, -1}); 
    }
    constexpr ld eps = 1e-11L; 
    rep(i, n) rep(j, i + 1, n) {
        const auto& s = a[i]; 
        const auto& u = a[j]; 
        ll c = cross(s.dx, s.dy, u.dx, u.dy); 
        ll x = u.x - s.x, y =u.y - s.y; 
        ld ti, tj; 
        if (c) {
            ll p = cross(x, y, u.dx, u.dy); 
            ll q = cross(x, y, s.dx, s.dy); 
            if (c < 0) {
                c = -c; 
                p = -p; 
                q = -q; 
            }
            if (p < 0 || q < 0) continue; 
            ti =(ld)p / c * s.len; 
            tj = (ld)q / c * u.len; 
        } else {
            if (cross(x, y, s.dx, s.dy)) continue; 
            ld d = (ld)(x * s.dx + y * s.dy) / s.len; 
            if (s.dx * u.dx + s.dy * u.dy > 0) {
                if (d> 0) {
                    ti = d; 
                    tj = 0; 
                } else {
                    ti = 0; 
                    tj = -d; 
                }
            } else {
                if (d <= 0) continue; 
                ti = tj = d / 2; 
            }
        }
        if (ti > s.wall + eps || tj > u.wall + eps) continue; 
        pq.push({max(ti, tj), ti, tj, (int)i, (int)j}); 
    }
    vector<ld> stop(n, 1e100L); 
    vector<bool> alive(n, true); 
    int rem = n; 
    ld ans = 0; 
    auto F = [&](int i, ld t) {
        alive[i] = false; 
        stop[i] = t; 
        rem--; 
        chmax(ans, t); 
        const auto& s = a[i]; 
        rep(j, n) {
            if (!alive[j]) continue; 
            const auto& u = a[j]; 
            if (cross(s.dx, s.dy, u.dx, u.dy)) continue; 
            if (cross(s.x - u.x, s.y - u.y, u.dx, u.dy)) continue; 
            if (s.dx * u.dx + s.dy * u.dy >= 0) continue;
            ld d = (ld)((s.x - u.x) * u.dx + (s.y - u.y) * u.dy) / u.len - t; 
            if (d < t - eps || d > u.wall + eps) continue; 
            pq.push({d, d, t, (int)j, i});  
        }
    }; 
    while(rem && !pq.empty()) {
        auto[t, ti, tj, i, j] = pq.top(); 
        pq.pop(); 
        if (j == -1) {
            if (alive[i] ) F(i, t); 
            continue; 
        }
        if (ti > stop[i] + eps || tj > stop[j] + eps) continue; 
        if (ti + eps >= tj) {
            if (alive[i]) F(i, t); 
        }
        if (tj + eps >= ti) {
            if (alive[j]) F(j, t); 
        }
    }
    ll cents = floorl(ans * 100 + 0.5L + 1e-10L); 
    printf("%lld.%02lld\n", cents / 100, cents % 100); 
}
