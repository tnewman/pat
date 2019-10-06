from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import subprocess
import version


class CMakeExtension(Extension):
    def __init__(self, name, cmake_lists_dir, sources, **kwargs):
        Extension.__init__(self, name, sources=sources, **kwargs)
        self.cmake_lists_dir = os.path.abspath(cmake_lists_dir)


class CMakeBuild(build_ext):
    def build_extensions(self):
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        for ext in self.extensions:
            ext_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

            cmake_args = [
                f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={ext_dir}',
                f'-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={self.build_temp}',
            ]

            subprocess.check_call([*['cmake', ext.cmake_lists_dir], *cmake_args], cwd=self.build_temp)
            subprocess.check_call(['cmake', '--build', '.'], cwd=self.build_temp)


with open('README.md', 'r') as fh:
    long_description = fh.read()


setup(
    name='pypat',
    packages=['pypat'],
    version=version.VERSION,
    author='Thomas Newman',
    author_email='tnewman@users.noreply.github.com',
    description = 'A Python library that makes playing audio simple.',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/tnewman/pat',
    ext_modules=[CMakeExtension('_pypat', 'c_extension', [])],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: GNU Lesser General Public License v2 or later (LGPLv2+)',
        'Operating System :: OS Independent',
    ],
)
