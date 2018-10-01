def read_coords(filename):
    y_coords = []
    with open(filename, "r") as file:
        s  = file.readlines()

        for i in range(len(s)):
            coord = s[i].split(", ")
            coord = coord[1].split("\n")[0]
            coord = float(coord)
            y_coords.append(coord)
    return y_coords

coords_1 = read_coords("hw1data.csv")
coords_2 = read_coords("hw1data_2.csv")

def take_average(list1, list2):
    average = []
    for i in range(len(list1)):
        average.append((list1[i] + list2[i])/2)
    return average
coords = take_average(coords_1, coords_2)

import matplotlib.pyplot as plt

plt.plot(list(range(10,30)), coords)

plt.xlabel("Object Size Log base 2")
plt.ylabel("Time in microseconds")
plt.show()
