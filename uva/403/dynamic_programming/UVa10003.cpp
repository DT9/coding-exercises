/* UVa problem: 10003 cutting sticks
 *
 * Topic: dp
 *
 * Level: mandatory, trivial
 * 
 * Brief problem description: 
 *
 *   Given a stick with X length, Y cuts are made at Z points. Each A Length
 *	 remaining will cost A dollars. Find the optimal order for cutting the points.
 *
 * Solution Summary:
 *
 *   We have 2 indices at the start and end of the array. We then check the cache
 *	 or calculate the min value of all the points after each cut through recursion.
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
 #include <vector>
 #include <cstring>
using namespace std;
int memo[52][52];//50+2 for 0 & l
std::vector<int> v;
int cut(int left, int right) {
	if ((left+1)==right) return 0;
	int & found = memo[left][right];
	if(found!=-1) return found;
	int min_cut = 99999;
	for (int i = left+1; i < right; ++i)
	{
		min_cut = min(min_cut,cut(left,i)+cut(i,right)+(v[right]-v[left]));
	}
	return	found = min_cut;
	
}

 int main() {
 	int l;  // < 1000
 	while (cin >> l,l){
 		int n,c; cin >> n;
 		v.push_back(0);
 		for (int i = 0; i < n; ++i){
 			cin >> c; v.push_back(c);
 		}
 		v.push_back(l);
 		memset(memo,-1,sizeof(memo));
		cout << "The minimum cutting is " << cut(0,n+1) << "." << endl; 	
		v.clear();	
 	}
 	return 0;
 }
