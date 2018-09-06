y_coords = []
with open("results.csv", "r") as file:
    x  = file.readlines()

for i in range(len(x)):
    print(x[i])
    coord = x[i].split(", ")
    coord = coord[1].split("\n")[0]
    coord = float(coord)
    y_coords.append(coord)

print(y_coords)


import matplotlib.pyplot as plt

plt.plot(list(range(8,25)), y_coords)
plt.show()
