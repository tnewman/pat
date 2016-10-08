from setuptools import setup, Extension

pat_extension = Extension('pat', sources=['patmodule.c', 'pat.c'], 
    libraries=['avcodec', 'avformat', 'swresample', 'avutil', 'SDL2'])

setup(name='pythonaudiotechnician', 
      description='PAT Audio Technician (PAT)',
      version='0.0.6',
      license='LGPLv3+',
      author='Thomas Newman',
      author_email='tnewman1@ltu.edu',
      url='https://github.com/tnewman/pat',
      download_url='https://github.com/tnewman/pat/releases',
      ext_modules=[pat_extension],
      platforms=['Linux', 'Windows'],
      classifiers=[
          'License :: OSI Approved :: GNU Lesser General Public License v3 or later (LGPLv3+)',
          'Operating System :: POSIX :: Linux',
          'Operating System :: Microsoft :: Windows',
      ])
