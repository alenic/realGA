import matplotlib.pyplot as plt
import numpy as np
import math

x = np.array([math.sin(i/6) for i in range(100)])

minv = x.min()
maxv = x.max()

delta = maxv - minv

cumsum = 0
cumf = []
for i in range(100):
    cumsum += 1 - (x[i] - minv) / delta
    cumf += [cumsum]


plt.step(range(100), cumf)
plt.plot(x)
plt.show()