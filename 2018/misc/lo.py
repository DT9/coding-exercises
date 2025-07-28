import sys

numServ = 0
plCost = []
costArr = [[]]

def lCost(index):
	costArr = [[sys.maxint for i in range(index)] for j in range(index)]

	costArr[index-1][0] = plCost[index-1]

	for i in range(index-2, -1, -1):
		for dist in range(index-1, -1, -1):
			if dist == 0:
				costArr[i][0] = min(costArr[i+1]) + plCost[i]
			else:
				costArr[i][dist] = costArr[i+1][dist-1] + dist
	return min(costArr[0])

if __name__ == "__main__":
	numServ = int(raw_input())

	for index in range(0, numServ):
		plCost.insert(index, int(raw_input()))

	print lCost(numServ)