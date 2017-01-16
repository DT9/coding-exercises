/* UVa problem: 10026 shoemaker's problem
 *
 * Topic: sorting
 *
 * Level: mandatory
 * 
 * Brief problem description: 
 *
 *   Find a sequence of jobs with the minimal fine
 *
 * Solution Summary:
 *
 *   
 *
 * Used Resources:
 *
 *   cppref
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */
 #include <iostream>
 #include <tuple>
 #include <vector>
 #include <algorithm>
 #define t3i tuple<int,int,int>
 using namespace std;
 int main() {
     int N;
     cin >> N;
     while (N--) {
         vector<t3i> v;
         int jobs;
         cin >> jobs;
         for (int j= 1; j<=jobs; j++){
             int t,s;
             cin >> t >> s;
             v.push_back(make_tuple(j,t,s));
         }
         stable_sort(begin(v),end(v),
            [](const t3i a, const t3i b) -> bool {
             return get<1>(a) * get<2>(b) < get<1>(b) * get<2>(a);
         });

         cout << get<0>(v[0]);
         for (unsigned long i = 1; i < v.size(); i++) {
             cout << " " << get<0>(v[i]);
         }
         cout << endl;
         if (N) cout << endl;
     }
     return 0;
 }