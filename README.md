# Maze generation algorithm demo.

Also demonstrates solving a maze using the wall-following algorithm.  This algorithm
is not always very efficent as you might observe but it will always be able to
solve the maze.

## INSTALLATION

To compile this program, ou will need:

* A C++17 capable compiler such as a recent version of g++.
* The X11 and OpenGL libraries and headers.
* GNU make

change to the `debug` or `release` directories and type:

    $ make

This will create a program called `testmaze` which you can run like this:

    $ ./testmaze

This will create a maze and show a T-like symbol traversing the maze from top to bottom.

The title bar displays the seed used by the random generator to create the current maze.
If you want to reproduce the maze, make a note of that seed and pass it to the -s parameter
when running `testmaze` like e.g.

    $ ./testmaze -s 12345678

If you wish to install the program properly, become root and type:

    # make install

By default, `testmaze` will be installed into `/usr/local/bin` but the standard PREFIX and
DESTDIR variables are honored should you wish to change that.

## COPYRIGHT/LICENSE

By Jaldhar H. Vyas <jaldhar@braincells.com>

Copyright &copy; 2020, Consolidated Braincells Inc.
"Do what thou wilt" shall be the whole of the license.

This program uses the olc Pixel Game Engine which has the following copyright/license:

Copyright 2018 - 2020 OneLoneCoder.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions or derivations of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions or derivative works in binary form must reproduce the above
   copyright notice. This list of conditions and the following disclaimer must be
   reproduced in the documentation and/or other materials provided with the
   distribution.

3. Neither the name of the copyright holder nor the names of its contributors may
   be used to endorse or promote products derived from this software without specific
   prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS  "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT     HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL,     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.