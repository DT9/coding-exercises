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
    total = 720
    a,b,c,d = [int(c) for c in line.strip().split()]
    
    print(total)