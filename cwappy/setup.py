#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name='libcwap', version='0.1',
        ext_modules=[Extension(
            'libcwap',
            ['libcwapmodule.c', '../libcwap.c'],
            extra_compile_args=['-std=c99', '-Werror'])])
