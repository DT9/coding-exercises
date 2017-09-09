
import sys, bisect
d,r,t = [int(i) for i in sys.stdin]
partial = [3]
for i in range(1,1000):
    partial[i] = partial[i-1] + 1
ti = 3
ts = 3
while ts < t:
    ts+=ti+1
    ti+=1
