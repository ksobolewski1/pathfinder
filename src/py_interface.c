#include "py_interface.h"

#include "core.h"

#include <Python.h>
#include <stdio.h>


struct mesh* mesh;
int* path;
int path_len;


static PyObject* get_mesh(PyObject* self, PyObject* args) {

    int nc = mesh->node_count;

    PyObject *node_list = PyList_New(nc);
    if (!node_list) {
        printf("Failed to create Python list. Exit.\n");
        Py_RETURN_NONE;
    }

    for (int i = 0; i < nc; i++) {

        struct node n = mesh->nodes[i];
    }
    
    Py_RETURN_NONE;
}

static PyObject* get_path(PyObject* self, PyObject* args) {
    
    Py_RETURN_NONE;
}

static PyMethodDef path_methods[] = {
    {"get_mesh", get_mesh, METH_VARARGS, "Builds either type of mesh."},
    {"get_path", get_path, METH_VARARGS, "Finds a path between start and end nodes."},
    {NULL, NULL, 0, NULL} 
};

static struct PyModuleDef path_module = {
    PyModuleDef_HEAD_INIT,
    "pathfinder",  
    "Module bridging C and python functions.", 
    -1,  
    path_methods
};


int python_start() {

    Py_Initialize();

    if (!Py_IsInitialized()) {
        printf("Could not initialize Python interpreter. Exit.\n");
        return 1;
    }

    PyObject* path_mod = PyModule_Create(&path_module);
    if (!path_mod) {
        printf("Could not create the pathfinder module. Exit.\n");
        Py_Finalize();
        return 1;
    }

    PyObject* sys_modules = PyImport_GetModuleDict();
    if (!sys_modules) {
        printf("Could not access sys.modules. Exit.\n");
        Py_DECREF(path_mod);
        Py_Finalize();
        return 1;
    }

    if (PyDict_SetItemString(sys_modules, "pathfinder", path_mod) < 0) {
        fprintf(stderr, "Failed to register pathfinder in sys.modules\n");
        Py_DECREF(path_mod);
        Py_Finalize();
        return 1;
    }

    // create the py_interface

    return 0;
    
}

int python_end() {

    Py_Finalize();

    return 0;
}


int maze_program(struct mesh* m, int* p, int plen) {

    mesh = m;
    path = p;
    path_len = plen;

    // Open the Python script file
    const char* scr_path = "/home/kubaby/work/path/maze.py";
    FILE *fp = fopen(scr_path, "r");
    if (fp == NULL) {
        printf("Failed to open the maze script. Exit.");
        return 1;
    }

    // Run the Python script
    PyRun_SimpleFile(fp, scr_path);

    fclose(fp);

    return 0;
}

int nav_mesh_program() {

    // run nav_mesh.py

    return 0;
}