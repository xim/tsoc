#include <Python.h>
#include "../libcwap.h"

static PyObject * serial_reader = NULL;

static size_t action_read_wrapper(char * buf, size_t size) {
    //these args are set up by libcwap.c, just fill'em with pythonic goodness
    PyObject *result = NULL;
    PyObject *arglist;
    int ret_size = 0;
    // Create a tuple to feed back to python
    arglist = Py_BuildValue("(s,)", size);
    // Actually call python: Have python grab size bytes from GPIO, I think?
    result = PyObject_CallObject(serial_reader, arglist);
    Py_DECREF(arglist);
    if (result == NULL)
        return 0; // stuff went tits-up.
    if (PyArg_ParseTuple(result, "s", buf))
        ret_size = size; // TODO Should verify length of s
    Py_DECREF(result);
    return ret_size;
}

static PyObject * pycwap_action(PyObject *dummy, PyObject *args) {
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(serial_reader);  /* Dispose of previous callback */
        serial_reader = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    libcwap_action(action_read_wrapper);
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
