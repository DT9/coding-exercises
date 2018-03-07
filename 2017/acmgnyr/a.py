# tc = int(input())
# for i in range(tc):
#     arr = input().split()[1:]
#     arr = [int(a) for a in arr].sort()
#     print(i+1,arr[-3])


[print("{} {}".format(i+1, sorted(list(map(int, input().split(' ')[1:])))[7])) for i in range(int(input()))] # :)