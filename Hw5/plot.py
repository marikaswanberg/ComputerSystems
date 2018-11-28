def read_y_coords(filename):
    y_coords = []
    with open(filename, "r") as file:
        s  = file.readlines()

        for i in range(len(s)):
            coord = s[i].split(", ")
            coord = coord[1].split("\n")[0]
            coord = float(coord)
            y_coords.append(coord)
    return y_coords

def read_x_coords(filename):
    x_coords = []
    with open(filename, "r") as file:
        s  = file.readlines()

        for i in range(len(s)):
            coord = s[i].split(", ")
            coord = coord[0].split("\n")[0]
            coord = float(coord)
            x_coords.append(coord)
    return x_coords

items_in_cache = read_x_coords("latency_per_num_items.csv")
mean_latency = read_y_coords("latency_per_num_items.csv")


import matplotlib.pyplot as plt
plt.title("Mean Response Time vs. Items in Cache")

plt.plot(items_in_cache, mean_latency)

plt.xlabel("Number of Items in Cache")
plt.ylabel("Mean Response Time (microseconds)")
plt.show()
