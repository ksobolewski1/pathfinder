#include <stdio.h>
#include <stdlib.h>

#include "py_interface.h"

int main(int argc, char *argv[]) {

    python_start();

    maze_program();

    python_end();

    return 0;
}