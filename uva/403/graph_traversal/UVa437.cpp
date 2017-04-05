/* UVa problem: tower of babylon
 *
 * Topic: graph traversal
 *
 * Level: trivial
 * 
 * Brief problem description: 
 *
 *   build the tallest tower with blocks types, 
 *   blocks bases must be smaller than previous
 *
 * Solution Summary:
 *
 *   Longest increasing subsequence
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
#include <algorithm>
#include <cstdio>
#include <stack>
#include <functional>
#include <numeric>
#include <vector>
#include <iostream>
using namespace std;

struct block{
    int x,y,z;
    block(){}
    block(int a, int b, int c){
        x=a;
        y=b;
        z=c;
    }
    bool operator < (block X) const{
        if((x<X.x && y<X.y) || (x<X.y && y<X.x)) return true;
        return false;
    }
};
struct compare_xy {
    bool operator ()(const block& left, const block& right) const {
        int x = (left.x + left.y);
        int y = (right.x + right.y);
        return x < y;
    }
};

void print_array(const char *s, vector<block> a, int n) {
  for (int i = 0; i < n; ++i) {
    if (i) printf(", ");
    else printf("%s: [", s);
    printf("%d", a[i].z);
  }
  printf("]\n");
}
int reconstruct_print(int end, vector<block> a, int p[]) {
  int x = end; int sum = 0;
  stack<block> s;
  for (; p[x] >= 0; x = p[x]) {
      printf("(%d %d %d)",a[x].x,a[x].y,a[x].z);
      sum+= a[x].z;
      s.push(a[x]);
  }
  printf("(%d %d %d)",a[x].x,a[x].y,a[x].z);
  sum += a[x].z;
  cout << endl;
  return sum;
}
// 40 21 28 342

#define MAX_N 100000
#define mm 90
typedef unsigned long ul;

int main() {
    int n, c = 1;
    int x, y, z;
    vector<block> L(MAX_N);
    int L_id[MAX_N], P[MAX_N];
    vector<block> A;
    while (cin >> n, n) {
        for (int i = 0; i < n; i++){
            cin >> x >> y >> z;
            A.push_back(block(x,y,z));
            A.push_back(block(y,z,x));
            A.push_back(block(z,x,y));
        }
        sort(A.begin(),A.end(),compare_xy());
        for(block i : A) {
          printf("(%d %d %d)",i.x,i.y,i.z);
        } cout << endl;
        int lis = 0, lis_end = 0;
        for (int i = 0; i < n*3; ++i) {
            int pos = lower_bound(L.begin(), L.begin() + lis, A[i]) - L.begin();
            L[pos] = A[i];
            L_id[pos] = i;
            P[i] = pos ? L_id[pos - 1] : -1;
            if (pos + 1 > lis) {
                lis = pos + 1;
                lis_end = i;
            }
        }
        for(int i=0; i < n*9; i++) {
          printf("(%d)",P[i]);
        } cout << endl;
        int ans = 0;
        ans = reconstruct_print(lis_end, A, P);
        A.clear();L.clear();
        cout << "Case " << c++ << ": maximum height = " 
        << ans << endl;
    }
    return 0;
}