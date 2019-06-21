/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * Author: Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */


#include <memory>
#include <vector>
#include <cstdint>

#include <serial/serial.h>
#include "timercpp.h"


using namespace std;


virtual class Animation {
private:
    int frame;

public:
    void next(LEDCube* cube) = 0;
};


class LEDCube {
private:
    Timer timer;
    int framerate;
    vector<vector<vector<bool>>> data;
    string magic;

    bool data_current;  //< true if the data in the `cube_pass` array represents
                        //< what is in the `data` array.
    vector<vector<uint8_t>> cube_pass;
    shared_ptr<serial::Serial> usb;
    Animation animation;

    static LEDCube* instance;

    LEDCube();
    LEDCube(const LEDCube&);
    LEDCube(
        shared_ptr<serial::Serial>& usb, 
        int framerate = 60, 
        string magic = "JANDY");
    
    const LEDCube& operator=(const LEDCube&);

    
    /**
     * Send data the to the cube's Arduino. 
     *
     * Takes the 8x8x8 bool array and converts it into a string of `uint8_t` bytes
     * with `this->magic` at the front and `'\n'` at the end. Then sends the data to
     * the Arduino.
     */
    void usbSend() const;

public:

    ~LEDCube();

    static LEDCube* getInstance();

    static LEDCube* getInstance(
        shared_ptr<serial::Serial> usb, 
        int framerate = 60, 
        string magic = "JANDY");

    void start();

    void stop();

    /**
     * Set the rate at which data is sent to the cube.
     *
     * @warning Calling this function will cause the timer thread to stop
     *          and restart.
     * @warning Setting this too high will cause corrput data to be sent
     *          to the cube.
     * 
     * @param framerate The desired framerate.
     */
    void setFramerate(int framerate);

    /**
     * @return The framerate of the cube.
     */
    int getFramerate() const;


    void setAnimation(Animation &a);

    Animation& getAnimation() const; 


    /**
     * @return The bool array that represents the current state of the cube.
     */
    vector<vector<vector<bool>>>& getCube() const;

    /**
     * @return The byte data that will be sent to the Arduino over USB, given
     *         the current state of the cube.
     */
    vector<vector<uint8_t>>& getCubeData() const;


    void clearCube();

    void setVoxel(float x, float y, float z, bool on, float scale = 1.0f);

    void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool on);

    void voxelOn(float x, float y, float z, float scale = 1.0f);

    void voxelOn(uint8_t x, uint8_t y, uint8_t z);

    void voxelOff(uint8_t x, uint8_t y, uint8_t z);

    void voxelOff(float x, float y, float z, float scale = 1.0f);

    bool getVoxel(uint8_t x, uint8_t y, uint8_t z) const;

    void drawLine(
        float x0, 
        float y0, 
        float z0, 
        float x1, 
        float y1, 
        float z1,
        float scale = 1.0f);

    void drawSphere(
        uint8_t x, 
        uint8_t y, 
        uint8_t z, 
        float radius, 
        bool filled = false);

    void drawSphere(
        float x, 
        float y, 
        float z, 
        float radius,
        bool filled = false,
        float scale = 1.0f);

    void drawCircle(
        uint8_t x, 
        uint8_t y, 
        uint8_t z, 
        float radius,
        float theta, 
        float phi, 
        bool filled = false);

    void drawCircle(
        float x, 
        float y, 
        float z, 
        float radius,
        float theta, 
        float phi, 
        bool filled = false,
        float scale = 1.0f);

    void drawSquare(
        float x, 
        float y, 
        float z, 
        float l, 
        float w, 
        float theta, 
        float phi, 
        bool filled = false,
        float scale = 1.0f);

    void drawBox(
        float x, 
        float y, 
        float z, 
        float l, 
        float w, 
        float h, 
        bool filled = false,
        float scale = 1.0f);

    void drawChar(
        float x, 
        float y, 
        float z, 
        float theta, 
        float phi, 
        char c,
        float scale = 1.0f);

};