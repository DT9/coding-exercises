n=10
def fib(n=10):
    if n <= 1:
        return n
    return fib(n-1) + fib(n-2)
print fib(n)

memo = [0,1]
def fib1(n=10):
    for i in range(2,n+1):
        memo.append(memo[i-1] + memo[i-2])
    return memo[n]
print fib1(n)
def fast_fibonacci(n):
    if n > 0:
        m = [1, 1]
        i = 1
    else:
        return 0
    while i <= n/2:
        m = [m[0] * (m[1] * 2 - m[0]),
            m[1]**2 + m[0]**2]
        i *= 2
    while i < n:
        m = [m[1], m[0]+m[1]]
        i += 1
    return m[0]
print fast_fibonacci(n)
def fibonacci(n):
    m = [0, 1]
    for i in range(2,n+1):
        m = [m[1], m[0]+m[1]] 
    return m[1]
print fibonacci(n)
def fibonacci(n):
    a,b = 1,1
    for i in range(n-1):
        a,b = b,a+b
    return a
print fibonacci(n)