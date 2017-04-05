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
c = 0
while True:
    c+=1
    a = int(input())
    if a == 0:
        break
    b = [int(i) for i in input().split()]
    s = 0
    for i in b:
        if i > 0:
            s+=1
        else:
            s-=1
    print("Case "+str(c)+": "+str(s))