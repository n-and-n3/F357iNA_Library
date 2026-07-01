# エラトステネスの篩
# https://yukicoder.me/submissions/1125247
def Eratosthenes(n):
	table = [True]*(n+1)
	primes = []
	
	table[0],table[1] = False,False
	
	for i in range(2,n+1):
		if table[i]:
			primes.append(i)
			for j in range(2*i,n+1,i):
				table[j] = False
	
	return table, primes