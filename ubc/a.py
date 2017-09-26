tc = int(input())
#http://www.cs.ubc.ca/~acm-web/practice/2017-09-09/problems/a.pdf
#closed form sol
for i in range(tc):
    n,pizza = list(map(int,input().split()))
    #ans[n] = min(ans[n-i]+ans[i]))
    #min(min(ans[n-i], ans[i]))