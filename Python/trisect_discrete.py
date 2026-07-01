# 3分探索 (定義域が離散)
# https://atcoder.jp/contests/typical90/submissions/69577625

def trisect_min(array,l=None,r=None,f = lambda x:x):
  if l == None:
    l = 0
  if r == None:
    r = len(array)-1
  while r - l > 2:
    mid1 = (l*2+r)//3
    mid2 = (l+r*2)//3
    if f(array[mid1]) >= f(array[mid2]):
      l = mid1
    else:
      r = mid2
  return min([f(array[i]) for i in range(l,r+1)])

def trisect_max(array,l=None,r=None,f = lambda x:x):
  if l == None:
    l = 0
  if r == None:
    r = len(array)-1
  while r - l > 2:
    mid1 = (l*2+r)//3
    mid2 = (l+r*2)//3
    if f(array[mid1]) <= f(array[mid2]):
      l = mid1
    else:
      r = mid2
  return max([f(array[i]) for i in range(l,r+1)]) 
