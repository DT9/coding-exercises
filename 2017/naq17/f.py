h = raw_input().split()
a,b = int(h[0]),int(h[1])
n = int(input())
inp = [raw_input() for _ in range(n)]

def lol(inp,a,b,n):
    fx,fy = 0,1
    x,y = 0,0
    for i in range(n):
        d = inp[i]
        if d == 'Forward':
            x += fx
            y += fy
        elif d == 'Right':
            if fx == 0 and fy == 1: #north
                fx,fy=1,0
            elif fx == 1 and fy == 0: #east
                fx,fy=0,-1
            elif fx == 0 and fy == -1: #south
                fx,fy=-1,0
            elif fx == -1 and fy == 0: #west
                fx,fy=0,1
        elif d == 'Left':
            if fx == 0 and fy == 1: #north
                fx,fy=-1,0
            elif fx == 1 and fy == 0: #east
                fx,fy=0,1
            elif fx == 0 and fy == -1: #south
                fx,fy=1,0
            elif fx == -1 and fy == 0: #west
                fx,fy=0,-1
    if x == a and y == b:
        return True
    return False

for t in ['Left','Forward','Right']:
    for j in range(n):
        lmao = inp[:]
        lmao[j] = t
        if lol(lmao,a,b,n):
            print(str(j+1)+' '+t)