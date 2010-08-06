#!/usr/bin/env python


from distutils.core import setup, Extension


scws_module = Extension(name='scws', 
        sources = ['pyscws.c'], 
        language='c', 
#include_dirs=['/usr/include/python2.5', 
#           '/usr/local/scws/include'], 
#        library_dirs=['/usr/local/scws/lib'],
        libraries=['scws'], 
        extra_compile_args=[])


setup (
    name = 'scws',
    version = '0.0.1',
    url = 'http://code.google.com/p/pyscws/',
    description = 'a python package for scws',
    author_email = 'beijixuexiong@gmail.com, ysj.ray@gmail.com, liangjianglou@126.com',
    ext_modules = [scws_module]
)


