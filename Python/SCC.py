# SCC(強連結成分分解)
# https://q.trap.jp/messages/01998415-33dc-7b4b-9d36-e52830d73640

def scc(connect):
  N = len(connect)
  orderlist = []

  # 1回目DFS
  c = 0
  visited = [0]*N    # 0:未到達　1:探索中(連結した点をスタックに入れた) 2:探索済み(スタックに入れた要素を使い果たした)
  for x in range(N):
    if visited[x] == 2:
      continue
    visited[x] = 1
    stack = [x]
    while stack:
      v = stack.pop()
      if v >= 0 and visited[v] == 2:
        continue
      if v < 0:
        c += 1
        orderlist.append(~v)
        visited[~v] = 2
        continue
      stack.append(~v)
      visited[v] = 1
      for u in connect[v]:
        if visited[u] == 0:
          stack.append(u)

  # 逆向きグラフの構成
  connect_inv = [[] for _ in range(N)]
  for v in range(N):
    for u in connect[v]:
      connect_inv[u].append(v)


  # 2回目DFS
  c = 0
  visited = [False]*N # False:未探索 True:訪問済み
  label = [-1]*N
  for x in orderlist[::-1]:
    if visited[x]:
      continue
    visited[x] = True
    stack = [x]
    while stack:
      v = stack.pop()
      if label[v] != -1:
        continue
      visited[v] = True
      label[v] = c
      for u in connect_inv[v]:
        if not visited[u]:
          stack.append(u)
    c += 1

  # 最終出力
  tps = [[] for _ in range(c)]
  for i in range(N):
    tps[label[i]].append(i)

  return label, tps 