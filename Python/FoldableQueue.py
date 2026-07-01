class FoldableQueue:
    def __init__(self,array):
        self.front = []
        self.rear = list(array)

    def append(self,x):
        self.rear.append(x)
    
    def popleft(self):
        if self.front:
            return self.front.pop()
        elif self.rear:
            self.normarize()
            return self.front.pop()
        else:
            raise IndexError("pop from empty list")
        
    def popleft_or(self,defualt):
        if self.front:
            return self.front.pop()
        elif self.rear:
            self.normarize()
            return self.front.pop()
        else:
            return defualt
    
    def normarize(self):
        while self.rear:
            self.front.append(self.rear.pop())
    
    def __len__(self):
        return len(self.front) + len(self.rear)
    
    def __getitem__(self,i):
        if 0 <= i <  len(self):
            if i < len(self.front):
                return self.front[~i]
            else:
                return self.rear[i-len(self.front)]
        elif -len(self) <= i < 0:
            if -len(self.rear) <= i:
                return self.rear[i]
            else:
                return self.front[~(i+len(self.rear))]
        else:
            raise IndexError("Index out of range")
        
    def __str__(self):
        return str(list(reversed(self.front))+self.rear)

def test():     
    q = Queue([])
    print(q)
    q.append(2)
    q.append(3)
    q.append(5)
    q.append(7)
    q.append(11)
    q.append(13)
    print(q)
    print(q.popleft())
    print(q)
    print(q.popleft())
    q.append(15)
    q.append(30)
    q.append(45)
    q.append(60)

    print(q)
    print(q[0])
    print(q[1])
    print(q[2])
    print(q[3])
    print(q[4])
    print(q[5])
    print(q[6])
    print(q[7])
    print(q[~0])
    print(q[~1])
    print(q[~2])
    print(q[~3])
    print(q[~4])
    print(q[~5])
    print(q[~6])
    print(q[~7])

# test()

# https://atcoder.jp/contests/abc335/submissions/72571308

N,Q = map(int,input().split())

q = FoldableQueue([(i,0) for i in range(N,0,-1)])
d = {"R":(1,0), "L":(-1,0),"U":(0,1),"D":(0,-1)}

for _ in range(Q):
    t,x = input().split()
    if t == "1":
        head = q[-1]
        new = (head[0]+d[x][0],head[1]+d[x][1])
        q.append(new)
        q.popleft()
    else:
        x = int(x)
        print(*q[~(x-1)])