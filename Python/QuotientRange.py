# https://yukicoder.me/submissions/1134484
# 内部実装の類似を利用：https://atcoder.jp/contests/arc160/submissions/70358315


import math

def quotient_range(N):
    ans = [] # (l,r,v) := x in [l,r) の区間において、N//x = z である。
    sqrtN = math.isqrt(N)
    m = N
    for i in range(1,sqrtN+1):
        ans.append((i,i+1,N//i))
        m = N//i
    for i in range(m-1,0,-1):
        L = N//(i+1)+1
        R = N//i+1
        if L < R:
          ans.append((L,R,i))
    return ans

def quotient_range_generator(N):
    sqrtN = math.isqrt(N)
    m = N
    for i in range(1,sqrtN+1):
        yield (i,i+1,N//i)
        m = N//i
    for i in range(m-1,0,-1):
        L = N//(i+1)+1
        R = N//i+1
        if L < R:
          yield (L,R,i)

def main():
    N = 5
    print(quotient_range(N))
    print(*[N//i for i in range(1,N+1)])
    print()
    N = 9
    print(quotient_range(N))
    print(*[N//i for i in range(1,N+1)])
    print()
    N = 15
    print(quotient_range(N))
    print(*[N//i for i in range(1,N+1)])
    print()

main()
