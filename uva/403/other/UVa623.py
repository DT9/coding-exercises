'''/* UVa problem: 623
 *
 * Topic: arithmetic
 *
 * Level: nontrivial
 * 
 * Brief problem description: 
 *
 *   given X, output X!
 *
 * Solution Summary:
 *
 *   python has an optimized factorial function
 *
 * Used Resources:
 *
 *   py
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */'''
import math
from sys import stdin
for line in stdin:
    n = int(line)
    print(line.strip()+"!")
    print(math.factorial(n))
