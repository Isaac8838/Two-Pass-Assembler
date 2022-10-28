#include <bits/stdc++.h>
using namespace std;

int main() {
    string s = "1000";
    string s1 = "4096";
    stringstream ss;
    int val, sv;
    ss.clear();
    ss << s1;
    ss >> val;
    ss.clear();
    ss << hex << s;
    ss >> sv;
    ss.clear();
    ss << val + sv;
    ss >> hex >> s;
    cout << s;
}