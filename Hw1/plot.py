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

python_coords = read_coords("Python_Results.csv")
c_double_coords = read_coords("C_Double_Results.csv")
c_float_coords = read_coords("Float_Results.csv")
c_int8_coords = read_coords("C_int8_t_Results.csv")


import matplotlib.pyplot as plt
plt.title("Microbenchmark Results")
plt.plot(list(range(8,25)), c_float_coords, label="C Float")
plt.plot(list(range(8,25)), c_double_coords, label="C Double")
plt.plot(list(range(8,25)), c_int8_coords, label="C int8_t")
plt.legend(loc=4)
plt.show()
