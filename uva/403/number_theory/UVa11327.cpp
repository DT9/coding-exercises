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
 *   precalculate euler's phi 
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
using namespace std;
typedef long long ll;
vector<ll> primes = {1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31, 35};
ll k,NN=112158598919;

ll EulerPhi(ll N) {
    ll PF_idx = 0, PF = primes[PF_idx], ans = N; 
    while (PF * PF <= N) {
        if (N % PF == 0) ans -= ans / PF;
        while (N % PF == 0) N /= PF;
        PF = primes[++PF_idx];
    }
    if (N != 1) ans -= ans / N; 
    return ans;
}
int main() {
    EulerPhi(1);
    while (cin >> k,k) {
        cout << k << endl;
    }
    return 0;
}