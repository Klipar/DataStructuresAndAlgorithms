import csv
import random

inp = [""]
out = [""]
size = 10000000

with open('tests/data/name.csv', mode='r', newline='', encoding='utf-8') as file:
    name = list(csv.reader(file))
with open('tests/data/surname.csv', mode='r', newline='', encoding='utf-8') as file:
    surname = list(csv.reader(file))

for i in range(0, size):
    strInp = ""
    strOut = ""

    strInp += f"s {i}\n"
    strOut += "ERROR\n"
    strInp += f"r {i}\n"
    strOut += "ERROR\n"
    strInp += f"i {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n"
    strOut += "\n"
    strInp += f"s {i}\n"
    strOut += f"i {i} {name[i%len(name)][1]} {surname[i%len(surname)][1]}\n"
    strInp += f"r {i}\n"
    strOut += "\n"
    strInp += f"s {i}\n"
    strOut += "ERROR\n"
    strInp += f"r {i}\n"
    strOut += "ERROR\n"
    strInp += f"i {i}\n"
    strOut += "ERROR\n"

    inp.append(strInp)
    out.append(strOut)

combined = list(zip(inp, out))

random.shuffle(combined)

with open(f'tests/tests/Inp_{size}.txt', 'w', encoding='utf-8') as file:
    for item in combined:
        file.write(item[0])

with open(f'tests/tests/Out_{size}.txt', 'w', encoding='utf-8') as file:
    for item in combined:
        file.write(item[1])