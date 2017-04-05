#include <iostream>

using namespace std;

const int Max = 26;
int N,T;

bool A[Max][Max];
bool B[Max][Max];

void read(bool arr[Max][Max])
{
    cin >> N;
    
    while (N--)
    {
        char from, to;
        cin >> from >> to;
        
        arr[from - 'A'][to - 'A'] = 1;
    }
}

void warshallx2()
{
    for (int k = 0; k < Max; ++k)
    {
        for (int i = 0; i < Max; ++i)
        {
            for (int j = 0; j < Max; ++j)
            {
                A[i][j] |= (A[i][k] && A[k][j]);
                B[i][j] |= (B[i][k] && B[k][j]);
            }
        }
    }
}

int main()
{
    
    cin >> T;
    
    for (int t = 0; t < T; ++t)
    {
        if (t)
            cout << endl;
        
        for (int i = 0; i < Max; ++i)
        {
            for (int j = 0; j < Max; ++j)
            {
                A[i][j] = B[i][j] = false;
            }
        }
        
        read(A);
        read(B);
        
        warshallx2();
        
        bool eq = 1;
        
        for (int i = 0; i < Max; ++i)
            for (int j = 0; j < Max; ++j)
                eq &= (A[i][j] == B[i][j]);
        
        cout << (eq ? "YES" : "NO") << endl;
    }
}