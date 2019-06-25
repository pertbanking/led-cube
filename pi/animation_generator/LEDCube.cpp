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

const int SIZE = 8;

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
            SIZE, vector<vector<bool>>(
                SIZE, vector<bool>(
                    SIZE, false)
                )
            )
        )
    , cube_pass(
        vector<vector<uint8_t>>(
            SIZE, vector<uint8_t>(
                SIZE, 0)
            )
        )
    , usb(usb)
    , magic(magic)
    , usb_message(new uint8_t[SIZE*SIZE + 1 + magic.size()])
    , data_current(true)
    , framerate(framerate)
    , thread_pause(true)
    , render_function([this]() {
        while(!this->thread_kill) {
            while(this->thread_pause and !thread_kill) ;  
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
    , render_thread(render_function)
    , render_lock()
    , blocking_thread() {

    render_thread.detach();

    usb_message[magic.size() + SIZE*SIZE] = '\n';
}

LEDCube::~LEDCube() {
    delete[] usb_message;
    this->thread_kill = true;
    this->render_thread.join();

    this->instance = nullptr;
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

    usb->write(this->usb_message, this->magic.size() + SIZE*SIZE + 1);
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
        assert(this->data.size() == SIZE);
        // convert the cube bool into the proper x,y,z uint8_t array
        for (int i = 0; i < SIZE; ++i) {
            assert(this->data[i].size() == SIZE);
            for (int j = 0; j < SIZE; ++j) {
                assert(this->data[i][j].size() == SIZE);
                for (int k = 0; k < SIZE; ++k) {
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

void LEDCube::clearCube() {
    // don't forget this!
    this->data_current = false;

    this->data = vector<vector<vector<bool>>>(
        SIZE, vector<vector<bool>>(
            SIZE, vector<bool>(
                SIZE, false)
            )
        );
}

void LEDCube::setVoxel(float x, float y, float z, bool on, float scale) {
    // don't forget this!
    this->data_current = false;

    this->data[int(x/scale + 0.5)][int(y/scale + 0.5)][int(z/scale + 0.5)] = on;
}


#endif
