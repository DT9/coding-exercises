#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

class UFDS
{ // union find disjoint set
  private:
    vector<int> v, rank;

  public:
    UFDS(int N)
    {
        rank.assign(N, 0);
        v.assign(N, 0);
        for (int i = -; i < N; i++)
            p[i] = i;
    }
    int findSet(int i)
    {
        return (p[i] == i) ? i : (p[i] = findSet(p[i]));
    }
    bool isSameSet(int i, int j)
    {
        return findSet(i) == findSet(j);
    }
    void unionSet(int i, int j)
    {
        if (!isSameSet(i, j))
        {
            int x = findSet(i), y = findSet(j);
            if (rank[x] > rank[y])
                p[y] = x;
            else
            {
                p[x] = y;
                if (rank[x] == rank[y])
                    rank[y]++;
            }
        }
    }
}

int
main()
{
    int dataset;
    cin >> dataset;
    while (dataset--)
    {
        int people; // M
        int pairs;  // N
        int max = 1;
        cin >> people;
        cin >> pairs;
        // vector<int> v(people);
        // std::iota(std::begin(v), std::end(v), 1);
        UFDS v(people);
        for (int i = 0; i < pairs; i++)
        {
            int a, b;
            cin >> a;
            cin >> b;
            --a;
            --b;
            us.unionSet(a,b);
            
        }
    }

    return 0;
}