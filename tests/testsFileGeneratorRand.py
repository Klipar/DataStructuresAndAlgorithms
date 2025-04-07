import csv
import random

inp = [""]

size = 100000000

with open('tests/data/name.csv', mode='r', newline='', encoding='utf-8') as file:
    name = list(csv.reader(file))
with open('tests/data/surname.csv', mode='r', newline='', encoding='utf-8') as file:
    surname = list(csv.reader(file))

# for i in range(0, size):
#     j = i%4

#     if (j == 0 or j == 1):  # i
#         inp.append(f"i {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n")
#     elif (j == 2):          # s
#         inp.append(f"s {i}\n")
#     else:                   # d
#         inp.append(f"d {i}\n")


# random.shuffle(inp)

tmp = [""]

for i in range(0, size):
    tmp.append(f"i {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

for i in range(0, size):
    tmp.append(f"s {i}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

for i in range(0, size):
    tmp.append(f"r {i}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

with open(f'tests/tests/test_{size}.txt', 'w', encoding='utf-8') as file:
    for item in inp:
        file.write(item)
