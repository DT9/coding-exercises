# line = input().strip().split()
thekey = 2512208
key = []
inp = "AtvthrqgseCnikg"
res="YourfriendAlice"
inp='Bjjrwkcsdwpypfwzovorswxjsvjetcezfqg'
res='Thequickbrownfoxjumpsoverthelazydog'
for i in range(len(inp)):
    a,b = inp[i],res[i]
    x,y = str.lower(a),str.lower(b)
    j,k = ord(x)-ord('a'),ord(y)-ord('a')
    c = abs(k-j)
    key.append(c)
print(key)
