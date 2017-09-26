tc = int(input())

# combinatorics
#closed form sol
for _ in range(2):
    n = list(map(int,input().split()))
    m = list(map(int,input().split()))
    #find cycles of groups
    #find gcd
    a = []
    for i in range(tc):
        