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

static PyObject* PATException;

PyMODINIT_FUNC PyInit_pat(void) {
    if(!pat_cleanup_registered) {
        Py_AtExit(pat_cleanup);
    }
    
    pat_init();
     
    PyObject *pat_instance = PyModule_Create(&patmodule);
    
    if(pat_instance == NULL) {
        return NULL;
    }
    
    PATException = PyErr_NewException("PATException", NULL, NULL);
    Py_INCREF(PatException);
    
    return pat_instance;
}

static PyObject* pat_python_play(PyObject* self, PyObject* args) {
    const char* audio_file_path;
    
    if(!PyArg_ParseTuple(args, "s", &audio_file_path)) {
        return NULL;
    }
    
    if(pat_play(audio_file_path) != PAT_SUCCESS) {
        PyErr_SetString(PatException, "PAT could not play back audio!");
        return NULL;
    }
    
    Py_RETURN_NONE;
}

void pat_cleanup(void) {
    pat_quit();
}
