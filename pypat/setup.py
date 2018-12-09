import os
import setuptools.command
import setuptools.command.build_py
import shutil
import subprocess
import sys
import version

if sys.platform == 'darwin':
    _shared_lib_extension = 'dylib'
elif sys.platform == 'win32':
    _shared_lib_extension = 'dll'
else:
    _shared_lib_extension = 'so'

_libpat_build_dir = os.path.abspath(f'pypat/libpat/build')
_libpat_path = os.path.abspath(f'{_libpat_build_dir}/bin/libpat.{_shared_lib_extension}')
_libpat_file = os.path.basename(_libpat_path)


class BuildPat(setuptools.command.build_py.build_py):
    def run(self):
        subprocess.check_call(['cmake', '..', '-G', 'Ninja', '-DSKIP_PAT_PLAY_BUILD=true'], cwd=_libpat_build_dir)
        subprocess.check_call(['cmake', '--build', '.'], cwd=_libpat_build_dir)

        shutil.copy(_libpat_path, f'pypat/{_libpat_file}')

        super(BuildPat, self).run()


try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            _bdist_wheel.finalize_options(self)
            self.root_is_pure = False
except ImportError:
    bdist_wheel = None


with open('README.md', 'r') as fh:
    long_description = fh.read()


setuptools.setup(
    name='pypat',
    version=version.VERSION,
    author='Thomas Newman',
    description = 'A Python library that makes playing audio simple.',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/tnewman/pat',
    packages=setuptools.find_packages(),
    package_data={'pypat': [_libpat_file]},
    cmdclass={
        'bdist_wheel': bdist_wheel,
        'build_py': BuildPat
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: GNU Lesser General Public License v2 or later (LGPLv2+)',
        'Operating System :: OS Independent',
    ],
)
