import math
#semi major, semi minor
a,b = list(map(int,input().split()))
a /= 2
res = math.pi*2*math.sqrt((a**2+b**2)/2)/2
lol = 0.9059514072663569
print(res*lol)

#453.08693596555906
#410.474747252
