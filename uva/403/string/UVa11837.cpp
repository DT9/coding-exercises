/* UVa problem: 11837 Musical Plagiarism
 *
 * Topic: string
 *
 * Level: mandatory
 * 
 * Brief problem description: 
 *
 *   Find the 
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
 #include <cstdio>
 #include <iostream>
 #include <vector>
 #include <map>
 using namespace std;

int convert(string s) {
    map<string,int> notes = { };
    
    return 0;
}

 int main() {
     int m,t;
     cin >> m >> t;
     while(m != 0 && t!= 0) {
        int song[m];
        int plag[t];
        string M,T;
        for(int i=0;i<m;i++){
            cin >> M;
            song[i] = convert(M);
        }
        for(int i=0;i<t;i++){
            cin >> T;
            plag[i] = convert(T);
        }
        //suffix array

        cin >> m >> t;
     }

     return 0;
 }