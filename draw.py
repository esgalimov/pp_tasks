import numpy as np
import matplotlib.pyplot as plt
import pathlib

p = pathlib.Path(__file__).parent / "out"

lines = []

for name in p.iterdir():
    with open(str(name), "r") as file:
        new_lines = file.readlines()
        if lines == []:
            lines = new_lines
            continue

        for i in range(len(lines)):
            lines[i] += new_lines[i] 

for i in range(len(lines)):
    lines[i] = [float(x) for x in lines[i].split()]

#print(lines)

k = [i for i in range(len(lines[0]))]


fig, ax = plt.subplots(figsize=(8,10))

for i in range(len(lines)):
    #ax.scatter(k, lines[i])
    ax.plot(k, lines[i], markersize=1.5, marker="o", linewidth=1,)

ax.grid(which='major')
ax.minorticks_on()
ax.grid(which='minor', linestyle=':')
ax.set_title("Итерации")

plt.savefig("png.png")
plt.show()