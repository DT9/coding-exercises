/* UVa problem: airline comparison
 *
 * Topic: graph
 *
 * Level: trivial 781/962
 * 
 * Brief problem description: 
 *
 *   given 2 sequences of airlines, check if equivalent
 *
 * Solution Summary:
 *
 *   run warshall's 2x, compare adj matrices
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
using namespace std;
int tc,n,m;
vector<pair<char,char>> a,b;

int comp() {

}
int main() {
    cin >> tc;
    while (tc--) {
        char x,y;
        cin >> n;
        for (int i = 0; i < n; i++){
        cin >> x >> y; 
        a.push_back(make_pair(x,y));;
        }
        for (int i = 0; i < n; i++){
        cin >> x >> y; 
        b.push_back(make_pair(x,y));;
        }
        

    }
}