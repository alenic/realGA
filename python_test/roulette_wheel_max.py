import matplotlib.pyplot as plt
import numpy as np
import math

def roulette(cumf, value):
    index = 1
    for i in range(len(cumf)):
        if value > cumf[i]:
            index = i+1
        else:
            break
    return index % (len(cumf)-1)



x = np.array([4, 5, 1, -2])

#x = np.array([math.sin(math.pi*i/(100-1)) for i in range(100)])

shuffle_ind = np.arange(len(x))
#np.random.shuffle(shuffle_ind)

x = x[shuffle_ind]

minv = x.min()
maxv = x.max()
x = x/x.sum()
x = x-x.min()
x = np.hstack([x, x[0]])
#delta = maxv - minv
#x = (x-minv)/delta # between 0 and 1
print(x)
n = len(x)
cumsum = 0
cumf = []
for i in range(n):
    perc = x[i]
    cumsum += perc
    cumf += [cumsum]
cumf = np.array(cumf)
print(cumf)

index = np.arange(10000)

for i in range(10000):
    cumulative1 = np.random.rand()*cumf.max()
    index[i] = roulette(cumf, cumulative1)

for i in range(n):
    print(i, ":", (index==i).sum())


plt.figure()
plt.step(range(n), cumf)
plt.plot(x)
plt.figure()
plt.hist(index, bins=n+1)
plt.show()
