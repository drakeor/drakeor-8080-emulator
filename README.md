# Drakeor's 8080 Emulator
[![Build Status](https://travis-ci.org/drakeor/drakeor-8080-emulator.svg?branch=master)](https://travis-ci.org/drakeor/drakeor-8080-emulator)
[![Build status](https://ci.appveyor.com/api/projects/status/i08x5bxqyr53cyk9?svg=true)](https://ci.appveyor.com/project/drakeor/drakeor-8080-emulator)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a6b0c7e331404e80b7986656ad0395a1)](https://www.codacy.com/app/drakeor.dragon/drakeor-8080-emulator?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=drakeor/drakeor-8080-emulator&amp;utm_campaign=Badge_Grade)

Yet another Intel 8080 emulator written in C.

I tried to build it to not segfault or crash if an 8080 program mis-behaves.

It's built to run on one of my micro controllers hence the small footprint.

I built it to run Space Invaders for a project I'm working on.

# Build Instructions
Tested to work on Ubuntu 16.04 / 18.04.
Run `./build.sh`

Run `/build/emu8080_tests` to insure all tests pass
This will generate the executables to the build folder.

# Loading ROMS
After building the project, run

`build/emu8080_genrom roms/invaders`

This will generate a .h file in the roms folder. Open main.c and change the #include file to point to the new rom

# Libraries
Uses [MUnit](https://github.com/nemequ/munit) for unit testing.

# Reference Links
[Intel 8080 Opcodes](http://nemesis.lonestar.org/computers/tandy/software/apps/m4/qd/opcodes.html)

[Intel 8080 Instruction Set](http://www.pastraiser.com/cpu/i8080/i8080_opcodes.html)

[intel 8080 Opcode card](http://www.cmas-net.org.uk/vintage/Ref-Cards/MCS85-Intel-8085-8080-Ref-Card-July-1977.pdf)

[Space Invaders ROM](https://github.com/ezet/i8080-emulator/tree/master/invaders%20rom)


# Features
TBD
