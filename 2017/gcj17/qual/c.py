tc = int(input())

for t in range(tc):
    n,k = input().split()
    n,k = int(n), int(k)
    b = range(n+2)
    ls,rs = n, n
    l,r=0,n
    mid = 0
    for i in range(k):
        mid = (l+r)/2
        ls = mid - 1
        lr = mid
        b[mid] = 1 


        
    print("Case #{}: {} {}".format(t+1,max(ls,rs),min(ls,rs)))
