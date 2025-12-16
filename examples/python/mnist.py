import realga as rg
import numpy as np
import torch
from torchvision.datasets import MNIST
import torchvision.transforms as T
from torch.utils.data import DataLoader, Subset
from typing import List, Optional


class SoftmaxLoss(rg.FitnessFunction):
    def __init__(
        self,
        train_subset: int = 2048,  # keep GA eval fast
        test_subset: int = 2048,
        batch_size: int = 256,
        seed: int = 0,
        use_gpu: bool = True,
    ):
        super().__init__()

        self.device = "cuda"

        # 28x28 -> 8x8 because model is Linear(8*8, 10)
        train_tr = T.Compose(
            [
                T.ToTensor(),
                T.Normalize((0.1307,), (0.3081,)),
                T.Lambda(lambda x: x.flatten()),
            ]
        )
        val_tr = train_tr

        self.ds_train = MNIST(".", train=True, download=True, transform=train_tr)

        self.ds_test = MNIST(".", train=False, download=True, transform=val_tr)

        # fixed subsets for deterministic eval speed
        g = torch.Generator().manual_seed(seed)
        train_idx = torch.randperm(len(self.ds_train), generator=g)[
            :train_subset
        ].tolist()
        test_idx = torch.randperm(len(self.ds_test), generator=g)[:test_subset].tolist()
        self.train_subset = Subset(self.ds_train, train_idx)
        self.test_subset = Subset(self.ds_test, test_idx)

        self.train_loader = DataLoader(
            self.train_subset,
            batch_size=batch_size,
            shuffle=False,
            num_workers=4,
        )
        self.test_loader = DataLoader(
            self.test_subset,
            batch_size=batch_size,
            shuffle=False,
            num_workers=4,
        )

        self.criterion = torch.nn.CrossEntropyLoss(reduction="mean")

    def _make_model(self) -> torch.nn.Module:
        return torch.nn.Linear(28 * 28, 10).to(self.device)

    def get_params(self, model: Optional[torch.nn.Module] = None) -> List[float]:
        # Flatten all parameters into a single python list[float]
        if model is None:
            model = self._make_model()
        with torch.no_grad():
            flat = torch.cat(
                [p.detach().view(-1).cpu() for p in model.parameters()], dim=0
            )
        return flat.tolist()

    def set_params(self, model: torch.nn.Module, params: List[float]) -> None:
        # Write params back into model parameters (in the same order as model.parameters()).
        with torch.no_grad():
            src = torch.tensor(params, dtype=torch.float32, device=self.device)
            offset = 0
            for p in model.parameters():
                n = p.numel()
                p.copy_(src[offset : offset + n].view_as(p))
                offset += n

    def eval(self, g: rg.RealChromosome) -> float:
        # Fitness to minimize: mean CE loss on a small train subset.
        model = self._make_model()
        self.set_params(model, g.gene)

        model.eval()
        total_loss = 0.0
        total_n = 0

        with torch.no_grad():
            for x, y in self.train_loader:
                x = x.to(self.device, non_blocking=True)
                y = y.to(self.device, non_blocking=True)
                logits = model(x)
                loss = self.criterion(logits, y)
                bs = y.size(0)
                total_loss += float(loss.item()) * bs
                total_n += bs

        return total_loss / max(1, total_n)

    def accuracy_on_test(self, params: List[float]) -> float:
        model = self._make_model()
        self.set_params(model, params)
        model.eval()

        correct = 0
        total = 0
        with torch.no_grad():
            for x, y in self.test_loader:
                x = x.to(self.device, non_blocking=True)
                y = y.to(self.device, non_blocking=True)
                pred = model(x).argmax(dim=1)
                correct += int((pred == y).sum().item())
                total += int(y.numel())
        return correct / max(1, total)


# ----------------- GA setup -----------------
torch.manual_seed(0)
np.random.seed(0)

myFitnessFunction = SoftmaxLoss(train_subset=2048, test_subset=2048, batch_size=256)

chromosome_size = len(myFitnessFunction.get_params())

options = rg.RealGAOptions()
options.setChromosomeSize(chromosome_size)
options.setSelectionType("roulette")
options.setPopulationSize(80)
options.setBounds(
    (-0.2 * np.ones(chromosome_size)).tolist(),
    (0.2 * np.ones(chromosome_size)).tolist(),
    )
options.setVerbose(1)
options.setMutationType("uniform")
options.setMutationUniformPerc(0.1, 0.001)
options.setElitismFactor(0.1)
options.setSeed(21)

ga = rg.RealGA()
ga.init(options, myFitnessFunction, False)
ga.popInitRandGaussian(0, 0.01)

# ---- Evolution loop ----
for gen in range(40):
    ga.evolve()

    chromosome = ga.getBestChromosome()
    train_loss = chromosome.fitness
    test_acc = myFitnessFunction.accuracy_on_test(chromosome.gene)

    pop = ga.getPopulation()
    for k, c in enumerate(pop):
        if k >= 10:
            break
        print(list(c.gene)[:10], c.fitness)

    print(
        f"Gen {gen:02d} | Train Loss: {train_loss:.4f} | Test Acc (subset): {test_acc*100:.2f}%"
    )
