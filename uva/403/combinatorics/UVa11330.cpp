/* UVa problem: andy's shoes
 *
 * Topic: combinatorics
 *
 * Level: trivial 311/381
 * 
 * Brief problem description: 
 *
 *   given an array of shoe pairs, find min # of swaps
 *
 * Solution Summary:
 *
 *   
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
#include <algorithm>
#include <vector>
using namespace std;
int tc, n, s, l, v[10005];

int main()
{
    cin >> tc;
    while (tc--)
    {
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            cin >> s >> l;
            v[s] = l;
        }
        int count = 0;
        for (int i = 1, c = v[i]; i <= 10000; i++, c = v[i]) {
            if (v[i] != 0) {
                v[i] = 0;
                while (v[c] != 0) {
                    count++;
                    int tmp = v[c];
                    v[c] = 0;
                    c = tmp;
                }
            }
        }
        cout << count << endl;
    }
    return 0;
}