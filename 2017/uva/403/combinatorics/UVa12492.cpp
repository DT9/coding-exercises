/* UVa problem: rubik's cycle
 *
 * Topic: combinatorics
 *
 * Level: trivial
 * 
 * Brief problem description: 
 *
 *   Given a rubiks cube, capital letter 90 deg cw rotate, 
 *   lowercase 90 deg ccw rotate, a sequence of movements,
 *   solve the cube
 *
 * Solution Summary:
 *
 *   todo: find lcm of the cube permutations or extended gcd
 *   actual : brute force the cube
 *
 * Used Resources:
 *
 *  cp3, code archive
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
using namespace std;

int idx, i, j, d, N, cnt;
map<char,int> r;
string in;
typedef struct
{
    char g[9][12];
} Cube;

int cx[6] = {4, 4, 4, 4, 1, 7};
int cy[6] = {1, 4, 7, 10, 4, 4};

int rx[6][4][3] = {{{0, 1, 2}, {5, 4, 3}, {6, 7, 8}, {3, 4, 5}},
                   {{2, 2, 2}, {5, 4, 3}, {6, 6, 6}, {3, 4, 5}},
                   {{2, 1, 0}, {5, 4, 3}, {8, 7, 6}, {3, 4, 5}},
                   {{5, 4, 3}, {8, 8, 8}, {3, 4, 5}, {0, 0, 0}},
                   {{3, 3, 3}, {3, 3, 3}, {3, 3, 3}, {3, 3, 3}},
                   {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}, {5, 5, 5}}};

int ry[6][4][3] = {{{3, 3, 3}, {11, 11, 11}, {3, 3, 3}, {3, 3, 3}},
                   {{3, 4, 5}, {2, 2, 2}, {5, 4, 3}, {6, 6, 6}},
                   {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}, {9, 9, 9}},
                   {{8, 8, 8}, {3, 4, 5}, {0, 0, 0}, {5, 4, 3}},
                   {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}},
                   {{11, 10, 9}, {8, 7, 6}, {5, 4, 3}, {2, 1, 0}}};

int isSolved(Cube c)
{
    int i, x, y;

    for (i = 0; i < 6; i++)
        for (x = cx[i] - 1; x <= cx[i] + 1; x++)
            for (y = cy[i] - 1; y <= cy[i] + 1; y++)
                if (c.g[x][y] != c.g[cx[i]][cy[i]])
                    return 0;
    return 1;
}

Cube Rotate(Cube c, int f)
{
    char t[3][3];
    int i, j, x = cx[f], y = cy[f];

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            t[i][j] = c.g[x + i - 1][y + j - 1];

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            c.g[x + i - 1][y + j - 1] = t[2 - j][i];

    for (i = 0; i < 3; i++)
        t[0][i] = c.g[rx[f][0][i]][ry[f][0][i]];

    for (j = 0; j < 3; j++)
        for (i = 0; i < 3; i++)
            c.g[rx[f][j][i]][ry[f][j][i]] =
                c.g[rx[f][j + 1][i]][ry[f][j + 1][i]];

    for (i = 0; i < 3; i++)
        c.g[rx[f][j][i]][ry[f][j][i]] = t[0][i];

    return c;
}

void printCube(Cube c)
{
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 12; j++)
            printf("%c", c.g[i][j] ? c.g[i][j] : ' ');
        printf("\n");
    }
    printf("\n");
}
void rotate(Cube & c) {
    char face = in[idx];
    bool ccw = islower(face);
    d = r[face];
    if (ccw) {
        c = Rotate(c, d);
        c = Rotate(c, d);
        c = Rotate(c, d);
    } else {
        c = Rotate(c, d); 
    }
            ++idx;
            if (idx == N) cnt++;
            idx = idx % N;
}
int main()
{
    r['U'] = r['u'] = 4;
    r['D'] = r['d'] = 5;
    r['L'] = r['l'] = 0;
    r['R'] = r['r'] = 2;
    r['F'] = r['f'] = 1;
    r['B'] = r['b'] = 3;

    Cube c;
    while (cin >> in)
    {
        N = in.length();
        memset(c.g, 0, sizeof(c.g));
        for (i = 0; i < 3; i++)
        {
            for (j = 3; j < 6; j++)
            {
                c.g[i][j] = '4';
            }
        }
        for (i = 3; i < 6; i++)
        {
            for (j = 0; j < 3; j++) c.g[i][j] = '0';
            for (j = 3; j < 6; j++) c.g[i][j] = '1';
            for (j = 6; j < 9; j++) c.g[i][j] = '2';
            for (j = 9; j < 12; j++) c.g[i][j] = '3';
        }
        for (i = 6; i < 9; i++)
        {
            for (j = 3; j < 6; j++)
            {
                c.g[i][j] = '5';
            }
        }
        cnt = 0; idx = 0;
        while(true) {
            for (int j = 0; j < N; j++) rotate(c);
            if (isSolved(c)) break;
        }
        cout << cnt << endl;
    }

    return 0;
}
