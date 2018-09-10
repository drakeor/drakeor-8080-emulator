#include <stdlib.h>
#include <stdio.h>
#include "minunit.h"
#include "disasm.h"

int tests_run = 0;

char blank_data[2] = { 0x00, 0x00 };

static char* test_op_to_text() {
    int pc = 0;
    mu_assert("error, 0x00 should return zero", op_to_text(blank_data, &pc) == 0);
    return 0;
}

static char* test_disassembly() {
    mu_assert("error, sample program should return zero", disassemble(blank_data, 2) == 0);
    return 0;
}

static char* run_all_tests() {
    mu_run_test(test_op_to_text);
    mu_run_test(test_disassembly);
    return 0;
}

int main(int argc, char** argv) {
    char* res = run_all_tests();
    if(res != 0) {
        printf("%s\n", res);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return res != 0;
}
