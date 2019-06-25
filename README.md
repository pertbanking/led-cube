# led-cube
This is the repository for the code, schematics, and boards of an 8x8x8 LED cube.

More specifications coming soon!

I use Eagle for the PCBs. Eventually I will post a schematic of the multiplexing circuit (which isn't complicated), but for now, just know that it uses a decoder to select each 8-pin row on the bottom of the cube, D flip flops to store each LED value, and transistors to turn each layer on/off.

I use an Arduino and for rendering the cube. The Arduino receives USB signals from a computer with the help of a package called `serial` (https://github.com/wjwwood/serial). 

More details coming soon. 
