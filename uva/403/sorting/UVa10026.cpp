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
 using namespace std;
 int main() {
     int N;
     cin >> N;
     for (int tc = 0; tc < N; tc++){
         vector<tuple<int,int,int>> v;
         int jobs;
         cin >> jobs;
         for (int j= 0; j<jobs; j++){
             int t,s;
             cin >> t >> s;
             v.push_back(make_tuple(j,t,s));
         }
         sort(begin(v),end(v),
            [](const tuple<int,int,int> a, const tuple<int,int,int> b) -> bool {
             return std::get<1>(a) > std::get<1>(b);
         });
         for (auto i:v) {
             cout << std::get<0>(i) << " ";
         }
         cout << endl;
     }
     return 0;
 }