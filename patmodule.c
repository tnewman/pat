#include <Python.h>
#include "pat.h"

static PyObject* pat_python_queue(PyObject* self, PyObject* args);

static PyObject* pat_python_stop(PyObject* self, PyObject* args);

static PyObject* pat_python_get_queue_len(PyObject* self, PyObject* args);

static void pat_cleanup(void);

static char* pat_map_error_to_string(PATError);

int pat_cleanup_registered = 0;

static PyMethodDef PatMethods[] = {
    {"queue", pat_python_queue, METH_VARARGS, "Queue"},
    {"get_queue_len", pat_python_get_queue_len, METH_VARARGS, "Get Queue Length"},
    {"stop", pat_python_stop, METH_VARARGS, "Stop"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef patmodule = {
    PyModuleDef_HEAD_INIT,
    "pat",
    NULL,
    -1,
    PatMethods
};

static PyObject* PatException;

PyMODINIT_FUNC PyInit_pat(void) {
    if(!pat_cleanup_registered) {
        Py_AtExit(pat_cleanup);
    }
    
    pat_init();
     
    PyObject *pat_instance = PyModule_Create(&patmodule);
    
    if(pat_instance == NULL) {
        return NULL;
    }
    
    PatException = PyErr_NewException("pat.PATException", NULL, NULL);
    Py_INCREF(PatException);
    PyModule_AddObject(pat_instance, "PATException", PatException);
    

    return pat_instance;
}

static PyObject* pat_python_queue(PyObject* self, PyObject* args) {
    const char* audio_file_path;
    
    if(!PyArg_ParseTuple(args, "s", &audio_file_path)) {
        return NULL;
    }
    
    PATError status;
    
    Py_BEGIN_ALLOW_THREADS
        status = pat_queue(audio_file_path);    
    Py_END_ALLOW_THREADS
    
    if(status != PAT_SUCCESS) {
        PyErr_SetString(PatException, pat_map_error_to_string(status));
        return NULL;
    }
    
    Py_RETURN_NONE;
}

static PyObject* pat_python_stop(PyObject* self, PyObject* args) {
    pat_stop();

    Py_RETURN_NONE;
}

static PyObject* pat_python_get_queue_len(PyObject* self, PyObject* args) {
    int queue_len = pat_get_queue_len();    
    return Py_BuildValue("i", queue_len);
}

void pat_cleanup(void) {
    pat_quit();
}

static char* pat_map_error_to_string(PATError error) {
    switch(error) {
        case PAT_NOT_INITIALIZED_ERROR:
            return "Not initialized yet.";
        case PAT_INIT_ERROR:
            return "Initialization failed.";
        case PAT_OPEN_DEVICE_ERROR:
            return "Could not open playback device.";
        case PAT_OPEN_FILE_ERROR:
            return "Could not open audio file.";
        case PAT_NO_AUDIO_STREAM_ERROR:
            return "Audio file does not contain an audio stream.";
        case PAT_CODEC_ERROR:
            return "Decoding failed.";
        case PAT_RESAMPLE_ERROR:
            return "Resampling failed.";
        case PAT_PLAYBACK_ERROR:
            return "Audio playback failed.";
        default:
            return "Unknown error.";
    }
}

