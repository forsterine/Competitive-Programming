#include "other/template.hpp"
using namespace std; 
int main() {
    int n; 
    string s; 
    scan(n, s); 
    int ans = 0, cnt = 0; 
    for (char c : s) {
        cnt = c == 'R' ? cnt + 1 : 0; 
        chmax(ans, cnt); 
    }
    prints(ans);
}