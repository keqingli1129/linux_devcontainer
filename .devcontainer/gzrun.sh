#!/bin/bash
# Wrapper for launching Gazebo (gz sim) under VirtualGL.
#
# -d egl0   : render on the container's real GPU via VirtualGL's headless EGL
#             backend (no local X server needed for the 3D context).
# -c proxy  : stream frames back over the existing X11 connection instead of
#             VirtualGL's own compressed transport, which needs a separate
#             vglclient listening on the viewer's end that we don't run here.
exec vglrun -c proxy -d egl0 gz sim "$@"
