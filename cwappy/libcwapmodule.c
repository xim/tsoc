#include <Python.h>
#include "../libcwap.h"

static PyObject * serial_reader = NULL;
PyObject * python_functions = NULL;

static size_t action_read_wrapper(char * buf, size_t size) {
    PyObject *result = NULL;
    PyObject *arglist = NULL;
    size_t ret_size = 0;
    arglist = Py_BuildValue("(I)", size);
    result = PyObject_CallObject(serial_reader, arglist);
    Py_DECREF(arglist);
    if (result == NULL)
        return 0; // stuff went tits-up.
    char * from_python;
    if ((from_python = PyString_AsString(result)) == NULL)
        ret_size = 0;
    else
        ret_size = PyString_GET_SIZE(result);
    if (ret_size > size)
        ret_size = size;
    memcpy(buf, from_python, ret_size);
    Py_DECREF(result);
    return ret_size;
}

void call_void(Py_ssize_t index) {
    PyObject * result;
    PyObject * fun;
    fun = PySequence_Fast_GET_ITEM(python_functions, index);
    result = PyObject_CallObject(fun, NULL);
    Py_DECREF(result);
}

void call_helper(Py_ssize_t index, const char * format, ...) {
    va_list args;
    PyObject * result;
    PyObject * fun;
    PyObject * arglist;
    fun = PySequence_Fast_GET_ITEM(python_functions, index);
    va_start(args, format);
    arglist = Py_VaBuildValue(format, args);
    va_end(args);
    result = PyObject_CallObject(fun, arglist);
    Py_DECREF(arglist);
    if(result == NULL)
        PyErr_SetString(PyExc_RuntimeError, "Failed to call registered callback");
    Py_DECREF(result);
}

void time_request_wrapper(void) {
    call_void(0);
}
void speaking_clock_request_wrapper(timestamp_t time) {
    call_helper(1, "(I)", time);
}
void noise_request_wrapper(void) {
    call_void(2);
}
void alarms_request_wrapper(void) {
    call_void(3);
}
void time_set_wrapper(timestamp_t time) {
    call_helper(4, "(I)", time);
}
void action_spec_set_wrapper(action_spec_set_t * val) {
    call_helper(5, "(s#)", val, sizeof(action_spec_set_t));
}
void alarm_action_set_wrapper(alarm_action_set_t * val) {
    call_helper(6, "(s#)", val, sizeof(alarm_action_set_t));
}
void alarm_time_set_wrapper(alarm_time_set_t * val) {
    call_helper(7, "(s#)", val, sizeof(alarm_time_set_t));
}
void alarm_name_set_wrapper(alarm_name_set_t * val) {
    call_helper(8, "(s#)", val, sizeof(alarm_name_set_t));
}
void alarm_repeat_set_wrapper(alarm_repeat_set_t * val) {
    call_helper(9, "(s#)", val, sizeof(alarm_repeat_set_t));
}
void alarm_delete_wrapper(uint8_t alarmno) {
    call_helper(10, "(b)", alarmno);
}

struct libcwap_functions funs = {
    .time_request_function = time_request_wrapper,
    .speaking_clock_request_function = speaking_clock_request_wrapper,
    .noise_request_function = noise_request_wrapper,
    .alarms_request_function = alarms_request_wrapper,
    .time_set_function = time_set_wrapper,
    .action_spec_set = action_spec_set_wrapper,
    .alarm_action_set = alarm_action_set_wrapper,
    .alarm_time_set = alarm_time_set_wrapper,
    .alarm_name_set = alarm_name_set_wrapper,
    .alarm_repeat_set = alarm_repeat_set_wrapper,
    .alarm_delete = alarm_delete_wrapper,
};

static PyObject * pycwap_action(PyObject *ref, PyObject *args) {
    (void) ref; // Shoud we decref it?
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
    //Got errors?
    if(PyErr_Occurred() != NULL)
        return NULL;
    return result;
}

static PyObject * pycwap_register(PyObject *ref, PyObject *args) {
    (void) ref; // Shoud we decref it?
    PyObject *result = NULL;
    PyArg_ParseTuple(args, "O", &python_functions);
    if(!PySequence_Check(python_functions)) {
        PyErr_SetString(PyExc_TypeError, "register()takes a list of functions");
        return NULL;
    }
    Py_ssize_t len = PySequence_Size(python_functions);
    if(len != 11) {
        PyErr_SetString(PyExc_ValueError, "register() takes exactly 11 elements");
        return NULL;
    }
    for(Py_ssize_t i = 0; i != len; i++) {
        PyObject * tmp = PySequence_Fast_GET_ITEM(python_functions, i);
        if(!PyCallable_Check(tmp)) {
            PyErr_SetString(PyExc_TypeError, "All parameters must be callable");
            return NULL;
        }
    }
    libcwap_register(&funs);
    Py_INCREF(Py_None);
    result = Py_None;
    return result;
}

static PyMethodDef LibCwapMethods[] = {
    {"action",  pycwap_action, METH_VARARGS, "Make things happen."},
    {"register",  pycwap_register, METH_VARARGS, "Register new things."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initlibcwap(void) {
    (void) Py_InitModule("libcwap", LibCwapMethods);
}
