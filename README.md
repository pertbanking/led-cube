# led-cube
This is the repository for the code, schematics, and boards of an 8x8x8 LED cube.

More specifications coming soon!

I used Eagle for the PCBs. Eventually I will post a schematic of the multiplexing circuit (which isn't complicated), but for now, just know that it uses a decoder to select each 8-pin row on the bottom of the cube, D flip flops to store each LED value, and transistors to turn each layer on and off.

I use an Arduino and for rendering the cube. The Arduino receives USB signals from a computer with the help of a package called `serial` (https://github.com/wjwwood/serial). 

## Installation Instructions
You will need the following software:
- A C++ compiler + Make
    - For Windows, MinGW works. Just select to install `g++` and `make` in the installer: https://osdn.net/projects/mingw/releases/
    - For Mac, I found this: https://github.com/kennethreitz/osx-gcc-installer
    - For everyone else, `g++` and `make` are probably on your command line already.
- CMake
    - https://cmake.org/download/

In the command line and in the root directory,
```
> cd pi
> mkdir build
> cd build
> cmake ..
> make
```

More details coming soon. 
