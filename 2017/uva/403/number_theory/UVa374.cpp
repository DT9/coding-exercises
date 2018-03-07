/* UVa problem: 374 big mod
 *
 * Topic: number theory
 *
 * Level: mandatory, trivial
 * 
 * Brief problem description: 
 *
 *   b^p % p efficiently
 *
 * Solution Summary:
 *
 *   fast exp with mod
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
#include <cmath>
using namespace std;

long fastExp(long b, long p, long m)
{
    long r = 1;
    for (long i = p; i > 0; i/=2)
    {   // odd
        if (i % 2 == 1) r = (r*b) % m;
        b = (b*b) % m;
    }
    return r;
}
int main()
{
    long b, p, m;
    while (cin >> b >> p >> m)
    {
        cout << fastExp(b, p, m) << endl;
    }
    return 0;
}