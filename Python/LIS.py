# LIS(最長増加部分列)
# https://atcoder.jp/contests/tessoku-book/submissions/69561043

import bisect

def LIS(array, broad:bool = False, INF = float("inf"), NINF = float("-inf")):
  N = len(array)
  table = [INF]*(N+1)  # 長さi以下の単調増加数列の末項の最小値
  table[0] = NINF
  for x in array:
    i =  bisect.bisect_right(table, x) if broad else bisect.bisect_left(table, x)
    table[i] = x
  return bisect.bisect_left(table, INF)-1