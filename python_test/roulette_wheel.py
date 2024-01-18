import matplotlib.pyplot as plt
import numpy as np
import math

def roulette(cumf, value):
    index = 0
    for i in range(len(cumf)):
        if cumf[i] <= value:
            index = i
        else:
            break
    
    return index

x = np.array([math.sin(math.pi*i/(100-1)) for i in range(100)])
np.random.shuffle(x)
minv = x.min()
maxv = x.max()

delta = maxv - minv

cumsum = 0
cumf = []
for i in range(100):
    cumsum += 1 - (x[i] - minv) / delta
    cumf += [cumsum]

index = []
for i in range(1000):

    cumulative1 = cumsum * np.random.rand()

    index += [roulette(cumf, cumulative1)]
plt.figure()
plt.step(range(100), cumf)
plt.plot(x)
plt.figure()
plt.hist(index, bins=10)
plt.show()