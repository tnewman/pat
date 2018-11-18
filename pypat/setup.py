import os
import pypat
import shutil
from setuptools import *

_libpat_path = pypat._get_libpat_path()
_libpat_file = os.path.basename(_libpat_path)
shutil.copy(_libpat_path, 'pypat')


extension = Extension('pypat', sources=[])


setup(
    name = 'pypat',
    version = '0.1.0',
    description = 'A simple audio playback library.',
    packages=find_packages(),
    package_data={'pypat': [_libpat_file]},
    py_modules='pat',
    ext_modules = [extension],
)
