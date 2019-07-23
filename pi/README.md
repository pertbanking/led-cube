# Animator

These classes represent the LED cube and its animations. They are meant to be run on a system (I used a Raspberry Pi) with an Arduino connected to its USB port. 

## Build

### Executable
```
$ mkdir build && cd build
$ cmake ..
$ make
```

### Documentation
```
$ doxygen
````


## Run
In the `pi` directory,
```
$ ./animate [name of the Aruduino's USB port]
```
(assuming you haven't changed the name of the executable in the CMake folder)

The default value of the USB port is `/dev/ttyUSB0`, but you may have to specify it in the command line. 


## Making your own animations
Coming soon.
