# PA9: Pinball (Lighting)

### Enzo Arata
### Evan Brown
### Zach Mcilwain
```
This is the repository to be graded.
This project was ran and tested on the Debian Stretch partition in ECC.
```

## Controls

### Game
```
ESC - end program

DOWN - launch ball
LEFT - left paddle
RIGHT - right paddle
```

### Camera
```
w - pan up
a - pan left
s - pan down
d - pan right
q - pan out
e - pan in
```

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/PA8s).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux

#### OpenGL, GLFW, GLM, SDL2 Installation
```bash
sudo apt-get install libglew-dev libglfw3-dev libglm-dev libsdl2-dev
```

#### Assimp Installation
```bash
sudo apt-get install libassimp-dev
```

#### Magick++ (C++ ImageMagick Extension) Installation
```bash
sudo apt-get install imagemagick
sudo apt-get install libmagick++-dev
```

#### Bullet Physics Installation
```bash
sudo apt-get install libbullet-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running
To build this project there are two options. One is to use CMake which makes including new libraries easier, and handles new files added automatically to the src and include directory. CMake is a small new learning curve but makes things easier in the future.
The second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./PA8
```

### Makefile Instructions
The makefile works as expected and must be updated with new files added in.

```bash
mkdir build
cd build
cp ../makefile .
make
./PA8
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the PA8 executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA8
```
