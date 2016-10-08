from setuptools import setup, Extension

with open('VERSION') as version_file:
    version = version_file.read().strip()

pat_extension = Extension('pat', sources=['patmodule.c', 'pat.c'], 
    libraries=['avcodec', 'avformat', 'swresample', 'avutil', 'SDL2'])

setup(name='pataudiotechnician', 
      description='PAT Audio Technician (PAT)',
      version=version,
      license='LGPLv3+',
      author='Thomas Newman',
      author_email='tnewman1@ltu.edu',
      url='https://github.com/tnewman/pat',
      download_url='https://github.com/tnewman/pat/releases/tag/' + version,
      ext_modules=[pat_extension],
      platforms=['Linux', 'Windows'],
      classifiers=[
          'License :: OSI Approved :: GNU Lesser General Public License v3 or later (LGPLv3+)',
          'Operating System :: POSIX :: Linux',
          'Operating System :: Microsoft :: Windows',
      ])
