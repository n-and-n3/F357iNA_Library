import math

class Arg:
    __slots__ = ['x', 'y', 'rank']

    def __init__(self, x, y):
        if x == 0 and y == 0:
            raise ValueError("Arg(0,0) is Undefined")
        self.x = x
        self.y = y
        if y < 0:
            self.rank = 0
        elif y == 0 and x > 0:
            self.rank = 1
        elif y > 0:
            self.rank = 2
        else:
            self.rank = 3

    def __lt__(self, other):
        if self.rank != other.rank:
            return self.rank < other.rank
        return self.x * other.y > other.x * self.y

    def __eq__(self, other):
        return self.rank == other.rank and self.x * other.y == other.x * self.y

    def __le__(self, other):
        if self.rank != other.rank:
            return self.rank < other.rank
        return self.x * other.y >= other.x * self.y

    def __gt__(self, other):
        if self.rank != other.rank:
            return self.rank > other.rank
        return self.x * other.y < other.x * self.y

    def __ge__(self, other):
        if self.rank != other.rank:
            return self.rank > other.rank
        return self.x * other.y <= other.x * self.y

    def __ne__(self, other):
        return not self.__eq__(other)

    def to_float(self):
        return math.atan2(self.y, self.x)

    def __repr__(self):
        return f"Arg({self.x}, {self.y})"

# =====================================================================================================

def test():   
    array = [Arg(-1,-1), Arg(0,-1), Arg(1,-1), Arg(1,0), Arg(1,1), Arg(0,1), Arg(-1,1), Arg(-1,0)]
    print(*[a.to_float() for a in array])

    for i in range(8):
        for j in range(i,8):
            if not( array[i] <= array[j]):
                print(array[i], array[j])


        
N,Q = map(int,input().split())

Mos = []
for i in range(N):
    a,b = map(int,input().split())
    Mos.append((Arg(a,b),i+1))


Mos.sort()

indexs = [-1]*(N+1)
prev_ind = -(N+1)
for i in range(N):
    t = Mos[i]
    if i == 0 or Mos[i-1][0] != t[0]:
        indexs[t[1]] = i
        prev_ind = i
    else:
        indexs[t[1]] = prev_ind


indexs_rev = [-1]*(N+1)
prev_ind = -(N+1)
for i in range(N-1,-1,-1):
    t = Mos[i]
    if i == N-1 or Mos[i+1][0] != t[0]:
        indexs_rev[t[1]] = i
        prev_ind = i
    else:
        indexs_rev[t[1]] = prev_ind

#print(Mos)
#print(indexs)
#print(indexs_rev)
#print([t[0].to_float() for t in Mos])

for i in range(Q):
    A,B = map(int,input().split())
    A_rank = indexs[A]
    B_rank = indexs[B]
    #print("Debug", A_rank, B_rank)
    if indexs[A] == indexs[B]:
        print(indexs_rev[A]-indexs[A]+1)
    elif indexs[B] < indexs[A]:
        print(indexs_rev[A]-indexs[B] + 1)
    else:
        print(N - (indexs_rev[B]-indexs[A] + 1)+ (indexs_rev[A]-indexs[A]+1) + (indexs_rev[B]-indexs[B]+1))






