import numpy as np
import time

def Measure(f):
    def Handle(*a, **b):
        t0 = int(time.time())
        optionPrice = f(*a, **b)
        t1 = int(time.time())
        print(f"Python took {t1 - t0} seconds to run")
        return optionPrice
    return Handle

@Measure
def Tree(S, K, r, q, v, t, steps, optype):
    dt = t / steps
    row = 4*steps + 2
    column = steps + 1

    tree = np.zeros((row, column)).tolist()
    center = int(row / 2 - 1)

    up = np.exp(v*np.sqrt(2.0*dt))
    dn = 1.0 / up
    m = 1.0

    A = np.exp((r - q)*dt/2.0)
    B = np.exp(-v*np.sqrt(dt/2.0))
    C = np.exp(v*np.sqrt(dt/2.0))

    pu = pow((A - B)/(C - B), 2)
    pd = pow((C - A)/(C - B), 2)
    pm = 1.0 - (pu + pd)

    for j in range(column):
        tree[center][j] = S
        for i in range(1, column - j):
            tree[center - 2*i][i + j] = tree[center - 2*(i-1)][i - 1 + j]*up
            tree[center][i + j] = tree[center][i - 1 + j]*m
            tree[center + 2*i][i + j] = tree[center + 2*(i-1)][i - 1 + j]*dn

    for i in range(row):
        if i % 2 != 0:
            if optype == 0:
                tree[i][column - 1] = np.max([tree[i-1][column - 1] - K, 0.0])
            else:
                tree[i][column - 1] = np.max([K - tree[i-1][column - 1], 0.0])

    inc = 2
    for j in range(2, column + 1):
        for i in range(inc, row - inc):
            if i % 2 != 0:
                A = tree[i-2][column-j+1]
                B = tree[i][column-j+1]
                C = tree[i+2][column-j+1]
                cash = pu*A + pm*B + pd*C
                cash = np.exp(-r*dt)*cash
                if optype == 0:
                    tree[i][column-j] = np.max([tree[i-1][column-j] - K, cash])
                else:
                    tree[i][column-j] = np.max([K - tree[i-1][column-j], cash])
        inc += 2

    return tree[center + 1][0]


S = 100
K = 90
r = 0.05
q = 0.01
v = 0.25
t = 30.0/365.0
steps = 1500
optype = 1

optionPrice = Tree(S, K, r, q, v, t, steps, optype)
print("Python option price is ", optionPrice)
print(f"Python took {steps} steps")


