#include <iostream>
#include <string>

using namespace std;

#define ll long long
#define ld long double

void solve() {
    string s = "null xs";
    auto it = find(s.begin(), s.end(), 'n');

    string str = s.substr(0, 4);
    if (it == s.begin()) cout << "true" << endl;
    cout << str << endl;
}

int main() {
    cin.tie(0); cout.tie(0);
    ios::sync_with_stdio(false);
    solve();
    return 0;
}