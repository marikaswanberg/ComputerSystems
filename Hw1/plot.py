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

python_coords = read_coords("csv_files/Python_Results.csv")
c_double_coords = read_coords("csv_files/C_Double_Results.csv")
c_float_coords = read_coords("csv_files/Float_Results.csv")
c_int8_coords = read_coords("csv_files/C_int8_t_Results.csv")
c_int16_coords = read_coords("csv_files/C_int16_t_Results.csv")
c_int32_coords = read_coords("csv_files/C_int32_t_Results.csv")
c_int64_coords = read_coords("csv_files/C_int64_Results.csv")


import matplotlib.pyplot as plt
# plt.title("Microbenchmark Results")
# plt.plot(list(range(8,25)), c_float_coords, label="C Float")
# plt.plot(list(range(8,25)), c_double_coords, label="C Double")
# plt.plot(list(range(8,25)), c_int8_coords, label="C int8_t")
# plt.plot(list(range(8,25)), c_int16_coords, label="C int16_t")
# plt.plot(list(range(8,25)), c_int32_coords, label="C int32_t")
# plt.plot(list(range(8,25)), c_int64_coords, label="C int64_t")
plt.plot(list(range(8,25)), python_coords, label="Python")

plt.xlabel("Object Size Log base 2")
plt.ylabel("Time in microseconds")
plt.legend(loc=4)
plt.show()
