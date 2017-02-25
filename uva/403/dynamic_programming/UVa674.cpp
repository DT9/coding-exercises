/* UVa problem: 674 coin change
 *
 * Topic: dp
 *
 * Level: trivial
 * 
 * Brief problem description: 
 *
 *   Given x amount of money, find total no. of combos
 *   with 50,25,10,5,1 cent coins to sum x
 *
 * Solution Summary:
 *
 *   compute the combinations while using memoization
 *
 * Used Resources:
 *
 *   cp3
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N = 5, V, 
    coinValue[5] = {1, 5, 10, 25, 50}, 
    memo[6][7500];
// N and coinValue are fixed for this problem, max V is 7489

int coin(int type, int value) {
  if (value == 0)              return 1;
  if (value < 0 || type == N)  return 0;
  int & found = memo[type][value];
  if (found != -1) return found;
  return found = coin(type + 1, value) + coin(type, value - coinValue[type]);
}


int main() {
    memset(memo,-1,sizeof(memo));
    int value;
    while (cin >> value) {
        cout << coin(0,value) << endl;
    }
     return 0;
 }