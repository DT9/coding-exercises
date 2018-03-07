import string
alphabet = list(string.ascii_lowercase)

encrypted_message = input().split()
key = [8,2,5,1,2,2,0]
k = 0
klen = 7
res = []

for word in encrypted_message:
    w = ""
    for letter in word:
        if not str.isalpha(letter):
            w+=letter
        else:
            iscap = letter.istitle()
            letterpos = alphabet.index(str.lower(letter))
            decryptPos = (letterpos-key[k])%26
            decryptedLetter = alphabet[decryptPos]
            if iscap:
                decryptedLetter = str.upper(decryptedLetter)
            k = (k+1)%klen
            w+=decryptedLetter
    res.append(w)
print(" ".join(res))