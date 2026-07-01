# 区間和①：https://atcoder.jp/contests/practice2/submissions/69407123
# 区間和②：https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10881162#2
# 区間min：https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10881147#2
# SegmentTree (抽象化非再帰) Ver2

import operator

class SegmentTree:  # 0-indexed
  def __init__(self, array, func, e):
      N = 1<<((len(array)-1).bit_length())
      self.N = N
      self.func = func
      self.e = e
      self.array = [e]*(2*N)
      for i in range(len(array)):
        self.array[N+i] = array[i]
      for i in range(N-1,0,-1):
        self.array[i] = self.func(self.array[2*i],self.array[2*i+1])
      
  def write(self,i,x): # 値の上書き
    ii = i + self.N
    self.array[ii] = x
    while ii > 1:
      self.array[ii>>1] = self.func(self.array[ii&(-2)],self.array[ii|1])
      ii >>= 1
    
  def reduce(self,l,r): # 半開区間[l,r)でのfunc
    resL,resR = self.e,self.e
    l += self.N
    r += self.N
    while l<r:
      if l&1:
        resL = self.func(resL,self.array[l])
        l += 1
      if r&1:
        r -= 1
        resR = self.func(self.array[r],resR)
      l >>= 1
      r >>= 1
    return self.func(resL,resR)
        
  
  def get(self,i):
    return self.array[i+self.N]
  
  def add(self,i,x):
    self.write(i,self.get(i)+x)

# =========================================================
