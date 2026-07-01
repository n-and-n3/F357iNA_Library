# Varified
# https://judge.yosupo.jp/submission/314547
# https://atcoder.jp/contests/abc350/submissions/70012959
# https://atcoder.jp/contests/abc333/submissions/70015327

class UnionFind:
  def __init__(self,N):
    self.parent = [-1]*N
    self.groupcount = N

  def union(self,x,y):
    fx = self.find(x)
    fy = self.find(y)
    if fx != fy: # x,y の属する木が異なる
      self.groupcount -= 1
      if self.parent[fx] <= self.parent[fy]: # xの木のサイズ >= yの木のサイズ
        self.parent[fx] += self.parent[fy]
        self.parent[fy] = fx
      else:
        self.parent[fy] += self.parent[fx]
        self.parent[fx] = fy


  def find(self,x):
    f = x
    stack = []
    while self.parent[f] >= 0:
      stack.append(f)
      f = self.parent[f]
    for s in stack:
      self.parent[s] = f
    return f

  def size(self,x):
    return -self.parent[self.find(x)]

  def same(self,x,y):
    return self.find(x) == self.find(y)

  def groups(self):
    A = sorted(set(self.find(i) for i in range(len(self.parent))))
    d = {v:i for i,v in enumerate(A)}
    res = [[] for i in range(len(A))]
    for i in range(len(self.parent)):
      res[d[self.find(i)]].append(i)
    return res

  def groupsizes(self):
    return [len(g) for g in self.groups()]
