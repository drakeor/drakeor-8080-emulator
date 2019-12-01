cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

project(emu8080 C)

add_executable(emu8080 main.c disasm.c cpu.c vram.c)
add_executable(emu8080_tests test/cpu_process_test.c test/cpu_test.c test/disasm_test.c test_main.c disasm.c cpu.c vram.c test/munit.c)
add_executable(emu8080_romgen romgen.c cpu.c vram.c)
