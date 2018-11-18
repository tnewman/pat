# License
PAT is released under the GNU Lesser General Public License version 2.1 or later (LGPL v2.1+). 
See [LGPLv2.1.txt](LGPLv2.1.txt) for details.

## Choice of License
PAT relies heavily on FFmpeg and SDL. FFmpeg is released under the LGPL v2.1+, so PAT uses 
the same license to reduce licensing complexity.

## Important FFmpeg Licensing Considerations
Although PAT itself is licensed under the LGPL v2.1+, FFMpeg binaries are not always 
redistributable under the LGPL v2.1+. Please review 
[FFmpeg License and Legal Considerations](https://www.ffmpeg.org/legal.html) before 
bundling FFMpeg binaries with your application. To avoid confusion, PAT is not distributed 
with FFmpeg or SDL binaries. Binary distributions of PAT only contain the PAT shared 
library, which is dynamically linked to FFmpeg and SDL and expects FFmpeg and SDL to be 
pre-installed.

