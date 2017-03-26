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
while True:
    tc = int(input())
    if tc == 0:
        break
    x,y = [int(j) for j in input().split()]
    arr = []
    for i in range(tc):
        a,b = [int(j) for j in input().split()]
        if a == x or b == y:
            print("divisa")
        elif a > x and b > y:
            print("NE")
        elif a < x and b > y:
            print("NO")
        elif a > x and b < y:
            print("SE")
        else:
            print("SO")