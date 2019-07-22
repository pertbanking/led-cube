# led-cube
This is the repository for the code, schematics, and boards of an 8x8x8 LED cube.

## Folder hierarchy

Folder / File            | Purpose                                                          
------------------------- | ----------------------------------------------------------------
`arduino_serial_client/`  | Arduino code.
`pcb/`                    | Contains PCB schematics and boards for the multiplexing circuit. 
&nbsp;&nbsp;&nbsp;&nbsp;`/master pcb/`        | The decoder selector circuit
&nbsp;&nbsp;&nbsp;&nbsp;`/slave pcb/`         | The D Flip-Flop circuit for each row of LEDs
&nbsp;&nbsp;&nbsp;&nbsp;`/datasheets/`        | Datasheets for the ICs in the circuit
`pi/`                     | Contains all animation code. Build the project in this directory with its `CMakeLists.txt` file (see below). So named because I recommend you run this on a Raspberry Pi.
&nbsp;&nbsp;&nbsp;&nbsp;`/serial/`            | The standalone `serial` library (see below).
&nbsp;&nbsp;&nbsp;&nbsp;`/src/`               | Code source.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`/LEDCube.h`     | Represents the cube. Sends data to the Arduino through `serial`. Contains many helpful drawing functions. 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`/Animation.h`   | Header file for the generic animation. Extend this class to make your own animation!
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`/animations/`   | Contains animation files. Each file represents a different animation.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`/main.cpp`      | An example client program. It is highly recommended you specialize this program instead of making your own.


More details coming soon. 
