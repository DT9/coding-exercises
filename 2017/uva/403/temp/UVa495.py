'''/* UVa problem:
 *
 * Topic:
 *
 * Level:
 *
 * Brief problem description:
 *
 *
 *
 * Solution Summary:
 *
 *
 *
 * Used Resources:
 *
 *
 *
 * I hereby certify that I have produced the following solution myself
 * using the resources listed above in accordance with the CMPUT 403
 * collaboration policy.
 *
 * --- Dennis Truong
 */'''
import sys
fibs = [0, 1, 1]
def fib(n):
    for f in range(2, n):
        fibs.append(fibs[-1] + fibs[-2])
    return fibs[n]

fib(5000)
for i in sys.stdin:
    a = int(i.strip())
    print("The Fibonacci number for " 
    + str(a) + " is " + str(fibs[a]))
