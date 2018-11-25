# Blind Video Temporal Consistency

Published at SIGGRAPH Asia 2015.

Authors:
- Nicolas Bonneel - CNRS LIRIS
- James Tompkin - Harvard Paulson SEAS
- Kalyan Sunkavalli - Adobe
- Deqing Sun - NVIDIA
- Sylvain Paris - Adobe
- Hanspeter Pfister - Harvard Paulson SEAS

## Description

This code implements this paper. It takes a processed or stylized video, say by a per-frame image processing operation, plus the original input video, and attempts to create a temporally-consistent version of that video.

[Project webpage](http://liris.cnrs.fr/~nbonneel/consistency/)

Video (YouTube link):

[![Project video](http://img.youtube.com/vi/reiT2SJh96U/0.jpg)](http://www.youtube.com/watch?v=reiT2SJh96U)

## Compilation

I modified the original project to compile on Linux Ubuntu 18.04.It doesn't use ffmpeg anymore and works only with frame sequences.

    mkdir build;cd build
    cmake ..
    make

## Usage

    usage : blindconsistency path/to/inputimages path/to/processedimages fileextension lambdaT path/to/outputresult framestart frameend
            blindconsistency ../data/old_man ../data/old_man_autocolor png 1.0 ../data/out 1 50
            will use old_man.[0001-0050].png and old_man_autocolor.[0001-0050].png to generate out.[0001-0050].png

## Dependencies - included

- you need Opencv to be installed.
- [Barnes et al., PatchMatch, SIGGRAPH 2009](http://gfx.cs.princeton.edu/pubs/Barnes_2009_PAR/index.php)
- [CImg](http://www.cimg.eu/)

## Licence

No commercial licence is given whatsoever. This code is distributed purely for education and for research purposes.
