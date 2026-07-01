def CartesianTree(X):
    N = int(len(X))
    stack = []
    left = [-1]*N
    right = [-1]*N
    for i in range(N):
        stack.append(i)
        while len(stack) >= 2 and X[stack[-2]] > X[stack[-1]]:
            if left[stack[-1]] != -1:
                right[stack[-2]] = left[stack[-1]]
            left[stack[-1]] = stack[-2]
            stack[-1] = stack.pop()
    while len(stack) >= 2:
        right[stack[-2]] = stack[-1]
        stack.pop()

    root = stack.pop()

    par = [-1]*N
    for p in range(N):
        if left[p] != -1:
            par[left[p]] = p
        if right[p] != -1:
            par[right[p]] = p
    
    return root, left, right, par