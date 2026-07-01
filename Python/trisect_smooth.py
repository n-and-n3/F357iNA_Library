# 三分探索 (定義域が連続)
# https://atcoder.jp/contests/abc426/submissions/69886926


def trisect_min(l,r,f,epsilon = 1e-7):
  while r - l > epsilon:  # これ繰り返し回数の上限があったほうがいいかも？
    mid1 = (l*2+r)/3
    mid2 = (l+r*2)/3
    if f(mid1) >= f(mid2):
      l = mid1
    else:
      r = mid2
  return f(l)