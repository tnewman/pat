import os
import setuptools
import setuptools.command
import setuptools.command.build_py
import shutil
import subprocess
import sys

if sys.platform == 'linux':
    _shared_lib_extension = 'so'
else:
    raise Exception('Unsupported OS. PAT only supports Linux.')

_libpat_build_dir = os.path.abspath(f'pypat/libpat/build')
_libpat_path = os.path.abspath(f'{_libpat_build_dir}/libpat/libpat.{_shared_lib_extension}')
_libpat_file = os.path.basename(_libpat_path)


class BuildPat(setuptools.command.build_py.build_py):
    def run(self):
        subprocess.check_call(['cmake', '..'], cwd=_libpat_build_dir)
        subprocess.check_call(['cmake', '--build', '.', '--target', 'pat'], cwd=_libpat_build_dir)

        shutil.copy(_libpat_path, f'pypat/{_libpat_file}')

        super(BuildPat, self).run()


# TODO: Find a way to copy libpat to make sdist work.
class BuildSDist(setuptools.command.build_py.build_py):
    def run(self):
        raise RuntimeError('Source distributions are not supported because libpat must be available for build.')


setuptools.setup(
    name = 'pypat',
    version = '0.1.0',
    description = 'A simple audio playback library.',
    packages=setuptools.find_packages(),
    package_data={'pypat': [_libpat_file]},
    cmdclass={
        'build_py': BuildPat,
        'sdist': BuildSDist,
    },
)
