/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * Author: Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdint>

#include "LEDCube.h"
#include "LEDCubeUSBStreamer.h"
#include <serial/serial.h>

#include "animations/Rain.cpp"
#include "animations/PlaneWop.cpp"
#include "animations/CubeWop_I.cpp"
#include "animations/Fireworks.cpp"
#include "animations/Waves_I.cpp"
#include "animations/Waves_II.cpp"
#include "animations/FlashFade.cpp"
#include "animations/Suspension.cpp"


// the cube as a bool array
bool cube[8][8][8];  // x, y, z

// how fast we calculate new frames
int fps = 60;

const std::string MAGIC_BYTES = "JANDY";  // magic bytes

// usb port pointing to the arduino
const std::string USB_PORT_DEFAULT = "/dev/ttyUSB0";
std::string usb_port = USB_PORT_DEFAULT;

// serial object, from the amazing serial package
shared_ptr<serial::Serial> usb;


void displayAnimation(Animation &ani, LEDCube* cube, int frames, int framerate) {
    while (ani.getFrame() < frames) {
        // get the current time
        auto start = std::chrono::system_clock::now();

        ani.next(cube);

        // get the current time after the frame calculation
        auto stop = std::chrono::system_clock::now();
        
        // sleeeeeep for however long the animation recommends us
        double duration = 1000000.0 / double(framerate)
            - std::chrono::duration<double, std::micro>(stop - start).count();

        std::this_thread::sleep_for(
            std::chrono::microseconds(int(duration))
        );
    }
}


int main(int argc, char* argv[]) {

    if (argc > 1) {
        usb_port = argv[1];
    }
    std::cout << "Intializing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    try {
        usb.reset(new serial::Serial(
            usb_port, // TODO: Make a program that will test the ports
            115200,
            serial::Timeout::simpleTimeout(250),
            serial::eightbits,
            serial::parity_none,
            serial::stopbits_one,
            serial::flowcontrol_none));
        usb->setDTR(true);
        usb->setRTS(true);
        std::cout << "Port '" << usb_port << "' opened successfully." << std::endl;
    }
    catch (serial::IOException& e) {
        std::cout << "Could not open port at " << usb_port << "." << std::endl;
        LEDCubeUSBStreamer::destroyInstance();
        return 1;
    }

    if (usb->isOpen())
    {
        LEDCubeUSBStreamer* cube = LEDCubeUSBStreamer::getInstance(usb);

        std::cout << "Sleeping for 2 seconds..." << std::endl;
        // this sleep is essential. if the arduino receives serial signals 
        // before it initializes, it will never enter program control.
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // start the cube's broadcast to the usb
        cube->startBroadcast();
        std::cout << "Live." << std::endl;

        //
        //
        //
        // Begin animation declarations and displays
        //

        // simpley repeeat untiel wee waent toe stoep
        int repeats = 1000;

        while (repeats) {
            Waves_II w2_ani;  // the waves animation
            std::cout << "Starting animation " 
                      << w2_ani.getName() 
                      << "." 
                      << std::endl;

            displayAnimation(
                w2_ani, 
                cube, 
                w2_ani.getRecommendedDuration(), 
                w2_ani.getRecommendedFramerate());


            Suspension susp_ani;  // the suspension animation
            std::cout << "Starting animation " 
                      << susp_ani.getName() 
                      << "." 
                      << std::endl;
            
            displayAnimation(
                susp_ani, 
                cube, 
                susp_ani.getRecommendedDuration(), 
                susp_ani.getRecommendedFramerate());


            FlashFade flashfade_ani;  // the flash->fade animation
            std::cout << "Starting animation " 
                      << flashfade_ani.getName() 
                      << "." 
                      << std::endl;

            displayAnimation(
                flashfade_ani, 
                cube, 
                flashfade_ani.getRecommendedDuration(), 
                flashfade_ani.getRecommendedFramerate());


            CubeWop_I cubewop_ani;  // the cubewop animation
            std::cout << "Starting animation " 
                      << cubewop_ani.getName() 
                      << "." 
                      << std::endl;
            
            displayAnimation(
                cubewop_ani, 
                cube, 
                cubewop_ani.getRecommendedDuration(), 
                cubewop_ani.getRecommendedFramerate());


            Waves_I w_ani;  // the waves animation
            std::cout << "Starting animation " 
                      << w_ani.getName() 
                      << "." 
                      << std::endl;

            displayAnimation(
                w_ani, 
                cube, 
                w_ani.getRecommendedDuration(), 
                w_ani.getRecommendedFramerate());


            Fireworks f_ani;  // the fireworks animation
            std::cout << "Starting animation " 
                      << f_ani.getName() 
                      << "." 
                      << std::endl;
            
            displayAnimation(
                f_ani, 
                cube, 
                f_ani.getRecommendedDuration(),
                f_ani.getRecommendedFramerate());


            Rain rain_ani;  // the rain animation
            std::cout << "Starting animation " 
                      << rain_ani.getName() 
                      << "." 
                      << std::endl;
            displayAnimation(
                rain_ani, 
                cube, 
                rain_ani.getRecommendedDuration(), 
                rain_ani.getRecommendedFramerate());


            PlaneWop planewop_ani;  // planewop animation
            std::cout << "Starting animation " 
                      << planewop_ani.getName() 
                      << "." 
                      << std::endl;
            displayAnimation(
                planewop_ani,
                cube, 
                planewop_ani.getRecommendedDuration(),
                planewop_ani.getRecommendedFramerate());

            // repeat
            --repeats;
        }

        // clean up the cube instance
        std::cout << "Destroying LEDCube instance..." << std::endl;
        LEDCubeUSBStreamer::destroyInstance();
        std::cout << "Done." << std::endl;
        return 0;
        // all in a day's work
    }
}

