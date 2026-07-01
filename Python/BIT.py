# BIT
# https://atcoder.jp/contests/practice2/submissions/69400835

class BIT:
  def __init__(self,N):   # 1-indexed
    self.array = [0]*(N+1)
    self.N = N
  
  def add(self,i,x):
    while i <= self.N:
      self.array[i] += x
      i += i&(-i)
    
  def sum(self,i):
    s = 0
    while i > 0:
      s += self.array[i]
      i -= i&(-i)
    return s