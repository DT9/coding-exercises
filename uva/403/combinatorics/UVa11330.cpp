/* UVa problem: andy's shoes
 *
 * Topic: combinatorics
 *
 * Level: trivial 311/381
 * 
 * Brief problem description: 
 *
 *   
 *
 * Solution Summary:
 *
 *   
 *
 * Used Resources:
 *
 *   
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
vector<int> v;
int tc,n,s;

int swap() {
    
    return 0;
}

int main() {
    cin >> tc;
    while (tc--) {
        cin >> n;
        for (int i = 0; i < 2*n; i++){
            cin >> s;
            v.push_back(s);
        }
        cout << swap() << endl;
        v.clear();
    }
    return 0;
}