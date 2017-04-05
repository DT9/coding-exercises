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
pyg = 'ay'

original = raw_input('Enter a word:')
if len(original) > 0 and original.isalpha():
    word = original.lower()
    first = word[0]
    if first == ('a' or 'e' or 'i' or 'o' or 'u'):
        new_word = word + pyg
        print new_word
    else:
        new_word = word[1:] + first + pyg
        print new_word
else:
    print 'empty'
