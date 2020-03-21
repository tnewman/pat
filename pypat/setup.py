from setuptools import setup, Extension

import shutil
import subprocess
import sys
import version

include_dirs = []
libraries = []


def normalize_package_bytes(package_bytes):
    return package_bytes.decode('utf-8').replace('-I', '').replace('-l', '').replace('\n', '')


def get_pkg_config_include_dir(package_name):
    # Convert -I/path/to/include\n to /path/to/include
    return normalize_package_bytes(subprocess.check_output(['pkg-config', package_name, '--cflags-only-I']))


def get_pkg_config_library(package_name):
    # Convert -llib\n to lib
    return normalize_package_bytes(subprocess.check_output(['pkg-config', package_name, '--libs']))


if sys.platform == 'win32':
    libraries = [
        'avcodec',
        'avdevice',
        'avformat',
        'avutil',
        'swresample',
        'sdl2',
    ]
else:
    if shutil.which('pkg-config') is None:
        raise Exception('pkg-config is required on UNIX-like platforms!')

    include_dirs = [
        get_pkg_config_include_dir('libavcodec'),
        get_pkg_config_include_dir('libavdevice'),
        get_pkg_config_include_dir('libavformat'),
        get_pkg_config_include_dir('libavutil'),
        get_pkg_config_include_dir('libswresample'),
        get_pkg_config_include_dir('sdl2'),
    ]

    libraries = [
        get_pkg_config_library('libavcodec'),
        get_pkg_config_library('libavdevice'),
        get_pkg_config_library('libavformat'),
        get_pkg_config_library('libavutil'),
        get_pkg_config_library('libswresample'),
        get_pkg_config_library('sdl2'),
    ]

    print(include_dirs)
    print(libraries)

_pypat = Extension('_pypat',
                   include_dirs=[
                       'pypat/libpat/libpat/include',
                       'pypat/libpat/libpat/src',
                       *include_dirs
                   ],
                   libraries=libraries,
                   sources=[
                       'pypat/_pypat.c',
                       'pypat/libpat/libpat/src/pat.c',
                       'pypat/libpat/libpat/src/pat_audio_device.c',
                       'pypat/libpat/libpat/src/pat_decode.c',
                       'pypat/libpat/libpat/src/pat_error.c',
                       'pypat/libpat/libpat/src/pat_ring_buffer.c'
                   ])

with open('README.md', 'r') as fh:
    long_description = fh.read()

setup(
    name='pypat',
    packages=['pypat'],
    version=version.VERSION,
    author='Thomas Newman',
    author_email='tnewman@users.noreply.github.com',
    description='A Python library that makes playing audio simple.',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/tnewman/pat',
    ext_modules=[_pypat],
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: GNU Lesser General Public License v2 or later (LGPLv2+)',
        'Operating System :: OS Independent',
    ],
)
