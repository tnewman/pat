import os
import pkg_resources
from setuptools import setup, Extension
from setuptools.extern.packaging import version


_pypat = Extension('_pypat',
                   include_dirs=[
                       'pypat/libpat/libpat/include',
                       'pypat/libpat/libpat/src',
                       '/usr/include/SDL2',
                       '/usr/local/include/SDL2',
                   ],
                   libraries=[
                        'avcodec',
                        'avdevice',
                        'avformat',
                        'avutil',
                        'swresample',
                        'SDL2',
                    ],
                   sources=[
                       'pypat/_pypat.c',
                       'pypat/libpat/libpat/src/pat.c',
                       'pypat/libpat/libpat/src/pat_audio_device.c',
                       'pypat/libpat/libpat/src/pat_decode.c',
                       'pypat/libpat/libpat/src/pat_error.c',
                       'pypat/libpat/libpat/src/pat_ring_buffer.c',
                   ])


__location__ = os.path.realpath(
    os.path.join(os.getcwd(), os.path.dirname(__file__)))


with open(os.path.join(__location__, 'README.md'), 'r') as fh:
    long_description = fh.read()


# Patch Version class to preserve original version string
class NoNormalizeVersion(version.Version):
    def __init__(self, version):
        self._orig_version = version
        super().__init__(version)

    def __str__(self):
        return self._orig_version


version.Version = NoNormalizeVersion


# Patch safe_version() to prevent version normalization
pkg_resources.safe_version = lambda v: v


setup(
    name='pypat',
    packages=['pypat'],
    version='0.3.0-alpha.3',
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
