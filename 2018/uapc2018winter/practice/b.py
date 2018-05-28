import sys
for line in sys.stdin:
    a,b=list(map(int,line.split()))
    upper = (2**31) -1
    c = a+b
    if c>upper:
        print('overflow')
    else:
        print(a+b)
