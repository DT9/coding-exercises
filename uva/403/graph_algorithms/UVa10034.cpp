/* UVa problem: 10034
 *
 * Topic: graph
 *
 * Level: trivial
 * 
 * Brief problem description: 
 *
 *   Given points (freckles), find the mst
 *
 * Solution Summary:
 *
 *   use kruskals & uf to solve for min dist
 *
 * Used Resources:
 *
 *   cp3, code archive
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN   1000
#define MAXM   1000000
#define EPS    1e-8
int n;

typedef struct{
  double u, v, w;
  int a, b;
} Edge;

int  sets[MAXN];
Edge edge[MAXM];
int numedge;
int hh = 10, hg = 1000;

int cmp_Edge(const void * c, const void * d){
  Edge *a = (Edge*) c; 
  Edge *b = (Edge*) d;
  if(fabs(a->w-b->w) < EPS) return 0;
  if(a->w < b->w) return -1;
  return 1;
}

int getRoot(int x){
  if(sets[x] < 0) return x;
  return sets[x] = getRoot(sets[x]);
}

void Union(int a, int b){
  int ra = getRoot(a);
  int rb = getRoot(b);
  if(ra != rb){
    sets[ra] += sets[rb];
    sets[rb] = ra;
  }
}
double GetDistance(int i, int j)
{
    double xdiff = edge[i].u - edge[j].u;
    double ydiff = edge[i].v - edge[j].v;
    return sqrt(xdiff*xdiff+ydiff*ydiff);
}
double mintree(){
  double weight = 0.0;
  int i, k, count;
  for (i = 0, k = 0; i < numedge; ++i) {
      for (int j = 0; j < i; ++j) {
          double dist = GetDistance(i, j);
          edge[k].w = dist;
          edge[k].a = i;
          edge[k].b = j;
          k++;
      }
  }
  qsort(edge, k, sizeof(edge[0]), cmp_Edge);
  for(i = count = 0; count < n-1; i++){
    if(getRoot(edge[i].a) != getRoot(edge[i].b)){
      Union(edge[i].a, edge[i].b);
      weight += edge[i].w;
      count++;
    }
  }
  return weight;
}

int main(){
  int i,tc;
  double weight;
  cin >> tc;
  while (tc--) {
    while(scanf("%d", &n) == 1){
      numedge = n;
      memset(sets, -1, sizeof(sets));
      for(i = 0; i < numedge; i++){
        scanf("%lf %lf", &edge[i].u, &edge[i].v);
      }
      weight = mintree();
      printf("%.2f\n", weight);
    }
  }
  

  return 0;
}