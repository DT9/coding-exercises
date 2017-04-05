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

tc = int(input().strip())
for _ in range(tc):
    t = int(input().strip())
    w = [int(i) for i in input().strip().split()]
    M = 0
    for i in range(t):
        for j in range(i+1,t):
            if (w[i] > w[j]):
                M+=1
    print("Optimal train swapping takes " + str(M) + " swaps.")