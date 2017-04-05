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
import sys
c = 0
for line in sys.stdin:
    line = line.strip()
    c += 1
    ans = "Case " + str(c) + ": "
    if (line == "#"):
        break
    elif line == "HELLO":
        ans += ("ENGLISH")
    elif line == "HOLA":
        ans += ("SPANISH")
    elif line == "HALLO":
        ans += ("GERMAN")
    elif line == "BONJOUR":
        ans += ("FRENCH")
    elif line == "CIAO":
        ans += ("ITALIAN")
    elif line == "ZDRAVSTVUJTE":
        ans += ("RUSSIAN")
    else:
        ans += ("UNKNOWN")
    print(ans)