y_coords = []
with open("Python_Results.csv", "r") as file:
    x  = file.readlines()

for i in range(len(x)):
    coord = x[i].split(", ")
    coord = coord[1].split("\n")[0]
    coord = float(coord)
    y_coords.append(coord)


import matplotlib.pyplot as plt
plt.title("Python Results")
plt.plot(list(range(8,25)), y_coords)
plt.show()
