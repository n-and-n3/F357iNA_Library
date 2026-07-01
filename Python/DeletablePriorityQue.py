class DeletablePriorityQue:
  def __init__(self,array,reverse = False): #デフォルトはminを取得
    self.array = [None] + array
    for i in range(len(self.array)//2-1,-1,-1):
      self._appropriate(i)
    self.maltiset = {}
    for v in array:
      if v in self.maltiset:
        self.maltiset[v] += 1
      else:
        self.maltiset[v] = 1
    self.reverse = reverse

  def pop(self):
    res = self.array[1]
    self.romove(res)
    return res
  
  def _appropriate(self,p):
    while 2*p+1 < len(self.array): #子ノード2つともが存在する
      mc = 2*p + (self.array[2*p] > self.array[2*p+1] if not self.reverse else self.array[2*p] < self.array[2*p+1])
      if (self.array[p] > self.array[mc] if not self.reverse else self.array[p] < self.array[mc]):
        self.array[p],self.array[mc] = self.array[mc],self.array[p]
        p = mc
      else:
        break
    if 2*p+1 == len(self.array) and (self.array[p] > self.array[2*p] if not self.reverse else self.array[p] < self.array[2*p]): #一人っ子の場合 and 入れ替える必要がある
      self.array[p],self.array[2*p] = self.array[2*p],self.array[p]
  
  def append(self,x):
    self.array.append(x)
    if x in self.maltiset:
      self.maltiset[v] += 1
    else:
      self.maltiset[v] = 1
    p = len(self.array)-1
    while p > 1 and (self.array[p] < self.array[p//2] if not self.reverse else self.array[p] > self.array[p//2]):
      self.array[p],self.array[p//2] = self.array[p//2],self.array[p]
      p //= 2
    
  def remove(self,x):
    if x not in self.maltiset:
      raise KeyError(f" {x}")
    self.maltiset[x] -= 1
    if self.maltiset[x] == 0:
      del self.maltiset[x]
    while self.array[1] not in self.maltiset:
      if len(self.array) != 2:
        self.array[1] = self.array.pop()
      else:
        self.array.pop()
      p = 1
      self._appropriate(p)

  def discard(self,x):
    if x in self.maltiset:
      self.remove(x)
  
  def get(self):
    return self.array[1]

  def __len__(self):
    return len(array)-1
  
  def __bool__(self):
    return len(self) != 0

# =============================================
"""
Q = int(input())

pq = PriorityQue([])
for _ in range(Q):
  query = input().split()
  if query[0] == "1":
    pq.append(int(query[1]))
  elif query[0] == "2":
    print(pq.get())
  elif query[0] == "3":
    pq.pop()
  #print(pq.array[1:])
"""
    