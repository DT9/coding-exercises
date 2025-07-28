# reverse array
arr = [1,2,3]

def reverse(arr):
    # swap elements from left to right
    res = []
    # append elements in reverse order
    n = len(arr)
    for i in range(n//2):
        j = n - i - 1
        arr[j],arr[i]=arr[i],arr[j]
        print(arr)
    return arr

# res = reverse(arr)

# whitebox
# functional
# unit
# acceptance
# smoke

# 249racecar043


def palindrome(string):
    pali = set()
    # bruteforce
    n = len(string)
    for i in range(n):
        for j in range(n):
            s = string[i:j]
            if len(s) > 1 and s == s[::-1]:
                pali.add(s)
    return pali

res = palindrome("249racecar0aa43")
print(res)