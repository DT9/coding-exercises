a=int(input())
for _ in range(a):
    a = int(input())
    arr = []
    b = a
    for i in range(2,101):
        while a % i == 0:
            a/=i
            arr.append(str(i))
    print(' '.join(arr))
