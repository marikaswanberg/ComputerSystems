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

reqs_old = read_x_coords("retime_sustained_throughput.csv")
mean_latency_old = read_y_coords("retime_sustained_throughput.csv")

reqs_new = read_x_coords("new_sustained_throughput.csv")
mean_latency_new = read_y_coords("new_sustained_throughput.csv")


import matplotlib.pyplot as plt
plt.title("Mean Response Time vs. Requests Per Second")

plt.plot(reqs_old, mean_latency_old, label="old FIFO")
plt.plot(reqs_new, mean_latency_new, label="new FIFO")


plt.xlabel("Requests Per Second")
plt.ylabel("Mean Response Time (microseconds)")
plt.xscale('log', basex=2)
plt.legend(loc=2)
plt.show()
