import re
import sys
def my_split(s):
    return filter(None, re.split(r'(\d+)', s))
for line in sys.stdin:
    s = my_split(line)
    print(s)