"""
File to, from an inputted file, output an array in c++ initializer form.
"""

import os
from sys import argv, exit

if len(argv) != 2:
    print("Usage: array_gen.py <filename>")
    exit(1)

if not os.path.exists(argv[1]):
    print("Cannot open file. Does not exist.")
    exit(1)

f = open(argv[1], "rb")
data = list(f.read())

print("byte program[] = {")

for byte in data:
    form_byte = format(byte, "#010b")   # Convert to 8 bit binary with leading zeros (8 bits plus 2 characters for 0b = 10)
    print(f"  {form_byte},")

print("};")