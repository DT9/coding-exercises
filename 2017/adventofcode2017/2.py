import sys
total = 0
for line in sys.stdin:
    line = list(map(int,line.split()))
    total += max(line) - min(line)
print(total)