/* UVa problem: 202 repeating decimals
 *
 * Topic: arithmetic
 * 
 * Level: mandatory
 * 
 * Brief problem description: 
 *
 *   find and calculate the length and value of repeating decimals
 *
 * Solution Summary:
 *
 *   using floyd's cycle finding algorithm, and f as a function of
 *   long division generation, I was able to find out the starting
 *   point and length of the repeating decimals, and then outputing
 *   the answer from the cached decimals.
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
#include <string>
#include <vector>
#include <cassert>
using namespace std;

typedef pair<int, int> ss;

int a, b, divisor;
vector<char> out;
int f(int dividend) { 
    return dividend % divisor * 10; 
}

ss floydCycleFinding(int x0) {  // function int f(int x) is defined earlier
  // 1st part: finding k*mu, back's speed is 2x front's
    out.push_back(x0/divisor + '0');
  int front = f(x0), back = f(f(x0));    // f(x0) is the node next to x0
  while (front != back) { 
      out.push_back(front/divisor + '0');
      front = f(front); back = f(f(back)); 
    }
  // 2nd part: finding mu, back and front move at the same speed
  int mu = 0; back = x0;
  while (front != back) { 
      out.push_back(front/divisor + '0');      
      front = f(front); back = f(back); mu++; 
    }
  // 3rd part: finding lambda, back moves, front stays
  int lambda = 1; back = f(front);
  while (front != back) {       
      back = f(back); lambda++; 
    }
  return ss(mu, lambda);
}

 int main() {    
    while (cin >> a >> b) {
        divisor = b;
        ss result = floydCycleFinding(a%b*10);
        int first = result.first, last = result.second, 
            digits = last;
        string str(out.begin(),out.end()),
        nonrepeat = str.substr(0,first), 
        repeat;
        if (digits > 50) {
            repeat = str.substr(first,50) + "...";
            assert(repeat.size() == 53);
        }
        else {
            repeat = str.substr(first,digits);
        }
        // cout << str << endl;
        // cout << first << " " << last << endl;
        cout << a << "/" << b << " = " << a / b << '.' << nonrepeat << "(" << repeat << ")" << endl;
        cout << "   " << digits << " = number of digits in repeating cycle" << endl << endl;
        out.clear();
    }
     return 0;
 }
