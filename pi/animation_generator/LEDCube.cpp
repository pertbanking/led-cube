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

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <cassert>
#include <cstdint>

#include <serial/serial.h>


using namespace std;


// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

const int CUBE_SIZE = 8;

LEDCube* LEDCube::instance = nullptr;

// private, do not use
LEDCube::LEDCube() {}

// private, do not use
LEDCube::LEDCube(const LEDCube&) {}

// private, do not use
const LEDCube& LEDCube::operator=(const LEDCube&) {}

LEDCube::LEDCube(shared_ptr<serial::Serial>& usb, int framerate, string magic)
    : data(
        vector<vector<vector<bool>>>(
            CUBE_SIZE, vector<vector<bool>>(
                CUBE_SIZE, vector<bool>(
                    CUBE_SIZE, false)
                )
            )
        )
    , cube_pass(
        vector<vector<uint8_t>>(
            CUBE_SIZE, vector<uint8_t>(
                CUBE_SIZE, 0)
            )
        )
    , usb(usb)
    , magic(magic)
    , usb_message(new uint8_t[CUBE_SIZE*CUBE_SIZE + 1 + magic.size()])
    , data_current(true)
    , framerate(framerate)
    , thread_pause(true)
    , render_function()
    , render_thread([this](void) {
        while(!this->thread_kill) {
            while(this->thread_pause and !this->thread_kill) ;  
                // wait for the startBroadcast() method
                // to be called
            if (this->thread_kill)
                break;
            this->usbSend();
            if (this->thread_kill)
                break;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    int(1000.0 / double(this->framerate))
                )
            );
        }
      })
    , render_lock()
    , blocking_thread() {

    // std::cout << "Detaching..." << std::endl;

    render_thread.detach();

    usb_message[magic.size() + CUBE_SIZE*CUBE_SIZE] = '\n';
}

LEDCube::~LEDCube() {
    std::cout << "Destroying LEDCube instance..." << std::endl;

    // destroy allocated resources
    delete[] usb_message;
    
    // halt the render thread
    this->thread_kill = true;

    std::cout << "Done." << std::endl;
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
    
    this->lock();
    this->getCubeData();
    this->unlock();

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


void LEDCube::startBroadcast() {
    this->thread_pause = false;
}

void LEDCube::pauseBroadcast() {
    this->thread_pause = true;
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
    this->render_lock.lock();
    this->blocking_thread = this_thread::get_id();
}

bool LEDCube::isLocked() const {
    return this->blocking_thread != thread::id(); // compare to the default
}

void LEDCube::unlock() {
    if (this->blocking_thread == this_thread::get_id()) {
        this->render_lock.unlock();
        this->blocking_thread = thread::id(); // set back to the default
    }
}



// ===============================================
// data management functions
// ===============================================

vector<vector<vector<bool>>>& LEDCube::getCube() {
    return this->data;
}

const vector<vector<uint8_t>>& LEDCube::getCubeData() {
    if (!(this->data_current)) {
        assert(this->data.size() == CUBE_SIZE);
        // convert the cube bool into the proper x,y,z uint8_t array
        for (int i = 0; i < CUBE_SIZE; ++i) {
            assert(this->data[i].size() == CUBE_SIZE);
            for (int j = 0; j < CUBE_SIZE; ++j) {
                assert(this->data[i][j].size() == CUBE_SIZE);
                for (int k = 0; k < CUBE_SIZE; ++k) {
                    this->cube_pass[k][j] += this->data[i][j][k] << i;
                }
            }
        }
    }
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


#endif
