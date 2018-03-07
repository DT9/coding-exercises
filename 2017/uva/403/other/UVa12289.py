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
 
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */'''
w = ["one","two","three"]
for i in range(int(input())):
    t = input().strip()
    m = 0 
    k = 0
    for idx,v in enumerate(w):
        b = list(set(v)-set(t))
        print(b)
        cnt = len(b)
        if cnt < m:
            m = cnt
            k = idx
    print(k+1)
