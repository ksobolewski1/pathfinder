#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "rb.h"
#include "py_interface.h"

int main(int argc, char *argv[]) {

    int SCREEN_WIDTH = 500;
    int SCREEN_HEIGHT = 500;
    float NODE_SIZE = 250.0f;
    const char* VENV = "/home/kubaby/venv";

    python_start();

    // return start and end too
    int start, end;
    struct mesh* m = recursive_backtrack(SCREEN_WIDTH, SCREEN_HEIGHT, NODE_SIZE, &start, &end);
    // check if null
    // int path_len;
    // int* path = astar(nodes, &path_len);

    maze_program(m, NULL, 0);

    // if nav_mesh
        // start interactive - build obstacle course, add nodes etc.

    python_end();

    return 0;
}