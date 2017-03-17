'''/* UVa problem: 123
 *
 * Topic: combinatorics
 *
 * Level: nontriv
 * 
 * Brief problem description: 
 *
 *   given words and titles, print the sentence
 *   with bolded keywords sorted
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
import sys
blacklist = set()
for line in sys.stdin:
    line = line.strip()
    if line == "::":
        break
    blacklist.add(line)
sent = [s.strip() for s in sys.stdin]
final = dict()
for line in sent:
    ll = line.lower().split()
    for i, word in enumerate(ll):
        if word not in blacklist:
            t = list(ll)
            t[i] = word.upper()
            if word not in final:
                final[word] = []
            final[word].append(' '.join(t))

keys = list(final.keys())
keys.sort()
for k in keys:
    for l in final[k]:
        print(l)