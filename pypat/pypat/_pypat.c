#define PY_SSIZE_T_CLEAN
#include <pat/pat.h>
#include <pat/pat_error.h>
#include <Python.h>

static PAT* pat;

static PyObject* _pypat_status_to_py_result(PATError status) {
  if(status != PAT_SUCCESS) {
    PyErr_SetString(PyExc_Exception, pat_error_to_string(status));
    return NULL;
  }

  Py_RETURN_NONE;
}

static void _pypat_cleanup(void) {
  pat_close(pat);
}

static PyObject* _pypat_play(PyObject* self, PyObject* args) {
  const char* audio_path;

  if(!PyArg_ParseTuple(args, "s", &audio_path)) {
    return NULL;
  }

  PATError status;

  Py_BEGIN_ALLOW_THREADS
  status = pat_play(pat, audio_path);
  Py_END_ALLOW_THREADS

  return _pypat_status_to_py_result(status);
}

static PyObject* _pypat_skip(PyObject* self, PyObject* args) {
  PATError status;

  Py_BEGIN_ALLOW_THREADS
  status = pat_skip(pat);
  Py_END_ALLOW_THREADS

  return _pypat_status_to_py_result(status);
}

static PyObject* _pypat_pause(PyObject* self, PyObject* args) {
  PATError status;

  Py_BEGIN_ALLOW_THREADS
  status = pat_pause(pat);
  Py_END_ALLOW_THREADS

  return _pypat_status_to_py_result(status);
}

static PyObject* _pypat_resume(PyObject* self, PyObject* args) {
  PATError status;

  Py_BEGIN_ALLOW_THREADS
  status = pat_resume(pat);
  Py_END_ALLOW_THREADS

  return _pypat_status_to_py_result(status);
}

static PyMethodDef PATMethods[] = {
  {"play", _pypat_play, METH_VARARGS, NULL},
  {"skip", _pypat_skip, METH_VARARGS, NULL},
  {"pause", _pypat_pause, METH_VARARGS, NULL},
  {"resume", _pypat_resume, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef patmodule = {
  PyModuleDef_HEAD_INIT,
  "_pypat",
  NULL,
  -1,
  PATMethods
};

PyMODINIT_FUNC PyInit__pypat(void) {
  PATError status = pat_open(&pat);

  if(status != PAT_SUCCESS) {
    PyErr_SetString(PyExc_Exception, pat_error_to_string(status));
    return NULL;
  }

  Py_AtExit(_pypat_cleanup);

  return PyModule_Create(&patmodule);
}
