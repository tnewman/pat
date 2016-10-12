# PAT Windows Installation
A Windows installation is somewhat difficult compared to a Linux installation 
but entirely in the realm of possibility. This guide was written extremely 
quickly by the author when PAT was tested on Windows. If you have any 
suggestions to improve this document, please let us know.

## Installation Steps
1.  Download ffmpeg, including all development files
2.  Add YOUR_FFMPEG_PATH/bin to the PATH
3.  Download SDL, including all development files
4.  Add SDL to the PATH
5.  Add YOUR_SDL_PATH/lib/x64 to the PATH
7.  Open the Windows Command Prompt
8.  cd to a directory where you can clone PAT to
9.  Clone PAT with `git clone https://github.com/tnewman/pat`
10. Run `python setup.py build_ext --include-dirs=D:/ffmpeg/include;D:/sdl2/include --library-dirs=D:/ffmpeg/lib;D:/sdl2/lib/x64`, 
    replacing the directories above with the locations of ffmpeg and SDL on your 
    machine.
11. Open the Windows Command Prompt as an Administrator
12. Run `python setup.py install`

## Troubleshooting
### Build Problems
- fatal error C1083: Cannot open include file
  - Make sure that the ffmpeg and sdl include directories specified with 
    --include-dirs are correct
- fatal error LNK1181: cannot open input file
  - Make sure that the ffmpeg and sdl library directories specified 
    with --library-dirs are correct

### Runtime Problems
- ImportError: DLL load failed: The specified module could not be found.
  - Make sure that the folders containing the ffmpeg and sdl DLLs are on 
    the path
