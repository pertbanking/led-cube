/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * Author: Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBE_CPP_
#define LEDCUBE_CPP_

#include "LEDCube.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdint>
#include <cstdio>

#include <serial/serial.h>


using namespace std;

// comment out to squelch debug messages to the console.
// #define CUBE_DEBUG


// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================


LEDCube* LEDCube::instance = nullptr;

// private, do not use
LEDCube::LEDCube() {}

// private, do not use
LEDCube::LEDCube(const LEDCube&) {}

// private, do not use
const LEDCube& LEDCube::operator=(const LEDCube&) {}

LEDCube::LEDCube(shared_ptr<serial::Serial>& usb, int framerate, string magic)
    : framerate(framerate)
    , data(
        vector<vector<vector<bool>>>(
            CUBE_SIZE, vector<vector<bool>>(
                CUBE_SIZE, vector<bool>(
                    CUBE_SIZE, false)
                )
            )
        )
    , magic(magic)
    , usb_message(new uint8_t[CUBE_SIZE*CUBE_SIZE + 1 + magic.size()])
    , data_current(true)
    , cube_pass(
        vector<vector<uint8_t>>(
            CUBE_SIZE, vector<uint8_t>(
                CUBE_SIZE, 0)
            )
        )
    , usb(usb)
    , render_thread()
    , data_m()
    , data_lock(this->data_m, std::defer_lock)
    , broadcast_lock(this->data_m, std::defer_lock)
    , render_start_m()
    , transmit_cv()
    , thread_pause(true)
    , thread_kill(false) {

    // start the thread
    render_thread = std::thread([this](void) {

        while(!this->thread_kill) {
            // TODO: Have a non-static wait period.
            // get the lock in the scope, so when it gets destroyed, we release
            // the mutex.
            {
                std::unique_lock<std::mutex> rendering_lock(
                    this->render_start_m, 
                    std::defer_lock
                );
                this->transmit_cv.wait(
                    rendering_lock, 
                    [this]() -> bool { return !this->thread_pause; }
                );
                this->usbSend();
            }
            this->transmit_cv.notify_one();

            if (this->thread_kill)
                break;

            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    int(1000.0 / float(this->framerate) + 0.5) // DEBUG: Changed to 10,000 ms
                )
            );
        }
    });

    render_thread.detach();

    usb_message[magic.size() + CUBE_SIZE*CUBE_SIZE] = '\n';
}

LEDCube::~LEDCube() {
    // destroy allocated resources
    delete[] usb_message;
    
    // halt the render thread
    this->thread_kill = true;
}

LEDCube* LEDCube::getInstance() {
    return LEDCube::instance;
}

LEDCube* LEDCube::getInstance(
    shared_ptr<serial::Serial> usb, 
    int framerate,
    string magic) {

    if (LEDCube::instance == nullptr)
        LEDCube::instance = new LEDCube(usb, framerate, magic);

    return LEDCube::instance;
}

void LEDCube::destroyInstance() {
    delete LEDCube::instance;
    LEDCube::instance = nullptr;
}


// =======================================
// timing and I/O control
// =======================================


void LEDCube::usbSend() {
    std::copy(
        this->magic.c_str(), 
        this->magic.c_str() + 
        this->magic.size(),
        usb_message);

    this->broadcastLock();
    this->getCubeData();
    this->broadcastUnlock();

    for (auto i = this->cube_pass.begin(); i != this->cube_pass.end(); ++i) {
        std::copy(
            i->begin(), 
            i->end(), 
            this->usb_message + 
                this->magic.size() + 
                i->size()*(i - this->cube_pass.begin())
        );
    }

    usb->write(this->usb_message, this->magic.size() + CUBE_SIZE*CUBE_SIZE + 1);
}

void LEDCube::broadcastLock() {
    this->broadcast_lock.lock();
}

void LEDCube::broadcastUnlock() {
    this->broadcast_lock.unlock();
}


void LEDCube::startBroadcast() {
    if (this->thread_pause) {
        std::unique_lock<std::mutex> rendering_lock(
            this->render_start_m, 
            std::defer_lock
        );
        this->thread_pause = false;
    }
    this->transmit_cv.notify_one();
}

void LEDCube::pauseBroadcast() {
    if (!this->thread_pause) {
        std::unique_lock<std::mutex> rendering_lock(
            this->render_start_m, 
            std::defer_lock
        );
        this->transmit_cv.wait(rendering_lock);
        this->thread_pause = true;
    }
}

bool LEDCube::isBroadcasting() {
    return !this->thread_pause;
}

void LEDCube::setFramerate(int f) {
    this->framerate = f;
}

int LEDCube::getFramerate() const {
    return this->framerate;
}


void LEDCube::lock() {
    this->data_lock.lock();
}

void LEDCube::unlock() {
    this->data_lock.unlock();
}



// ===============================================
// data management functions
// ===============================================

vector<vector<vector<bool>>>& LEDCube::getCube() {
    return this->data;
}

const vector<vector<uint8_t>>& LEDCube::getCubeData() {
    if (!(this->data_current)) {
        // clear the data for the cube
        this->cube_pass = vector<vector<uint8_t>>(
            CUBE_SIZE, vector<uint8_t>(CUBE_SIZE, 0)
        );

        // convert the cube bool into the proper x,y,z uint8_t array
        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {
                for (int k = 0; k < CUBE_SIZE; ++k) {
                    this->cube_pass[k][j] += this->data[i][j][k] << i;
                }
            }
        }
    }

    #ifdef CUBE_DEBUG
    for (int i = CUBE_SIZE-1; i >= 0; --i) {
        for (int j = 0; j < CUBE_SIZE; ++j) {
            printf(" %X", cube_pass[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    #endif

    return this->cube_pass;
}



// ==========================================
// pixel manipulation functions
// ==========================================

void LEDCube::clear() {
    // don't forget this!
    this->data_current = false;

    this->data = vector<vector<vector<bool>>>(
        CUBE_SIZE, vector<vector<bool>>(
            CUBE_SIZE, vector<bool>(
                CUBE_SIZE, false)
            )
        );
}

void LEDCube::setVoxel(uint8_t x, uint8_t y, uint8_t z, bool on) {
    // don't forget this!
    this->data_current = false;

    this->data[x][y][z] = on;
}

void LEDCube::setVoxel(float x, float y, float z, bool on, float scale) {
    // don't forget this!
    this->data_current = false;

    this->data[int(x/scale + 0.5)][int(y/scale + 0.5)][int(z/scale + 0.5)] = on;
}

void LEDCube::voxelOn(uint8_t x, uint8_t y, uint8_t z) {
    this->setVoxel(x, y, z, true);
}

void LEDCube::voxelOn(float x, float y, float z, float scale) {
    this->setVoxel(x, y, z, true, scale);
}

void LEDCube::voxelOff(uint8_t x, uint8_t y, uint8_t z) {
    this->setVoxel(x, y, z, false);
}

void LEDCube::voxelOff(float x, float y, float z, float scale) {
    this->setVoxel(x, y, z, false, scale);
}

bool LEDCube::getVoxel(uint8_t x, uint8_t y, uint8_t z) const {
    return this->data[x][y][z];
}

void LEDCube::drawLine(
    float x0,
    float y0,
    float z0,
    float x1,
    float y1,
    float z1,
    float scale) {

    x0 /= scale;
    x1 /= scale;
    y0 /= scale;
    y1 /= scale;
    z0 /= scale;
    z1 /= scale;

    vector< vector<bool> > xyplane(CUBE_SIZE, vector<bool>(CUBE_SIZE, false));
    vector< vector<bool> > xzplane(CUBE_SIZE, vector<bool>(CUBE_SIZE, false));
    vector< vector<bool> > yzplane(CUBE_SIZE, vector<bool>(CUBE_SIZE, false));

    // set (x1,y1,z1) to true
    xyplane[int(x1 + 0.5)][int(y1 + 0.5)] = true;
    xzplane[int(x1 + 0.5)][int(z1 + 0.5)] = true;
    yzplane[int(y1 + 0.5)][int(z1 + 0.5)] = true;

    // goal: traverse each axis and follow the projected line on each plane.
    //       each projected line has a slope. we move in the other axes
    //       proportional to the slope, just like we would a normal line.
    //       if every plane marks a point as true on its respective axis, 
    //       then the point is illuminated.

    float x_prog, y_prog, z_prog;

    if (abs(x0 - x1) > 0.0001) {
        // xy-plane and xz-plane
        y_prog = y0;
        z_prog = z0;
        float xy_slope = (y0 - y1) / (x0 - x1);
        float xz_slope = (z0 - z1) / (x0 - x1);
        for (
          x_prog = x0;
          (x0 < x1)? x_prog < x1 : x_prog > x1;
          x_prog += (x0 < x1)? 1.0 : -1.0) {
            xyplane[int(x_prog + 0.5)][int(y_prog + 0.5)] = true;
            xzplane[int(x_prog + 0.5)][int(z_prog + 0.5)] = true;
            y_prog += (x0 < x1)? xy_slope : -xy_slope;
            z_prog += (x0 < x1)? xz_slope : -xz_slope;
        }
    }

    if (abs(y0 - y1) > 0.0001) {
        // xy-plane and yz-plane
        x_prog = x0;
        z_prog = z0;
        float yx_slope = (x0 - x1) / (y0 - y1);
        float yz_slope = (z0 - z1) / (y0 - y1);
        for (
          y_prog = y0;
          (y0 < y1)? y_prog < y1 : y_prog > y1;
          y_prog += (y0 < y1)? 1.0 : -1.0) {
            xyplane[int(x_prog + 0.5)][int(y_prog + 0.5)] = true;
            yzplane[int(y_prog + 0.5)][int(z_prog + 0.5)] = true;
            x_prog += (y0 < y1)? yx_slope : -yx_slope;
            z_prog += (y0 < y1)? yz_slope : -yz_slope;
        }
    }

    if (abs(z0 - z1) > 0.0001) {
        // xz-plane and yz-plane
        x_prog = x0;
        y_prog = y0;
        float zx_slope = (x0 - x1) / (z0 - z1);
        float zy_slope = (y0 - y1) / (z0 - z1);
        for (
          z_prog = z0;
          (z0 < z1)? z_prog < z1 : z_prog > z1;
          z_prog += (z0 < z1)? 1.0 : -1.0) {
            xzplane[int(x_prog + 0.5)][int(z_prog + 0.5)] = true;
            yzplane[int(y_prog + 0.5)][int(z_prog + 0.5)] = true;
            x_prog += (z0 < z1)? zx_slope : -zx_slope;
            y_prog += (z0 < z1)? zy_slope : -zy_slope;
        }
    }

    // cout << "xy: " << endl;
    // for (int i = 0; i < CUBE_SIZE; ++i) {
    //     for (int j = 0; j < CUBE_SIZE; ++j) {
    //         cout << xyplane[i][j];
    //     }
    //     cout << endl;
    // }

    // cout << "xz: " << endl;
    // for (int i = 0; i < CUBE_SIZE; ++i) {
    //     for (int j = 0; j < CUBE_SIZE; ++j) {
    //         cout << xzplane[i][j];
    //     }
    //     cout << endl;
    // }

    // cout << "yz: " << endl;
    // for (int i = 0; i < CUBE_SIZE; ++i) {
    //     for (int j = 0; j < CUBE_SIZE; ++j) {
    //         cout << yzplane[j][i];
    //     }
    //     cout << endl;
    // }

    // if the planes all agree, illuminate the voxel
    for (uint8_t i = 0; i < CUBE_SIZE; ++i) {
        for (uint8_t j = 0; j < CUBE_SIZE; ++j) {
            if (xyplane[i][j]) {
                for (uint8_t k = 0; k < CUBE_SIZE; ++k) {
                    if (xzplane[i][k] and yzplane[j][k])
                        this->voxelOn(i,j,k);
                }
            }
        }
    }

}

void LEDCube::drawXPlane(uint8_t x) {
    // don't forget this!
    this->data_current = false;

    for (int i = 0; i < CUBE_SIZE; ++i)
        for (int j = 0; j < CUBE_SIZE; ++j)
            data[x][i][j] = true;
}

void LEDCube::drawYPlane(uint8_t y) {
    // don't forget this!
    this->data_current = false;

    for (int i = 0; i < CUBE_SIZE; ++i)
        for (int j = 0; j < CUBE_SIZE; ++j)
            data[i][y][j] = true;
}
    
void LEDCube::drawZPlane(uint8_t z) {
    // don't forget this!
    this->data_current = false;

    for (int i = 0; i < CUBE_SIZE; ++i)
        for (int j = 0; j < CUBE_SIZE; ++j)
            data[i][j][z] = true;
}

void LEDCube::drawBox(
    float x0, 
    float y0, 
    float z0, 
    float x1, 
    float y1, 
    float z1, 
    bool filled,
    float scale) {

    // TODO: Implement a 'filled' algorithm
    // TODO: Do NOT use the drawLine method here. It is inefficient, since
    //       it generalizes to line segments not parallel to the axes.
    this->drawLine(x0, y0, z0, x1, y0, z0);
    this->drawLine(x0, y0, z0, x0, y1, z0);
    this->drawLine(x0, y0, z0, x0, y0, z1);
    this->drawLine(x1, y0, z0, x1, y0, z1);
    this->drawLine(x1, y0, z1, x0, y0, z1);
    this->drawLine(x0, y1, z0, x0, y1, z1);
    this->drawLine(x0, y1, z1, x0, y0, z1);
    this->drawLine(x0, y1, z0, x1, y1, z0);
    this->drawLine(x1, y0, z0, x1, y1, z0);
    this->drawLine(x0, y1, z1, x1, y1, z1);
    this->drawLine(x1, y0, z1, x1, y1, z1);
    this->drawLine(x1, y1, z0, x1, y1, z1);
}


#endif
