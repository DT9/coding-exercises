#include <iostream> 
#include <cstdio>
#include <vector>
using namespace std;
// Union Find Disjoint Set from CP3 p.79
class UnionFind {
  // OOP style
  private: vector<int> p, rank, size;
  // remember: vi is vector<int>
  public:
    UnionFind(int N) {
      rank.assign(N, 0);
      p.assign(N, 0);
      size.assign(N,1);
      for (int i = 0; i < N; i++) {
      	p[i] = i;
      }
    }
  int findSet(int i) {
    return (p[i] == i) ? i : (p[i] = findSet(p[i]));
  }
  bool isSameSet(int i, int j) {
    return findSet(i) == findSet(j);
  }
  int unionSet(int i, int j) {
    if (!isSameSet(i, j)) {
      // if from different set
      int x = findSet(i), y = findSet(j);
      if (rank[x] > rank[y]) {
      	p[y] = x;
      	return size[x] += size[y];
      }
      // rank keeps the tree short
      else {
        p[x] = y;
        if (rank[x] == rank[y]) rank[y]++;
        return size[y] += size[x];        
      }
    }
  }

};

int main() {
	int t,n,m,a,b;
	cin >> t;
	for (int _ = 0; _ < t; ++_)
	{
		cin >> n >> m;
		//create UFDS
		UnionFind uf(n);
		int max = 1;
		for (int i = 0; i < m; ++i)
		{
			cin >> a >> b;
			a--;b--;
			//union
			int cur = uf.unionSet(a,b);
			if (cur > max) max = cur;
		}
		cout << max << endl;
	}
  	return 0;
};