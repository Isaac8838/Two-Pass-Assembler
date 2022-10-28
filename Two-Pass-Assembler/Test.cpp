#include <bits/stdc++.h>
using namespace std;

int main() {
    std::string str = "107a";
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    cout << str;
}