# https://judge.yosupo.jp/submission/320861

def is_prime(n):
    if n <= 1:
        return False
    elif n == 2:
        return True
    elif n % 2 == 0:
        return False

    s =  ((n - 1) & -(n - 1)).bit_length() - 1
    d = (n - 1) // (1 << s)

    if n < 4759123141:
      A = [2 ,7 ,61]
    elif n <= 2**64:
      A = [2, 7, 61, 325, 9375, 28178, 450775, 9780504, 1795265022]
    else:
      raise ValueError("Too Large!")

    for a in A:
        if n <= a:
          return True
        x = pow(a,d,n)
        if x != 1:
            for t in range(s):
                if x == n - 1:
                    break
                x = (x*x)%n
            else:
              return False
    return True
