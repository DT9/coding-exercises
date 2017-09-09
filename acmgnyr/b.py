tc = int(input())

def rec(arr):
    f = 0
    l = len(arr)
    for i in range(l):
        f = sum(arr[0:i])
        g = i + 1
        while g < l:

            # get next equal sum
            # if reach end 
            b = -1
            for j in range(g,l):
                b = sum(arr[g:j])
                if (f == b):
                    g = j
                    break
            
    return f
for c in range(tc):
    i, t = list(map(int, input().split(' ')))
    arr = list(map(int, input().split(' ')))
    wow = sum(arr)
    equal = rec(arr)
    print(c+1,equal)