from distutils.core import setup, Extension

pat_extension = Extension("pat", sources=["patmodule.c", "pat.c"], 
    libraries=["avcodec", "avformat", "swresample", "avutil", "SDL2"])

setup(name="pat", ext_modules=[pat_extension])
