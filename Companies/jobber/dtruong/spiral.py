def createSpiral(N):
    spiral = [[0 for i in range(N)] for j in range(N)]
    top = left = 0
    bottom = right = N - 1
    RIGHT, DOWN, LEFT, UP = 0, 1, 2, 3
    direction = RIGHT
    count = 1
    while top <= bottom and left <= right:
        if direction == RIGHT:
            for i in range(left,right+1):
                spiral[top][i] = count
                count += 1
            top += 1
        elif direction == DOWN:
            for i in range(top,bottom+1):
                spiral[i][right] = count
                count += 1
            right -= 1
        elif direction == LEFT:
            for i in range(right,left-1,-1):
                spiral[bottom][i] = count
                count += 1
            bottom -= 1
        elif direction == UP:
            for i in range(bottom,top-1,-1):
                spiral[i][left] = count
                count += 1
            left += 1
        direction = (direction + 1) % 4
    return spiral

import argparse
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("height", type=int)
    args = parser.parse_args()

    height = args.height
    for row in createSpiral(height):
        print(row)

