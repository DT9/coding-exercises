def nextVersion(current_version):
    versions = list(map(int,current_version.split('.')))
    carry = 0
    versions[-1] += 1
    for i in range(len(versions)-1,0,-1):
        x = versions[i] + carry
        carry = x // 10
        versions[i] = x % 10
    if carry:
        versions[0] += carry
    result = '.'.join(map(str,versions))
    return result

import argparse
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("version", type=str)
    args = parser.parse_args()

    version = args.version
    ver = nextVersion(version)
    print(ver)

