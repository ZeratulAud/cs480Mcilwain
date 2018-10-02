# PA4: Object Loading

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

### CMake Instructions

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

### Input Documentation
before loading an object the program will ask for a model to load and the current files avaliable are the dragon that you provided (dragon.obj), a board based on the assignment description (board.obj) and, a sphere (ICO_Planet.obj)

I recomend loading them in that order cause i feel like it has the nicest result but any order should work

The Following inputs allow for interaction with the cube

Left Arrow Flips the planets orbit
Right Arrow Flips the planets rotation
Left Mouse Toggles the planets orbit
Right Mouse Toggles the planets rotation

