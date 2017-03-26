'''/* UVa problem:  11044
 *
 * Topic:  other
 *
 * Level:  triv
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
 *   cp3
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */'''
for i in range(int(input())):
    a=[int(c) for c in input().split()]
    a.sort()
    print("Case " + str(i+1) + ": " + str(a[1]))