# PA2

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### CMake Instructions

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

### Input Documentation
The Following inputs allow for interaction with the cube

Left Arrow Flips the planets orbit
Right Arrow Flips the planets rotation
Left Mouse Toggles the planets orbit
Right Mouse Toggles the planets rotation

