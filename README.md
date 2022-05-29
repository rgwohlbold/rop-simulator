# ROP Simulator

The purpose of this program is to teach about Return-Oriented Programming.

## Usage

Build the program using `make`. Run the program using `./sim`. The program overrides its own stack using the information provided, starting at the return address of `main`.

There are three types of tokens you can enter:

1. Gadgets like `2`. This will place the 8-byte address of the gadget on the stack.
2. Unsigned constants like `$0xdeadbeef` with (hexadecimal). This will place the 8-byte constant on the stack.
3. Strings like `"hello, world"`. This will put a copy of the string on the heap and place a pointer on the stack.

## Tasks

These tasks should be increasing in difficulty.

1. Print the help text
2. Open a shell using `system()`
3. Open a shell and exit with exit code 0
4. Open a shell using `execve` and exit with exit code 0
5. Open a shell without using any of the provided addresses
