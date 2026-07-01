# トポロジカルソート
# https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10895919#1

def toposort(connect):
  N = len(connect)
  indegree = [0]*N
  for i in range(N):
    for j in connect[i]:
      indegree[j] += 1
  res = []
  tmp = [i for i in range(N) if indegree[i] == 0]
  while tmp:
    v = tmp.pop()
    res.append(v)
    for j in connect[v]:
      indegree[j] -= 1
      if indegree[j] == 0:
        tmp.append(j)
  if len(res) != N:
    raise ValueError("this graph has cycle(s).")
  return res
