N = 100

primes = []
lpf = [-1]*(N+1)

for i in range(2,N+1):
  if lpf[i] == -1:
    lpf[i] = len(primes)
    primes.append(i)
  for j in range(lpf[i]+1):
    if i * primes[j] <= N:
      lpf[i * primes[j]] = j
    else:
      break


print(primes)
print(lpf)
print([primes[ind] if ind != -1 else -1 for ind in lpf])

# ===================================================================

N = 100

primes = []
mpf = [-1]*(N+1)

for i in range(2,N+1):
  if mpf[i] == -1:
    mpf[i] = len(primes)
    primes.append(i)
  for j in range(mpf[i]+1):
    if i * primes[j] <= N:
      mpf[i * primes[j]] = mpf[i]
    else:
      break
  

print(primes)
print(mpf)
print([primes[ind] if ind != -1 else -1 for ind in mpf])