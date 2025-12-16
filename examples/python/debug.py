import realga as rg
import matplotlib.pyplot as plt
import numpy as np


class QuadraticFitness(rg.FitnessFunction):
    def eval(self, g: rg.RealChromosome):
        return g.gene[0] * g.gene[0] + g.gene[1] * g.gene[1]


myFitnessFunction = QuadraticFitness()

options = rg.RealGAOptions()
options.setChromosomeSize(2)
options.setPopulationSize(50)
options.setBounds([-5.0, -5.0], [5.0, 5.0])
options.setVerbose("soft")
options.setMutationType("gaussian")
options.setMutationGaussianPerc(0.01, 0.001)

ga = rg.RealGA()
ga.init(options, myFitnessFunction, False)
ga.popInitRandUniform()

# ---- Matplotlib setup ----
plt.ion()  # interactive mode
fig, ax = plt.subplots()
ax.set_xlim(-5, 5)
ax.set_ylim(-5, 5)
ax.set_title("GA population")
ax.set_xlabel("x")
ax.set_ylabel("y")

# initial empty scatter
scatter = ax.scatter([], [])

# ---- Evolution loop ----
for gen in range(40):
    ga.evolve()

    pop = ga.getPopulation()
    pop_values = np.array([[c.gene[0], c.gene[1]] for c in pop])

    scatter.set_offsets(pop_values)

    ax.set_title(f"Generation {gen}")
    plt.pause(0.5)  # allows GUI update

plt.ioff()
plt.show()
