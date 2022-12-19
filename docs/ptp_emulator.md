# PTP Emulator Specification

The emulators organisation will determine the bytecode instrutions chosen.

The emulator will only run a single Pseudo Tools Bytecode file, with a ".ptb" file format. This removes the requirement for synchronisation & concurrency. This can be looked at at a future date.

The emulator will have a contiguous array of 1 byte elements, but the size will be given as a parameter to the start-up of the emulator, allowing for more memory to be given to the running.

The stack will grow from the top down starting at the highest memory address. The heap will grow from the lowest address.

There will be byte addressing, and the memory space is organised as big endian.

For example: Assume the emulator has 5 bytes of memory. `CMOV 100 &3` is the instruction to move the constant `100` into the 4th byte in memory. The memory array now looks like this:

0x0000006400

`RMOV &3 &4` is a reference move and will copy the contents from memory of the 4th byte into the 5th.

0x0000006464

## Registers

Stack Pointer - SP

Input Store - IS

## Instruction Set

### EXIT

- `EXIT A` - Exit interpreter with code A

### MOV

- `CMOV A &B` - Move constant A into contents of B
- `RMOV &A &B` - Move contents of A into contents of B

### _PMEM()

- Dump contents of memory to std out