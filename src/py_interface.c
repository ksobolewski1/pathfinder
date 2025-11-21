#include "py_interface.h"

#include "core.h"
#include "rb.h"
#include "bw.h"
#include "astar.h"

#include <Python.h>
#include <structmember.h>
#include <stdio.h>
#include <stddef.h>


typedef struct {
    PyObject_HEAD
    float x;
    float y;
    int id;
    PyObject* edges;
} py_node;


static PyMemberDef node_members[] = {
    {"x", T_FLOAT, offsetof(py_node, x), 0, "x coordinate"},
    {"y", T_FLOAT, offsetof(py_node, y), 0, "y coordinate"},
    {"id", T_INT, offsetof(py_node, id), 0, "identifier"},
    {"edges", T_OBJECT_EX, offsetof(py_node, edges), 0, "ids of connected nodes"},
    {NULL}  
};

static PyObject* node_new(PyTypeObject *type, PyObject* args, PyObject* kwds)
{
    py_node *self;
    self = (py_node*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->x = 0.0;
        self->y = 0.0;
        self->id = 0;
        self->edges = PyList_New(0);  // empty list by default
        if (self->edges == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}


static void node_dealloc(py_node *self) {
    Py_XDECREF(self->edges);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyTypeObject node_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pathfinder.node",               /* tp_name */
    sizeof(py_node),               /* tp_basicsize */
    0,                              /* tp_itemsize */
    &node_dealloc,                  /* tp_dealloc */
    0,                              /* tp_vectorcall_offset */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_as_async */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    0,                              /* tp_flags */
    PyDoc_STR("py_interface objects"),        /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    node_members,                   /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    0,                              /* tp_init */
    0,                              /* tp_alloc */
    &node_new,                      /* tp_new */
};


typedef struct {
    PyObject_HEAD 
    PyObject* nodes;
} py_mesh;


static PyMemberDef mesh_members[] = {
    {"nodes", T_OBJECT_EX, offsetof(py_mesh, nodes), 0, "mesh structure"},
    {NULL}  
};


static PyObject* mesh_new(PyTypeObject *type, PyObject* args, PyObject* kwds)
{
    py_mesh *self;
    self = (py_mesh*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->nodes = PyList_New(0);  // empty list by default
        if (self->nodes == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}


static void mesh_dealloc(py_mesh* self) {
    Py_XDECREF(self->nodes);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyTypeObject mesh_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pathfinder.mesh",               /* tp_name */
    sizeof(py_mesh),                /* tp_basicsize */
    0,                              /* tp_itemsize */
    &mesh_dealloc,                              /* tp_dealloc */
    0,                              /* tp_vectorcall_offset */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_as_async */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    0,                              /* tp_flags */
    PyDoc_STR("py_interface objects"),        /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    mesh_members,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    0,                              /* tp_init */
    0,                              /* tp_alloc */
    &mesh_new,                              /* tp_new */
};


static PyObject* get_maze(PyObject* self, PyObject* args) {

    int screen_w, screen_h, start;
    float cell_size;

    if (!PyArg_ParseTuple(args, "iiif", &screen_w, &screen_h, &start, &cell_size)) {
        return NULL; 
    }

    struct mesh* m = recursive_backtrack(screen_w, screen_h, cell_size, start);

    int nc = m->node_count;

    py_mesh* mesh = PyObject_New(py_mesh, &mesh_type);
    mesh->nodes = PyList_New(nc);

    for (int i = 0; i < nc; i++) {

        struct node n = m->nodes[i];
        py_node *node = PyObject_New(py_node, &node_type);

        node->x = n.x;
        node->y = n.y;
        node->id = n.id;

        node->edges = PyList_New(n.edge_count);
        for (int j = 0; j < n.edge_count; j++) {
            PyList_SetItem(node->edges, j, PyLong_FromLong(n.edges[j]));
        }

        PyList_SetItem(mesh->nodes, i, (PyObject*)node);

    }

    free(m->nodes);
    free(m);
    
    return (PyObject*)mesh;
}


static PyObject* get_path(PyObject* self, PyObject* args) {

    PyObject* pmesh;
    int start, end;
    if (!PyArg_ParseTuple(args, "Oii", &pmesh, &start, &end)) {
        return NULL; 
    }

    PyObject* nodes_list = PyObject_GetAttrString(pmesh, "nodes");
    if (!nodes_list) {
        return NULL;
    }
    
    struct mesh m;
    m.node_count = (int)PyList_Size(nodes_list);
    m.nodes = (struct node*)malloc(sizeof(struct node) * m.node_count);

    for (int i = 0; i < m.node_count; i++) {

        PyObject* node = PyList_GetItem(nodes_list, i);
        PyObject* edges_list = PyObject_GetAttrString(node, "edges");
        if (!edges_list) {
            return NULL;
        }

        struct node n;
        n.edge_count = (int)PyList_Size(edges_list);
        n.edges = (int*)malloc(sizeof(int) * n.edge_count);

        for (int j = 0; j < n.edge_count; j++) {
            n.edges[j] = (int)PyLong_AsLong(PyList_GetItem(edges_list, j));
        }

        PyObject* xval = PyObject_GetAttrString(node, "x");
        PyObject* yval = PyObject_GetAttrString(node, "y");
        PyObject* node_id = PyObject_GetAttrString(node, "id");

        n.x = PyFloat_AsDouble(xval);
        n.y = PyFloat_AsDouble(yval);
        n.id = PyLong_AsLong(node_id);

        Py_DECREF(edges_list);
        Py_DECREF(xval);
        Py_DECREF(yval);
        Py_DECREF(node_id);

        m.nodes[i] = n;
    }

    Py_DECREF(nodes_list);

    int path_length = 0;
    int* path = find_path(&m, start, end, &path_length);

    PyObject* res = PyList_New(path_length);
    for (int i = 0; i < path_length; i++) PyList_SetItem(res, i, PyLong_FromLong(path[i]));

    free(m.nodes);
    free(path);
    
    return res;
}


static PyMethodDef path_methods[] = {
    {"get_maze", get_maze, METH_VARARGS, "Builds the maze using the recursive backtracker."},
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
    
    if (PyType_Ready(&node_type) < 0) {
        printf("Node type fails to initialise. Exit.\n");
        return 1;
    }

    if (PyType_Ready(&mesh_type) < 0) {
        printf("Mesh type fails to initialise. Exit.\n");
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


    Py_INCREF(&node_type);
    PyModule_AddObject(path_mod, "Node", (PyObject*)&node_type);

    Py_INCREF(&mesh_type);
    PyModule_AddObject(path_mod, "Mesh", (PyObject*)&mesh_type);


    return 0;
    
}

int python_end() {

    Py_Finalize();

    return 0;
}


int maze_program(const char* script_path) {

    // Open the Python maze.py script file
    FILE *fp = fopen(script_path, "r");
    if (fp == NULL) {
        printf("Failed to open the maze script. Exit.");
        return 1;
    }

    // Run the Python script
    PyRun_SimpleFile(fp, script_path);

    fclose(fp);

    return 0;
}

int nav_mesh_program(const char* script_path) {

    // run nav_mesh.py

    return 0;
}