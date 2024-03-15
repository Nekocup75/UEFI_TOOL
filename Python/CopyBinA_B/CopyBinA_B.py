import io

File1 = "a.bin"
InsertFile = "mod.bin"
Offset = 0xF20000
WriteSize = 0xE0000
OutputFile = "Output.bin"

def main():
    with open(File1, "rb") as f:
        File1Buf = f.read()

    with open(InsertFile, "rb") as f:
        InsertFileBuf = f.read()

    File1Buf = File1Buf[:Offset] + InsertFileBuf[Offset:Offset+WriteSize] + File1Buf[Offset+WriteSize:]

    with open(OutputFile, "wb") as f:
        f.write(File1Buf)

if __name__ == "__main__":
    main()