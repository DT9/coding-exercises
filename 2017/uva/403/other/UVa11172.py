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
    a,b = [int(j) for j in input().split()]
    if a > b:
        print(">")
    elif a < b:
        print("<")
    else:
        print("=")