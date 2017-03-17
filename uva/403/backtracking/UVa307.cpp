/* UVa problem: sticks 307
 *
 * Topic: backtracking
 *
 * Level: mandatory, nontrivial
 * 
 * Brief problem description: 
 *
 *   given no of sticks and its lengths
 *
 * Solution Summary:
 *
 *   dfs
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
#include <cstdlib> // we use the int version of 'abs'
#include <cstdio>
#include <cstring>
#include <bitset>
#include <iostream>
#include <vector>
using namespace std;

// ok to use global variables
int TC, a, b, lineCounter, n, ans;
char board[15][15];
vector<int> v;
bitset<30> rw, ld, rd; // for the largest n = 14, we have 27 diagonals

void backtrack(int c)
{
    if (c == n)
    {
        ans++;
        return;
    }                           // a solution
    for (int r = 0; r < n; r++) // try all possible row
        if (board[r][c] != '*' && !rw[r] && !ld[r - c + n - 1] && !rd[r + c])
        {
            rw[r] = ld[r - c + n - 1] = rd[r + c] = true; // flag off
            backtrack(c + 1);
            rw[r] = ld[r - c + n - 1] = rd[r + c] = false; // restore
        }
}
int main()
{
    int f;
    while (cin >> n, n)
    {
        v.clear();
        for (int i = 0; i < n; i++)
        {
            cin >> f;
            v.push_back(f);
        }
        // backtrack(0); // generate all possible 8! candidate solutions
        for (auto &iv : v)
            ans += iv;
        ans /= v.size() / 2;
        cout << ans << endl;
        ans = 0;
    }
    return 0;
}
