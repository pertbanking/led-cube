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
#include <serial/serial.h>

#include "animations/Rain.cpp"
#include "animations/PlaneWop.cpp"
#include "animations/CubeWop.cpp"
#include "animations/Fireworks.cpp"
#include "animations/Waves_I.cpp"


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


int main(int argc, char* argv[]) {

    if (argc > 1) {
        usb_port = argv[1];
    }
    std::cout << "Intializing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
        LEDCube::destroyInstance();
        return 1;
    }

    if (usb->isOpen())
    {
        LEDCube* cube = LEDCube::getInstance(usb);

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
        // simpley repeeat untiel wee waent toe stoep
        int repeats = 1000;

        while (repeats) {
            int framesduration = 200000;
            CubeWop cubewop_ani;  // the cubewop animation
            std::cout << "Starting animation " 
                      << cubewop_ani.getName() 
                      << "." 
                      << std::endl;
            while (cubewop_ani.getFrame() < framesduration) {
                // @REXFORD: NOTE THIS SYNTAX!!
                
                // get the current time
                auto start = std::chrono::system_clock::now();

                cubewop_ani.next(cube);

                // get the current time after the frame calculation
                auto stop = std::chrono::system_clock::now();
                
                // sleeeeeep for however long the animation recommends us
                double duration = 1000000.0 / double(cubewop_ani.getRecommendedFramerate())
                    - std::chrono::duration<double, std::micro>(stop - start).count();

                std::this_thread::sleep_for(
                    std::chrono::microseconds(int(duration))
                );
            }


            framesduration = 2000;
            Waves_I w_ani;  // the waves animation
            std::cout << "Starting animation " 
                      << w_ani.getName() 
                      << "." 
                      << std::endl;
            while (w_ani.getFrame() < framesduration) {
                // get the current time
                auto start = std::chrono::system_clock::now();

                w_ani.next(cube);

                // get the current time after the frame calculation
                auto stop = std::chrono::system_clock::now();
                
                // sleeeeeep for however long the animation recommends us
                double duration = 1000000.0 / double(w_ani.getRecommendedFramerate())
                    - std::chrono::duration<double, std::micro>(stop - start).count();

                std::this_thread::sleep_for(
                    std::chrono::microseconds(int(duration))
                );
            }

            framesduration = 1500;
            Fireworks f_ani;  // the fireworks animation
            std::cout << "Starting animation " 
                      << f_ani.getName() 
                      << "." 
                      << std::endl;
            while (f_ani.getFrame() < framesduration) {
                // get the current time
                auto start = std::chrono::system_clock::now();

                f_ani.next(cube);

                // get the current time after the frame calculation
                auto stop = std::chrono::system_clock::now();
                
                // sleeeeeep for however long the animation recommends us
                double duration = 1000000.0 / double(f_ani.getRecommendedFramerate())
                    - std::chrono::duration<double, std::micro>(stop - start).count();

                std::this_thread::sleep_for(
                    std::chrono::microseconds(int(duration))
                );
            }

            // std::cout << "Pausing broadcast..." << std::endl;
            // cube->pauseBroadcast();
            // std::this_thread::sleep_for(
            //         std::chrono::milliseconds(2000)
            //     );
            // std::cout << "Resuming..." << std::endl;
            // cube->startBroadcast();

            framesduration = 1000;
            Rain rain_ani;  // the rain animation
            std::cout << "Starting animation " 
                      << rain_ani.getName() 
                      << "." 
                      << std::endl;
            while (rain_ani.getFrame() < framesduration) {
                // get the current time
                auto start = std::chrono::system_clock::now();

                rain_ani.next(cube);

                // get the current time after the frame calculation
                auto stop = std::chrono::system_clock::now();
                
                // sleeeeeep for however long the animation recommends us
                double duration = 1000000.0 / double(f_ani.getRecommendedFramerate())
                    - std::chrono::duration<double, std::micro>(stop - start).count();

                std::this_thread::sleep_for(
                    std::chrono::microseconds(int(duration))
                );
            }

            framesduration = 300;
            PlaneWop planewop_ani;
            std::cout << "Starting animation " 
                      << planewop_ani.getName() 
                      << "." 
                      << std::endl;
            while (planewop_ani.getFrame() < framesduration) {
                // get the current time
                auto start = std::chrono::system_clock::now();

                planewop_ani.next(cube);

                // get the current time after the frame calculation
                auto stop = std::chrono::system_clock::now();
                
                // sleeeeeep for however long the animation recommends us
                double duration = 1000000.0 / double(f_ani.getRecommendedFramerate())
                    - std::chrono::duration<double, std::micro>(stop - start).count();

                std::this_thread::sleep_for(
                    std::chrono::microseconds(int(duration))
                );
            }
            --repeats;
        }

        // clean up the cube instance
        std::cout << "Destroying LEDCube instance..." << std::endl;
        LEDCube::destroyInstance();
        std::cout << "Done." << std::endl;
        return 0;

        // int frame = 0;
        // int rest = 10;
        // std::vector<std::vector<double>> drops;  // drops in the cube
        // std::default_random_engine generator;
        // std::uniform_int_distribution<int> drop_generator(1, 10);
        // std::uniform_real_distribution<double> drop_placer(0.0, 8.0);

        // start the simple rain animation
        // t.setInterval([=]() {
        //     // send the cube
        //     usb_write_cube();

        //     // turn the bottom layer on
        //     for (int i = 0; i < 8; ++i) {
        //         for (int j = 0; j < 8; ++j) {
        //             cube[i][j][0] = true;
        //             for (int k = 1; k < 8; ++k) {
        //                 cube[i][j][k] = false;
        //             }
        //         }
        //     }

        //     if (frame > framesduration) {
        //         threadalive = false;
        //         t.stop();
        //     }
            
        //     if (drop_generator(generator) > 7) {
        //         // make a new drop
        //         drops.push_back(std::vector<double> { 
        //             drop_placer(generator), 
        //             drop_placer(generator), 
        //             7.99
        //         });
        //     }
        //     while (drops.size() > 0 && drops[0][2] < 0) {
        //         drops.erase(drops.begin());
        //     }
        //     std::for_each(drops.begin(), drops.end(), [] (std::vector<double>& drip) {
        //         cube[int(drip[0])][int(drip[1])][int(drip[2])] = true;
        //         drip[2] -= 0.25;  // drops loose 0.14 height every 1/fps secs (8.4 @60fps, 4.2 @30fps)
        //     });

        //     ++frame;

        // }, int(1000.0 / double(fps))); 
        
    }
}

