/* UVa problem: 10327 flip sort
 *
 * Topic: sorting
 *
 * Level: mandatory
 * 
 * Brief problem description: 
 *
 *   Count the minimum no. of swaps to sort an array
 *
 * Solution Summary:
 *
 *   Bubble sort the array and count number of swaps
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
 int main() {
     int N;
     while (cin >> N) {
         vector<int> v;
         for(int i=0;i<N;i++){
             int num; 
             cin >> num;
             v.push_back(num);
         }
         int M = 0;
         for (int i=0;i<N;i++){
             for (int j=i+1;j<N;j++){
                 if (v[i]>v[j]) M++;
             }
         }
         cout << "Minimum exchange operations : " << M << endl;
     }
     return 0;
 }