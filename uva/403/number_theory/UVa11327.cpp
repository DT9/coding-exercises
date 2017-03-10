/* UVa problem: 11327 enumerating rational numbers
  *
  * Topic: number theory
  *
  * Level: mandatory
  * 
  * Brief problem description: 
  *
  *   Find rational numbers
  *
  * Solution Summary:
  *
  *   modified sieve, precalculate euler's phi, gcd
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
#include <bitset> // compact STL for Sieve, more efficient than vector<bool>!
#include <cmath>
#include <cstdio>
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#define MAX_NUM 200002
typedef long long ll;
typedef vector<int> vi;
typedef map<int, int> mii;
ll _sieve_size;
bitset<10000010> bs; // 10^7 should be enough for most cases
vi primes;           // compact list of primes in form of vector<int>

// first part
int gcd(int a, int b)
{
    if (b)
        return gcd(b, a % b);
    return a;
}

void sieve(ll upperbound)
{                                 // create list of primes in [0..upperbound]
    _sieve_size = upperbound + 1; // add 1 to include upperbound
    bs.set();                     // set all bits to 1
    bs[0] = bs[1] = 0;            // except index 0 and 1
    for (ll i = 2; i <= _sieve_size; i++)
        if (bs[i])
        {
            // cross out multiples of i starting from i * i!
            for (ll j = i * i; j <= _sieve_size; j += i)
                bs[j] = 0;
            primes.push_back((int)i); // also add this vector containing list of primes
        }
} // call this method in main method

ll EulerPhi(ll N)
{
    ll PF_idx = 0, PF = primes[PF_idx], ans = N; // start from ans = N
    while (N != 1 && (PF * PF <= N))
    {
        if (N % PF == 0)
            ans -= ans / PF; // only counter unique factor
        while (N % PF == 0)
            N /= PF;
        PF = primes[++PF_idx];
    }
    if (N != 1)
        ans -= ans / N; // last factor
    return ans;
}
long long counter[MAX_NUM + 1];
int main()
{
    sieve(200002); // can go up to 10^7 (need few seconds)
    counter[0] = 0;
    counter[1] = 1;
    counter[2] = 2;

    for (int i = 2; i < MAX_NUM; ++i)
    {
        counter[i + 1] = counter[i] + EulerPhi(i);
    }

    long long n;
    while (cin >> n, n)
    {
        if (n == 1)
            cout << "0/1\n";
        else if (n == 2)
            cout << "1/1\n";
        else
        {
            int denom_idx =  find(counter,counter+MAX_NUM,n) - counter - 1; // find denom index = n
            n -= counter[denom_idx];    // subtract denom value
            int i;
            for (i = 1; n; ++i)
                if (gcd(i, denom_idx) == 1) // count n number of primes
                    --n;

            cout << (i - 1) << '/' << denom_idx << '\n';
        }
    }
    return 0;
}