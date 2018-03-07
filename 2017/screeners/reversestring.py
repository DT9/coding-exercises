s = list('quick brown fox')

def reverseString(p):
    s = list(p)
    l = len(s)
    for i in range(l/2):
        temp = s[i]
        s[i] = s[l-i-1]
        s[l-i-1] = temp
        print l-i-1
    print s

reverseString(s)