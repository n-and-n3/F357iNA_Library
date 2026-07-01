from functools import cmp_to_key
from math import gcd

def convex(array):
    N = len(array)
    if N <= 2:
        return list(range(N))
    
    mx,my,mi = float("inf"), float("inf"), -1
    for i,t in enumerate(array):
        x,y = t
        if x < mx or (x == mx and y <= my):
            mx = x
            my = y
            mi = i
    
    P = [(t[0]-mx,t[1]-my,i) for i,t in enumerate(array) if i != mi]

    def comarg(a,b): # a > b -> +1, a == b -> 0, a < b -> -1
        outer = a[0]*b[1] - a[1]*b[0]
        if outer > 0:
            return -1
        elif outer < 0:
            return +1
        else:
            la = a[0]**2+a[1]**2
            lb = b[0]**2+b[1]**2
            if la < lb:
                return -1
            elif la > lb:
                return +1
            else:
                return 0

    P.sort(key=cmp_to_key(comarg))

    stack = [(0,0,mi)]
    for t in P:
        stack.append(t)
        while len(stack) >= 4:
            ax = stack[-3][0] - stack[-2][0]
            bx = stack[-1][0] - stack[-2][0]
            ay = stack[-3][1] - stack[-2][1]
            by = stack[-1][1] - stack[-2][1]
            outer = ax*by - ay*bx
            if outer >= 0:
                stack[-1] = stack.pop()
            else:
                break
    
    return stack



N = int(input())
array = [tuple(map(int,input().split())) for _ in range(N)]

res = convex(array)
M = len(res)
res += [res[0]]

online = 0
for i in range(M):
    online += gcd(res[i+1][0]-res[i][0], res[i+1][1]-res[i][1])

S2 = 0
for i in range(1,M-1):
    S2 += (res[i][0]-res[0][0])*(res[i+1][1]-res[0][1]) - (res[i][1]-res[0][1])*(res[i+1][0]-res[0][0])

ans = (S2+online+2)//2-N

print(ans)





