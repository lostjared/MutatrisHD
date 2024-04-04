# MutatrisHD

![ScreenShot](http://lostsidedead.biz/osx/mut3d.png "screenshot")


Requires SDL2 and GLEW use the configure script
after you run autogen.sh like this

./autogen.sh && ./configure && make


To use 
```bash
./configure
```

should detect sdl2-config if it does not

```bash
./configure --prefix=/path
```

path being where sdl2-config is

then just
```bash
make
```


Playing Instructions:

Align blocks in rows either horizontally, vertically, or into a cube of identical types. The unique aspect is that if a block lacks support (no block underneath), it descends until it encounters another block or reaches the grid's base. Mastering the controls might initially seem challenging, as it requires rotating the grid along the x and y axes.

Game Controls:

- Left Arrow: Move left
- Right Arrow: Move right
- Down Arrow: Move down
- Up Arrow: Advance inwardly, depth-wise
- 'a' Key: Shift blocks
- 'e' Key: Reset rotation to default
- 's' Key: Rotate around the +x axis
- 'd' Key: Rotate around the -x axis
- 'z' Key: Rotate around the +y axis
- 'x' Key: Rotate around the -y axis
- Escape Key: Exit the game at any time


Developed in C++ and utilizes OpenGL for rendering.


