cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

project(emu8080 C)

add_executable(emu8080 main.c disasm.c cpu.c)
add_executable(emu8080_tests test_main.c disasm.c munit.c)
add_executable(emu8080_romgen romgen.c cpu.c)
