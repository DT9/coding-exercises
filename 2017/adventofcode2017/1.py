import sys
for _ in sys.stdin:
    s = list(map(int,_.strip()))
    stack = []
    total = 0
    count = 0
    prev = -1
    for i in s:
        if prev == i:
            count+=i
        if prev != i:
            prev = i
            total += count
            count = 0
        prev = i
    b = s[-1]
    if s[0] == b:
        count+=b
    total += count
    
    print(total)