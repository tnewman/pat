#include <Python.h>
#include "pat.h"

static PyObject* pat_python_play(PyObject* self, PyObject* args);

static void pat_cleanup(void);

int pat_cleanup_registered = 0;

static PyMethodDef PatMethods[] = {
    {"play", pat_python_play, METH_VARARGS, "Play"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef patmodule = {
    PyModuleDef_HEAD_INIT,
    "pat",
    NULL,
    -1,
    PatMethods
};

PyMODINIT_FUNC PyInit_pat(void) {
    if(!pat_cleanup_registered) {
        Py_AtExit(pat_cleanup);
    }
    
    pat_init();
        
    return PyModule_Create(&patmodule);
}

static PyObject* pat_python_play(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

void pat_cleanup(void) {
    pat_quit();
}
