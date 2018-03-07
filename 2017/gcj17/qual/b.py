tc = int(input())

def isNonIncreasing(l):
    for i, el in enumerate(l[1:]):
        if int(el) >= int(l[i-1]):
            return False
    return True
def s(n):
    for i in range(1,len(str(n))):
        if int(n[i-1]) > int(n[i]):
            return False
    return True

for i in range(tc):
    lp = int(input())
    j = lp
    while True:
        if s(str(j)):
            print("Case #{}: {}".format(i+1,j))
            break
        else:
            j = j - (j % 10) - 1