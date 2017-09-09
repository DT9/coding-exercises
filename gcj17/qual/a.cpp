#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

int N = 5, V, 
    memo[7500];
bitset<1000> p;
// N and coinValue are fixed for this problem, max V is 7489
// int coin(int type, int value) {
//   if (value == 0)              return 1;
//   if (value < 0 || type == N)  return 0;
//   int & found = memo[type][value];
//   if (found != -1) return found;
//   return found = coin(type + 1, value) + coin(type, value - coinValue[type]);
// }
void flip(int s, int k) {
    for (int i = s; i < k; i++) {
        p.flip(i);
    }
}
int fl(int type, int value) {
  if (value == 0)              return 1;
  if (value < 0 || type == N)  return 0;
  int & found = memo[value];
  if (found != -1) return found;
  return found = fl(type + 1, value) + fl(type, value - flValue[type]);
}


int main() {
    memset(memo,-1,sizeof(memo));
    int tc, k, c;
    string s;
    cin >> tc;
    while (cin >> s, cin >> k) {
        p.reset(1);
        for (int i = 0; i < s.size(); i++) {
            if (s[i] = '-') p[i] = 1;
        }
        cout << fl(0,k) << endl;
    }
     return 0;
 }