import matplotlib.pyplot as plt
import numpy as np
import math

def roulette(cumf, value):
    index = 0
    print(value)
    for i in range(len(cumf)):
        if value > cumf[i]:
            index = i+1
        else:
            break
    return index


n = 3
x = np.array([10, 20, 30])

#x = np.array([math.sin(math.pi*i/(100-1)) for i in range(100)])

shuffle_ind = np.arange(n)
#np.random.shuffle(shuffle_ind)

x = x[shuffle_ind]

minv = x.min()
maxv = x.max()
delta = maxv - minv
#x = (x-minv)/delta

x_sum = x.sum()
x = x / x_sum
cumsum = 0
cumf = []
for i in range(n):
    perc = x[i]
    cumsum += perc
    print(perc)
    cumf += [cumsum]
cumf = np.array(cumf)
print(cumf)

index = np.arange(10000)

for i in range(10000):
    cumulative1 = cumf.min() + np.random.rand()*(cumf.max()-cumf.min())
    index[i] = roulette(cumf, cumulative1)
print(index)


plt.figure()
plt.step(range(n), cumf)
plt.plot(x)
plt.figure()
plt.hist(index, bins=n+1)
plt.show()