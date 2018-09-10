# Drakeor's 8080 Emulator
[![Build Status](https://travis-ci.org/drakeor/drakeor-8080-emulator.svg?branch=master)](https://travis-ci.org/drakeor/drakeor-8080-emulator)

Yet another Intel 8080 emulator written in C. 
It's built to run on one of my micro controllers hence the small footprint.
I built it to run Space Invaders for a project I'm working on.

# Build Instructions
Run `./build.sh`

This will generate the executables to the build folder.

# Loading ROMS
After building the project, run

`build/8080_genrom roms/invaders`

This will generate a .h file in the roms folder. Open main.c and change the #include file to point to the new rom

# Libraries
Uses [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html) for unit testing.

# Features
TBD
