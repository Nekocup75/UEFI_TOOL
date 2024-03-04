import binascii
import io
import os

File1 = "a.bin"
File2 = "MM.bin"
Offset = 0xF20000
WriteSize = 0xE0000
OutPutFile = "c.bin"

def main():
    with open(File1, "rb") as f:
        a_bin = f.read()

    with open(File2, "rb") as f:
        b_bin = f.read()

    a_bin_size = len(a_bin)
    b_bin_size = len(b_bin)

    a_bin = a_bin[:Offset] + b_bin[Offset:Offset+WriteSize] + a_bin[Offset+WriteSize:]

    with open(OutPutFile, "wb") as f:
        f.write(a_bin)

if __name__ == "__main__":
    main()