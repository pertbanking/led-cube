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
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>


using namespace std;


const int CUBE_SIZE = 8;  //< The size of our cube


class LEDCube {

protected:

    int framerate;
    std::vector<std::vector<std::vector<bool>>> data;

    bool data_current;  //< true if the data in the `cube_pass` array represents
                        //< what is in the `data` array.
    std::vector<std::vector<uint8_t>> cube_pass;

    std::thread render_thread;  //< The thread that runs visualizer. You
                                //< <b>must</b> set this in the ctor.

    // the data_m mutex is shared between the data_lock and broadcast_lock
    // it's used to ensure the cube data is accessed one-at-a-time between the
    // render thread and the calculation thread.
    std::mutex data_m;
    std::unique_lock<std::mutex> data_lock;
    std::unique_lock<std::mutex> broadcast_lock;


    LEDCube(int message_rate);

    /**
     * @brief Lock the cube for transmitting data. 
     * 
     * If the cube is already locked, this method will block the current
     * thread's execution.
     * 
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void broadcastLock();

    /**
     * @brief Unlock the cube for transmitting data. 
     * 
     * If the cube is already unlocked, does nothing.
     * 
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting to the USB!! This method locks the mutex 
     *          of the render thread (ensuring its data won't change 
     *          midway in transmission), so it must be unlocked for the
     *          data to be accessed again.
     */
    void broadcastUnlock();


public:

    LEDCube() = delete;

    LEDCube(const LEDCube&) = delete;

    const LEDCube& operator=(const LEDCube&) = delete;

    /**
     * @brief Lock the cube for writing data. If the cube is already locked, this
     * method will block the current thread's execution.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting!! This method locks the mutex of the render thread
     *          (ensuring its data won't change  midway in transmission), so it
     *          must be unlocked for the data to be accessed again.
     */
    void lock();

    /**
     * @brief Unlock the cube for writing data. If the cube is already unlocked,
     * does nothing.
     * @warning You <b>must</b> call `unlock` for the cube to continue 
     *          broadcasting!! This method locks the mutex of the render thread
     *          (ensuring its data won't change  midway in transmission), so it
     *          must be unlocked for the data to be accessed again.
     */
    void unlock();


    /**
     * @brief Set the rate at which the cube data is parsed and sent to the 
     *        visualizer.
     *
     * @warning            Setting the rate too high can corrupt the data.
     * @param message_rate The desired message rate.
     */
    void setMessageRate(int message_rate);

    /**
     * @return The message rate of the cube.
     */
    int getMessageRate() const;

    /**
     * @brief Return a copy of the queued state of the cube.
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
     * @brief Clears the cube. Sets all pixel values to `false`.
     */
    void clear();

    /**
     * @brief Sets the specified pixel value to the value of `on`. 
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
     * @brief Set the voxel at the given coordinate.
     *
     * There is no scaling for this command. If you use it, it is assumed you
     * want to control the voxels directly.
     * @param x  the x coord
     * @param y  the y coord
     * @param z  the z coord
     * @param on `bool` for the value of the voxel
     */
    void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool on);

    void setVoxel(int x, int y, int z, bool on) {
        this->setVoxel(uint8_t(x), uint8_t(y), uint8_t(z), on);
    }

    void voxelOn(float x, float y, float z, float scale = 1.0f);

    void voxelOn(uint8_t x, uint8_t y, uint8_t z);
    
    void voxelOn(int x, int y, int z) {
        this->voxelOn(uint8_t(x), uint8_t(y), uint8_t(z));
    }

    void voxelOff(float x, float y, float z, float scale = 1.0f);

    void voxelOff(uint8_t x, uint8_t y, uint8_t z);

    void voxelOff(int x, int y, int z) {
        this->voxelOff(uint8_t(x), uint8_t(y), uint8_t(z));
    }

    bool getVoxel(uint8_t x, uint8_t y, uint8_t z) const;

    /**
     * @brief Draws a line segment from `(x0,y0,z0)` to `(x1,y1,z1)`.
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
     * @brief Draws an yz-plane at the designated x-index.
     * @param x the x-index of the matrix.
     */
    void drawXPlane(uint8_t x);

    /**
     * @brief Draws an xz-plane at the designated y-index.
     * @param y the y-index of the matrix.
     */
    void drawYPlane(uint8_t y);
    
    /**
     * @brief Draws an xy-plane at the designated z-index.
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

    /**
     * @brief Draws a box with faces parallel to the plane axes.
     * @param x0     x0
     * @param y0     y0
     * @param z0     z0
     * @param x1     x1
     * @param y1     y1
     * @param z1     z1
     * @param filled If `true`, all voxels between the faces are illuminated.
     * @param scale  The scale for the coordinates.
     */
    void drawBox(
        float x0, 
        float y0, 
        float z0, 
        float x1, 
        float y1, 
        float z1, 
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
