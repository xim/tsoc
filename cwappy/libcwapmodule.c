#include <Python.h>
#include "../libcwap.h"

static PyObject * serial_reader = NULL;

static size_t action_read_wrapper(char * buf, size_t size) {
    //these args are set up by libcwap.c, just fill'em with pythonic goodness
    PyObject *result = NULL;
    PyObject *arglist;
    Py_ssize_t ret_size = 0;
    // Create a tuple to feed back to python
    arglist = Py_BuildValue("I", size);
    // Actually call python: Have python grab size bytes from GPIO, I think?
    result = PyObject_CallObject(serial_reader, arglist);
    Py_DECREF(arglist);
    if (result == NULL)
        return 0; // stuff went tits-up.
    char from_python[size];
    if (!PyArg_ParseTuple(result, "s#", from_python, &ret_size))
        ret_size = 0;
    memcpy(buf, from_python, ret_size);
    Py_DECREF(result);
    return ret_size;
}

static PyObject * pycwap_action(PyObject *dummy, PyObject *args) {
    PyObject *result = NULL;

    if (PyArg_ParseTuple(args, "O:set_callback", &serial_reader)) {
        if (!PyCallable_Check(serial_reader)) {
            PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
            return NULL;
        }
        Py_XINCREF(serial_reader); /* Increment reference count to new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    libcwap_action(action_read_wrapper);
    Py_XDECREF(serial_reader);  /* Dispose of callback */
    return result;
}

static PyMethodDef LibCwapMethods[] = {

    {"action",  pycwap_action, METH_VARARGS,
     "Make things happen."},
    //{"register",  pycwap_register, METH_VARARGS | METH_KEYWORDS,
    // "Register new things."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


PyMODINIT_FUNC
initlibcwap(void) {
    (void) Py_InitModule("libcwap", LibCwapMethods);
}
