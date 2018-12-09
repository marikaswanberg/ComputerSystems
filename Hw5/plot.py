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

reqs_per_sec = read_x_coords("sustained_throughput2.csv")
mean_latency = read_y_coords("sustained_throughput2.csv")


import matplotlib.pyplot as plt
plt.title("Mean Response Time vs. Requests Per Seconds")

plt.plot(reqs_per_sec, mean_latency)

plt.xlabel("Number of Requests Per Second")
plt.ylabel("Mean Response Time (microseconds)")
plt.xscale('log', basex=2)
plt.show()
