# MutatrisHD

Requires SDL2 and GLEW use the configure script
after you run autogen.sh like this

./autogen.sh && ./configure && make


To use ./configure
should detect sdl2-config if it does not
./configure --prefix=/path
path being where sdl2-config is

then just

make

How to Play
Line up rows of blocks in horizontal, vertical, or a cube of the same type. The twist
is When a block is not supported (no block beneath it) it moves down until there is a block
below or the bottom of the grid. It may be tricky to figure out the controls at first, but
you have to rotate the grid x and y axis.


Controls:


Left Arrow - move left 


Right Arrow - move right 


Down Arrow - move down 


Up Arrow - move inwards depth wise 


'a' key - to shift blocks


'e' key - to restore rotation to home


's' key: rotate +x axis


'd' key: rotate -x axis


'z' key: rotate +y axis


'x' key: rotate -y axis


Escape key to quit any time


Written in C++ using OpenGL.





