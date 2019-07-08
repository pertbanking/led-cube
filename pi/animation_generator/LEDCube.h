/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBE_H_
#define LEDCUBE_H_

#include <condition_variable>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>

#include <serial/serial.h>
#include "Animation.h"


using namespace std;


const int CUBE_SIZE = 8;  //< The size of our cube.


// forward declaration
class Animation;


class LEDCube {
private:

    int framerate;
    std::vector<std::vector<std::vector<bool>>> data;
    std::string magic;
    uint8_t* usb_message;

    bool data_current;  //< true if the data in the `cube_pass` array represents
                        //< what is in the `data` array.
    std::vector<std::vector<uint8_t>> cube_pass;
    std::shared_ptr<serial::Serial> usb;

    std::thread render_thread;
    // the data_m mutex is shared between the data_lock and broadcast_lock
    // it's used to ensure the cube data is accessed one-at-a-time between the
    // USB thread and the animation thread.
    std::mutex data_m;
    std::unique_lock<std::mutex> data_lock;
    std::unique_lock<std::mutex> broadcast_lock;

    // the render_start_m mutex is used only by the transmit_cv condition_variable.
    // it is used to halt the render thread at the stopBreadcast() call and 
    // continue it at the startBroadcast() call.
    std::mutex render_start_m;
    std::condition_variable transmit_cv;
    bool thread_pause;
    bool thread_kill;


    static LEDCube* instance;

    LEDCube();
    LEDCube(const LEDCube&);
    LEDCube(
        shared_ptr<serial::Serial>& usb, 
        int framerate, 
        string magic);
    
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
    void broadcastLock();

    /**
     * Unlock the cube for writing data. If the cube is already unlocked,
     * does nothing.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void broadcastUnlock();


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
     * Unlock the cube for writing data. If the cube is already unlocked,
     * does nothing.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void unlock();


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


    /**
     * Clears the cube. Sets all pixel values to `false`.
     */
    void clear();

    /**
     * Sets the specified pixel value to the value of `on`. 
     * 
     * The `x`, `y`, and `z` parameters are used as coordinates in the cube 
     * by dividing each by `scale` and rounding them off to the nearest integer.
     * For instance, if `500.0, 350.0, 125.0` are input as `x,y,z` and `100.0`
     * is input as the `scale`, then the voxel at `(5,4,1)` will be set to the
     * value of `on`.
     * 
     * @param x     x-coord
     * @param y     y-coord
     * @param z     z-coord
     * @param on    The value of the voxel to be set
     * @param scale The scale of the coordinates (optional)
     */
    void setVoxel(float x, float y, float z, bool on, float scale = 1.0f);

    /**
     * Set the voxel at the given coordinate.
     *
     * There is no scaling for this command. If you use it, it is assumed you
     * want to control the voxels directly.
     * @param x  the x coord
     * @param y  the y coord
     * @param z  the z coord
     * @param on `bool` for the value of the voxel
     */
    void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool on);

    void voxelOn(float x, float y, float z, float scale = 1.0f);

    void voxelOn(uint8_t x, uint8_t y, uint8_t z);

    void voxelOff(uint8_t x, uint8_t y, uint8_t z);

    void voxelOff(float x, float y, float z, float scale = 1.0f);

    bool getVoxel(uint8_t x, uint8_t y, uint8_t z) const;

    /**
     * Draws a line segment from `(x0,y0,z0)` to `(x1,y1,z1)`.
     * @param x0    x0
     * @param y0    y0
     * @param z0    z0
     * @param x1    c1
     * @param y1    y1
     * @param z1    z1
     * @param scale pre-divisor of the coordinates (optional)
     */
    void drawLine(
        float x0, 
        float y0, 
        float z0, 
        float x1, 
        float y1, 
        float z1,
        float scale = 1.0f);

    /**
     * Draws an yz-plane at the designated x-index.
     * @param x the x-index of the matrix.
     */
    void drawXPlane(uint8_t x);

    /**
     * Draws an xz-plane at the designated y-index.
     * @param y the y-index of the matrix.
     */
    void drawYPlane(uint8_t y);
    
    /**
     * Draws an xy-plane at the designated z-index.
     * @param z the z-index of the matrix.
     */
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
