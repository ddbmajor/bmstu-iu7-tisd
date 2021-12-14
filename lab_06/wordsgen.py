from os import read
import random
import sys

f = open("in.txt", "w")

abc = "abcdefghkjlmnoptxyz"

k = int(sys.argv[1])

for i in range(k - 1):
    word = []
    for j in range(random.randint(1, 6)):
        word.append(random.choice(abc))
    word.append("\n")
    f.write("".join(word))
f.write("sdcsdc\n")
f.close()