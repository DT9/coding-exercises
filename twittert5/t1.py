import dateutil.parser as d
start = d.parse(input())
end = d.parse(input())
num = int(input())
for _ in range(num):
    line = input()
    if line[0].isalpha():
        print(line)
        continue
    iso,msg = line.split("\t",1)
    dt = d.parse(iso)
    if dt >= start and dt < end:
        print(line)