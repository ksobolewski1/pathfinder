
import random

PT_SIZE = 100

with open("bw-test", "w") as testfile:
    for i in range(PT_SIZE):
        testfile.write(str(random.uniform(0.0, 900.0)) + " " + str(random.uniform(0.0, 900.0)) + "\n")
