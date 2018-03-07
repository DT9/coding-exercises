/* UVa problem: 11955 binomial theorem
 *
 * Topic: arithmetic
 *
 * Level: mandatory
 * 
 * Brief problem description: 
 *
 *   
 *
 * Solution Summary:
 *
 *   construct a pascals triangle using dp.
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
 #include <cstdio>
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <algorithm>
 #include <vector>
 using namespace std;
long long P[50][50];
long long combo(int n, int k) {
    if (n == k || k == 0) return 1;
    long long & found = P[n][k];
    if (!found) found = combo(n-1,k-1) + combo(n-1,k);
    return found;
}

 string expand(string a, string b, int k) {
     stringstream ss;
     if (k==1) {
         ss << a << '+' << b;
         return ss.str();
     }
    ss << a << '^' << k;
     for (int i = 1, j = k - 1; i < k; ++i,--j)
     {
         ss << '+' << combo(k,j) << '*' << a;
         if (j != 1) ss << '^' << j;
         ss << '*' << b;
         if (i != 1) ss << '^' << i;
     }
     ss << '+' << b << '^' << k;
     return ss.str();
 }
 int main() {
     //Build pascals triangle

     int T;
     cin >> T;
     for (int tc = 1; tc <= T; tc++) {
        string a,b,input;
        int power;
        cin >> input;
        int front = input.find('(')+1, back = input.find('+');
        a = input.substr(front,back-front);
        front = input.find('+')+1, back = input.find(')');
        b = input.substr(front,back-front);
        power = stoi(input.substr(input.find('^')+1));
        string expr = expand(a,b,power);
        cout << "Case " << tc << ": " << expr << endl;
     }
     return 0;
 }