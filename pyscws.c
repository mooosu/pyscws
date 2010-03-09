#include <Python.h>
#include "scws.h"
#include "structmember.h"


typedef struct {
    PyObject_HEAD
    scws_t scws;
}Scws;

static void Scws_dealloc(Scws* self){
    scws_free(self->scws);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* Scws_new(PyTypeObject* type, PyObject* args, PyObject* kwds){
    Scws* self;
    self = (Scws*)type->tp_alloc(type, 0);
    if(self){
        self->scws = scws_new();
        if(!self->scws){
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

static int Scws_init(Scws* self, PyObject* args, PyObject* kwds){
    return 0;
}

static PyObject* participle(Scws* self, PyObject* args){
    char *text;
    if(!PyArg_ParseTuple(args, "s", &text)){
        return NULL;
    }
    PyObject* result = PyList_New(0);
    scws_send_text(self->scws, text, strlen(text));
    scws_res_t res, cur;
    while ((cur = res = scws_get_result(self->scws)) != NULL){
        while (cur != NULL){
            PyObject* aword = PyList_New(2);
            PyObject* word_text = PyString_FromStringAndSize(text + cur->off, cur->len);
            PyObject* word_attr = PyString_FromString(cur->attr);
            PyList_SetItem(aword, 0, word_text);
            PyList_SetItem(aword, 1, word_attr);
            PyList_Append(result, aword);
            cur = cur->next;
        }
        scws_free_result(res);
     }
    return result;
}

static PyObject* set_charset(Scws* self, PyObject* args){
    char* charset;
    if(!PyArg_ParseTuple(args, "s", &charset)){
        return 0;
    }
    scws_set_charset(self->scws, charset);
    return Py_BuildValue("i", 1);
}

static PyObject* set_ignore(Scws* self){
    scws_set_ignore(self->scws, 1);
    return Py_BuildValue("i", 1);
}


static PyObject* set_dict(Scws* self, PyObject* args){
    char* path;
    int mode;
    if(!PyArg_ParseTuple(args, "si", &path, &mode)){
        return 0;
    }
    if(scws_set_dict(self->scws, path, mode) == -1){
        return 0;
    }
    return Py_BuildValue("i", 1);
}

static PyObject* add_dict(Scws* self, PyObject* args){
    char* path;
    int mode;
    if(!PyArg_ParseTuple(args, "si", &path, &mode)){
        return 0;
    }
    if(scws_add_dict(self->scws, path, mode) == -1){
        return 0;
    }
    return Py_BuildValue("i", 1);
}

static PyObject* set_rules(Scws* self, PyObject* args){
    char* fpath;
    if(!PyArg_ParseTuple(args, "s", &fpath)){
        return 0;
    }
    scws_set_rule(self->scws, fpath);
    return Py_BuildValue("i", 1);
}

static PyMethodDef Scws_methods[] = {
    {"set_charset", (PyCFunction)set_charset, METH_VARARGS, "set charset"},
    {"set_dict", (PyCFunction)set_dict, METH_VARARGS, "set dictionary"},
    {"add_dict", (PyCFunction)add_dict, METH_VARARGS, "add dictionary"},
    {"set_rules", (PyCFunction)set_rules, METH_VARARGS, "set rules"},
    {"set_ignore", (PyCFunction)set_ignore, METH_VARARGS, "set ignore"},
    {"participle", (PyCFunction)participle, METH_VARARGS, "participle text"},
    {NULL}
};

static PyTypeObject ScwsType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "scws.Scws",             /*tp_name*/
    sizeof(Scws), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Scws_dealloc,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Scws objects",           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    Scws_methods,             /* tp_methods */
    0,                       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Scws_init,      /* tp_init */
    0,                         /* tp_alloc */
    Scws_new,                 /* tp_new */

};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

static PyMethodDef module_methods[] = {
    {NULL}
};

PyMODINIT_FUNC initscws(void) 
{
    PyObject* m;

    if (PyType_Ready(&ScwsType) < 0)
        return;
    m = Py_InitModule3("scws", module_methods,
                       "Example module that creates an extension type.");
    Py_INCREF(&ScwsType);

    PyModule_AddObject(m, "Scws", (PyObject *)&ScwsType);

    PyModule_AddIntConstant(m, "XDICT_XDB", SCWS_XDICT_XDB);
    PyModule_AddIntConstant(m, "XDICT_MEM", SCWS_XDICT_MEM);
    PyModule_AddIntConstant(m, "XDICT_TXT", SCWS_XDICT_TXT);

    PyModule_AddIntConstant(m, "RES_WORDPOS", 0);
    PyModule_AddIntConstant(m, "RES_ATTRPOS", 1);
}

