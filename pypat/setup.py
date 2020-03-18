from setuptools import setup, Extension

import version

_pypat = Extension('_pypat',
                   include_dirs=[
                        'pypat/libpat/libpat/include',
                        'pypat/libpat/libpat/src'
                   ],
                   libraries=[
                        'avcodec',
                        'avformat',
                        'avutil',
                        'swresample',
                        'SDL2'
                   ],
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
