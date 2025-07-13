import os
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

# Directory containing CSV files (adjust if needed)
csv_dir = "../build/debug_csv"
# Find all population CSV files
csv_files = sorted(
    [f for f in os.listdir(csv_dir) if f.startswith("pop_") and f.endswith(".csv")],
    key=lambda x: int(x.split("_")[1].split(".")[0]),
)


class PopulationPlotter:
    def __init__(self, csv_files, csv_dir):
        self.csv_files = csv_files
        self.csv_dir = csv_dir
        self.index = 0
        # Set the figure size (width, height) in inches
        self.fig, self.ax = plt.subplots(figsize=(8, 6))
        plt.subplots_adjust(bottom=0.2)
        self.scatter = None
        self.title = self.ax.set_title("")
        self.plot_population()

        # Buttons
        axprev = plt.axes([0.6, 0.05, 0.15, 0.075])
        axnext = plt.axes([0.8, 0.05, 0.15, 0.075])
        self.bnext = Button(axnext, "Next")
        self.bprev = Button(axprev, "Previous")
        self.bnext.on_clicked(self.next)
        self.bprev.on_clicked(self.prev)

    def plot_population(self):
        import numpy as np

        csv_path = os.path.join(self.csv_dir, self.csv_files[self.index])
        df = pd.read_csv(csv_path)
        self.ax.clear()
        # Plot contour of 10*(x-1)**2 + (y-1)**2 - np.cos(X) ** 2 - np.sin(Y) ** 2
        x = np.linspace(-10, 10, 200)
        y = np.linspace(-10, 10, 200)
        X, Y = np.meshgrid(x, y)
        Z = 0.1 * (X - 1) ** 2 + 0.1 * (Y - 1) ** 2 - np.cos(X) ** 2 - np.sin(Y) ** 2
        contour = self.ax.contour(X, Y, Z, levels=20, cmap="Reds", alpha=0.5)
        # Scatter plot
        self.ax.scatter(df["x0"], df["x1"], c="blue")
        self.ax.set_xlabel("x0")
        self.ax.set_ylabel("x1")
        self.ax.set_xlim(-10, 10)
        self.ax.set_ylim(-10, 10)
        self.ax.set_title(f"Population {self.index} ({self.csv_files[self.index]})")
        self.fig.canvas.draw_idle()

    def next(self, event):
        if self.index < len(self.csv_files) - 1:
            self.index += 1
            self.plot_population()

    def prev(self, event):
        if self.index > 0:
            self.index -= 1
            self.plot_population()


if __name__ == "__main__":
    if not csv_files:
        print("No population CSV files found.")
    else:
        plotter = PopulationPlotter(csv_files, csv_dir)
        plt.show()
