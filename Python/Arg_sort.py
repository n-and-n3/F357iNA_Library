# 参考文献：https://nebocco.hatenablog.com/entry/2021/11/13/185816、というかほぼコピペです...
# 提出：https://atcoder.jp/contests/abc442/submissions/72740147

from functools import cmp_to_key

def area(p):
    x, y = p[0], p[1]
    if y < 0:
        return 3
    elif x < 0:
        return 2
    else:
        return 1


def arg_cmp(p, q):
    ap = area(p)
    aq = area(q)
    if ap < aq:
        return -1
    elif ap > aq:
        return 1
    else:
        px, py = p[0], p[1]
        qx, qy = q[0], q[1]
        z = px * qy - py * qx
        if z > 0:
            return -1
        elif z < 0:
            return 1
        else:
            return 0

def sample(array):
    return array.sort(key=cmp_to_key(arg_cmp))  ## タプルに対して、第0成分をx、第1成分をyとして、偏角ソート (-180,180] を行う。



# ========================================

N,Q = map(int,input().split())


Mos = []
for i in range(N):
    a,b = map(int,input().split())
    Mos.append((a,b,i+1))



Mos.sort(key=cmp_to_key(arg_cmp))
indexs = [-1]*(N+1)

for i in range(N):
    t = Mos[i]
    if i == 0 or arg_cmp(Mos[i-1],Mos[i]) != 0:
        indexs[t[2]] = i
        prev_ind = i
    else:
        indexs[t[2]] = prev_ind


indexs_rev = [-1]*(N+1)
for i in range(N-1,-1,-1):
    t = Mos[i]
    if i == N-1 or arg_cmp(Mos[i],Mos[i+1]) != 0:
        indexs_rev[t[2]] = i
        prev_ind = i
    else:
        indexs_rev[t[2]] = prev_ind

#print(Mos)
#print(indexs)
#print(indexs_rev)
#print([math.atan2(t[1],t[0]) for t in Mos])

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






