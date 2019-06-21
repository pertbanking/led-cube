/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * Author: Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */


#include "LEDCube.h"

#include <memory>
#include <vector>
#include <cassert>
#include <cstdint>

#include <serial/serial.h>
#include "timercpp.h"


using namespace std;


// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

const int SIZE = 8;

LEDCube::instance = nullptr;

LEDCube::LEDCube() {}

LEDCube::LEDCube(const LEDCube&) {}

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
            SIZE, vector<uint8_t>(SIZE, 0)
            )
        )
    , usb(usb)
    , magic(magic)
    , data_current(true)
    , framerate(framerate)
    , animation(Animation()) {

}

LEDCube::~LEDCube() {
    this->stop();
}

LEDCube::getInstance() {
    return LEDCube::instance;
}

LEDCube::getInstance(
    shared_ptr<serial::Serial> usb, 
    int framerate,
    string magic) {

    if (LEDCube::instance == nullptr)
        LEDCube::instance = new LEDCube(usb, framerate, magic);

    return this->getInstance();
}



// =======================================
// timing and I/O control
// =======================================

void LEDCube::usbSend() {

    int mbl = this->magic.length();
    uint8_t message[SIZE*SIZE + 1 + mbl];

    this->getCubeData();

    std::copy(this->magic.c_str(), this->magic.c_str() + mbl, message);

    for (auto i = this->cube_pass.begin(); i != this->cube_pass.end(); ++i) {
        std::copy(
            i->begin(), 
            i->end(), 
            message + mbl + i->size()*(i - this->cube_pass.begin())
        );
    }

    message[mbl + SIZE*SIZE] = '\n';

    usb->write(message, mbl + SIZE*SIZE + 1);
}


void LEDCube::start() {
    t.setInterval([]() {
        // send the cube
        this->usb_send();
        this->animation.next();

    }, int(1000.0 / double(this->framerate))); 
}

void LEDCube::stop() {
    this->t.stop();
}

void LEDCube::setFramerate(int f) {
    this->stop();
    this->framerate = f;
    this->start();
}

int LEDCube::getFramerate() const {
    return this->framerate;
}


void LEDCube::setAnimation(Animation &a) {
    this->animation = a;
}

Animation& LEDCube::getAnimation() const {
    return this->animation;
}



// ===============================================
// data management functions
// ===============================================

vector<vector<vector<bool>>>& LEDCube::getCube() const {
    return this->data;
}

vector<vector<uint8_t>> getCubeData() const {
    if (!this->data_current) {
        assert(this->data.size() == SIZE)
        // convert the cube bool into the proper x,y,z uint8_t array
        for (int i = 0; i < SIZE; ++i) {
            assert(this->data[i].size() == SIZE)
            for (int j = 0; j < SIZE; ++j) {
                assert(this->data[i][j].size() == SIZE)
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

