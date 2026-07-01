# MexMultiSet
# 追加：$O(1)$
# 削除：$O(\log N)$ (最悪)
# mexの取得：$O(\log N)$ (償却)
# https://atcoder.jp/contests/abc330/submissions/69931793

from collections import defaultdict
import heapq as hq

class MexMultiSet:
  L = 10**4
  
  def __init__(self,array):
    M = max(array)
    self.mulS = defaultdict(int)
    for x in array:
        self.mulS[x] += 1
    array_ = [i for i in range(MexMultiSet.L) if self.mulS[i] == 0]
    array_.append(MexMultiSet.L)
    hq.heapify(array_)
    self.q = array_
    self.inf = MexMultiSet.L    # infの値
    self.length = len(array)
  
  def add(self,x):
    self.mulS[x] += 1
    self.length += 1

  def remove(self,x):
    if self.mulS[x] > 0:
      self.mulS[x] -= 1
      self.length -= 1
      if x < self.inf and self.mulS[x] == 0:
        hq.heappush(self.q, x)
    else:
      raise KeyError(str(x))
      
  def discard(self,x):
    if self.mulS[x] > 0:
      self.mulS[x] -= 1
      self.length -= 1
      if x < self.inf and self.mulS[x] == 0:
        hq.heappush(self.q, x)

  def mex(self):
    while self.mulS[self.q[0]] > 0:
      if hq.heappop(self.q) == self.inf:
        for i in range(self.inf,self.inf+MexMultiSet.L):
          if self.mulS[i] == 0:
            self.q.append(i)
        self.q.append(self.inf+MexMultiSet.L)
        self.inf = self.inf+MexMultiSet.L
    return self.q[0]
  
  def __len__(self):
    return self.length

  def __contains__(self, x):
      return self.mulS[x] > 0
