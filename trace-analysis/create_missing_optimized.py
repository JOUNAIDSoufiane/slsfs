import os

files = []
sizes = []

for line in open("missing_files.txt", "r"):
    splitted = line[:-1].split(" ")
    files.append(splitted[0])
    sizes.append(float(splitted[1]))

directories = {}

for index, filename in enumerate(files):
    root_dir = "test_dir"
    DIR = filename[0:3]
    C = filename[3:4]
    path = filename[0:3] + "/" + filename[3:4]

    if DIR not in directories:
        directories[DIR] = {}
        directories[DIR][C] = 0
        os.mkdir(root_dir + "/" + DIR)
        os.mkdir(root_dir + "/" + path)
    elif C not in directories[DIR]:
        directories[DIR][C] = 0
        os.mkdir(root_dir +  "/" + path)

    name = path + "/" + filename[4:] 

    with open(root_dir + "/" + name, "w") as file:
        for i in range(int(sizes[index]) - 1):
            file.write("0");
        file.write("\n")
