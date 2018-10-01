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
coords_3 = read_coords("hw1data_3.csv")
coords_4 = read_coords("hw1data_4.csv")
coords_5 = read_coords("hw1data_5.csv")
coords_6 = read_coords("hw1data_6.csv")
coords_7 = read_coords("hw1data_7.csv")
coords_8 = read_coords("hw1data_8.csv")
coords_9 = read_coords("hw1data_9.csv")
coords_10 = read_coords("hw1data_10.csv")
coords_11 = read_coords("hw1data_11.csv")



def take_average(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11):
    average = []
    for i in range(len(l1)):
        average.append((l1[i] + l2[i] + l3[i] + l4[i] +l5[i] + l6[i] +l7[i] + l8[i] +l9[i] + l10[i] +l11[i])/11)
    return average

coords = take_average(coords_1, coords_2, coords_3, coords_4, coords_5, coords_6, coords_7, coords_8, coords_9, coords_10, coords_11)

import matplotlib.pyplot as plt

xint = list(range(10,30))

plt.xticks(xint)
plt.title("Access time in nanoseconds vs. buffer size")

plt.plot(list(range(10,30)), coords)

plt.xlabel("Object Size Log base 2")
plt.ylabel("Time in microseconds")
plt.show()
