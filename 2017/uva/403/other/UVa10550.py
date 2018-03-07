'''/* UVa problem:  1125
 *
 * Topic:  other
 *
 * Level:  triv
 * 
 * Brief problem description: 
 *
 *    solve the combo lock
 *
 * Solution Summary:
 *
 *    do as told
 *
 * Used Resources:
 *
 *   cp3
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */'''
import sys
for line in sys.stdin:
    t = 1080
    a,b,c,d = [int(int(c)*360/40) for c in line.strip().split()]
    t += (a - b) + (0 if a > b else 360)
    t += c - b if c > b else c - b + 360
    t += c - d if c > d else c - d + 360
    if a == b == c == d:
        pass
    else:
        print(t)