f = open(file,'r')
total = 0
for i in f.readlines():
    total += int(i)
print total