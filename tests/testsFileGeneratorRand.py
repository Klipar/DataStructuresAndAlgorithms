import csv
import random
import os

directory = 'tests/tests'

inp = [""]

size = 10000000

with open('tests/data/name.csv', mode='r', newline='', encoding='utf-8') as file:
    name = list(csv.reader(file))
with open('tests/data/surname.csv', mode='r', newline='', encoding='utf-8') as file:
    surname = list(csv.reader(file))

tmp = [""]

for i in range(0, size):
    tmp.append(f"i {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

for i in range(0, size):
    tmp.append(f"s {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

for i in range(0, size):
    tmp.append(f"r {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n")

random.shuffle(tmp)
inp += tmp
tmp = [""]

if not os.path.exists(directory):
    os.makedirs(directory)

with open(f'{directory}/test_{size}.txt', 'w', encoding='utf-8') as file:
    for item in inp:
        file.write(item)
