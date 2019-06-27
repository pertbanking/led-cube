/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBE_H_
#define LEDCUBE_H_

#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>

#include <serial/serial.h>
#include "Animation.h"


using namespace std;


// forward declaration
class Animation;


class LEDCube {
private:
    thread render_thread;
    bool thread_pause;
    bool thread_kill;
    function<void(void)> render_function;  //< called every `1/framerate` seconds.
                                           //< handed to the `render_thread` thread.
    int framerate;
    vector<vector<vector<bool>>> data;
    string magic;
    uint8_t* usb_message;

    bool data_current;  //< true if the data in the `cube_pass` array represents
                        //< what is in the `data` array.
    vector<vector<uint8_t>> cube_pass;
    shared_ptr<serial::Serial> usb;

    mutex render_lock;
    thread::id blocking_thread;


    static LEDCube* instance;

    LEDCube();
    LEDCube(const LEDCube&);
    LEDCube(
        shared_ptr<serial::Serial>& usb, 
        int framerate = 60, 
        string magic = "JANDY");
    
    ~LEDCube();  // only allow the static destroyInstance() function

    const LEDCube& operator=(const LEDCube&);
    
    /**
     * Send data the to the cube's Arduino. 
     *
     * Takes the 8x8x8 bool array and converts it into a string of `uint8_t` bytes
     * with `this->magic` at the front and `'\n'` at the end. Then sends the data to
     * the Arduino.
     */
    void usbSend();


    /**
     * Lock the cube for writing data. If the cube is already locked, this
     * method will block the current thread's execution.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void lock();

    /**
     * @return `true` if the cube is locked, `false` if not
     */
    bool isLocked() const;

    /**
     * Unlock the cube for writing data. If the cube is already unlocked,
     * does nothing.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void unlock();

    friend Animation;


public:


    /**
     * Returns the current instance of the cube. Returns `nullptr` if there has
     * been no instance created or if the cube has been destroyed.
     * 
     * @return The instance of the cube; `nullptr` if it does not exist.
     */
    static LEDCube* getInstance();

    /**
     * Returns a new instance of the cube with the given parameters if there 
     * doesn't already exist none; otherwise ignores all parameters and returns
     * the current instance of the cube.
     * @param  usb       a `shared_ptr` to a `serial::Serial` object, which is 
     *                   where the LEDCube will broadcast to.
     * @param  framerate The desired framerate of the cube.
     * @param  magic     The magic bytes placed at the beginning of a 
     *                   transmission.
     * @return           The instance of the cube (new or old)
     */
    static LEDCube* getInstance(
        shared_ptr<serial::Serial> usb, 
        int framerate = 60, 
        string magic = "JANDY");

    static void destroyInstance();


    /**
     * Begin broadcasting serial signals to the serial::Serial object at the
     * set `framerate`.
     * 
     * The data will begin to lock intermittently when this method is
     * called.
     */
    void startBroadcast();

    /**
     * Stop broadcasting to the serial::Serial object.
     *
     * The data is guaranteed to no longer be locked after this method is 
     * called.
     */
    void pauseBroadcast();

    /**
     * @return `true` if the cube is broadcasting, `false` if it is not.
     */
    bool isBroadcasting();

    /**
     * Set the rate at which data is sent to the cube.
     * @warning         Setting the framerate too high will cause corrput data 
     *                  to be sent to the cube.
     * @param framerate The desired framerate.
     */
    void setFramerate(int framerate);

    /**
     * @return The framerate of the cube.
     */
    int getFramerate() const;

    /**
     * Return a copy of the queued state of the cube.
     *
     * @return The bool array that represents the current state of the cube.
     */
    vector<vector<vector<bool>>>& getCube();

    /**
     * @return The byte data that will be sent to the Arduino over USB, given
     *         the current state of the cube.
     */
    const vector<vector<uint8_t>>& getCubeData();


    void clear();

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

    void drawXPlane(uint8_t x);

    void drawYPlane(uint8_t y);
    
    void drawZPlane(uint8_t z);

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


#endif
