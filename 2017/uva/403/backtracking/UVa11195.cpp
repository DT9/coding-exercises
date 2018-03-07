/* UVa problem: another n queens problem
 *
 * Topic: backtracking
 *
 * Level: nontriv 881/1338
 * 
 * Brief problem description: 
 *
 *   given n*n boards, solve n queens
 *
 * Solution Summary:
 *
 *   backtrack
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
/* 8 Queens Chess Problem */
#include <cstdlib>                       // we use the int version of 'abs'
#include <cstdio>
#include <cstring>
#include <bitset>
#include <iostream>
using namespace std;

// ok to use global variables
int row[8], TC, a, b, lineCounter, n, ans; 
char board[15][15];
bitset<30> rw, ld, rd; // for the largest n = 14, we have 27 diagonals

void backtrack(int c) {
    if (c == n) { ans++; return; } // a solution 
    for (int r = 0; r < n; r++) // try all possible row
    if (board[r][c] != '*' && !rw[r] && !ld[r - c + n - 1] && !rd[r + c]) { 
        rw[r] = ld[r - c + n - 1] = rd[r + c] = true; // flag off 
        backtrack(c + 1);
        rw[r] = ld[r - c + n - 1] = rd[r + c] = false; // restore
}   }
int main() {
int tc = 1;
string s;
  while (cin >> n,n) {
    memset(board,'.',sizeof(board));
    for (int i = 0; i < n; i++){
        cin >> board[i];
    }
    backtrack(0);    // generate all possible 8! candidate solutions
    cout << "Case " << tc << ": " << ans << endl;
    tc++; ans = 0;
} } // return 0;