'''/* UVa problem: 272
 *
 * Topic: adhoc
 *
 * Level: triv
 * 
 * Brief problem description: 
 *
 *   replace all double quotes to tex style quotes
 *
 * Solution Summary:
 *
 *   replace
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
tilde = True
for i in sys.stdin:
    line = i.strip()
    for l in line:
        if l == '"':
            if tilde:
                line = line.replace('"',"``",1)
                tilde = False
            else:
                line = line.replace('"',"''",1)
                tilde = True
    print(line)