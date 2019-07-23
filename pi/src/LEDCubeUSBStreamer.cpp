/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBEUSBSTREAMER_CPP_
#define LEDCUBEUSBSTREAMER_CPP_

#include "LEDCubeUSBStreamer.h"

#include <algorithm>
#include <chrono>
#include <memory>
#include <cstdint>

#include <serial/serial.h>


LEDCubeUSBStreamer* LEDCubeUSBStreamer::instance = nullptr;

// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

LEDCubeUSBStreamer::LEDCubeUSBStreamer(
    int message_rate, 
    shared_ptr<serial::Serial> usb, 
    string magic)
        : LEDCube(message_rate)
        , magic(magic)
        , usb_message(new uint8_t[CUBE_SIZE*CUBE_SIZE + 1 + magic.size()])
        , usb(usb)
        , render_start_m()
        , transmit_cv()
        , thread_pause(true)
        , thread_kill(false) {

    // start the thread
    render_thread = std::thread([this](void) {

        while(!this->thread_kill) {
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

LEDCubeUSBStreamer::~LEDCubeUSBStreamer() {
    // destroy allocated resources
    delete[] usb_message;
    
    // halt the render thread
    this->thread_kill = true;
}

LEDCubeUSBStreamer* LEDCubeUSBStreamer::getInstance() {
    return LEDCubeUSBStreamer::instance;
}

LEDCubeUSBStreamer* LEDCubeUSBStreamer::getInstance(
    shared_ptr<serial::Serial> usb, 
    int framerate,
    string magic) {

    if (LEDCubeUSBStreamer::instance == nullptr)
        LEDCubeUSBStreamer::instance
            = new LEDCubeUSBStreamer(framerate, usb, magic);

    return LEDCubeUSBStreamer::instance;
}

void LEDCubeUSBStreamer::destroyInstance() {
    if (LEDCubeUSBStreamer::instance != nullptr) {
        delete LEDCubeUSBStreamer::instance;
        LEDCubeUSBStreamer::instance = nullptr;
    }
}


// =======================================
// timing and I/O control
// =======================================

void LEDCubeUSBStreamer::usbSend() {
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

void LEDCubeUSBStreamer::startBroadcast() {
    if (this->thread_pause) {
        std::unique_lock<std::mutex> rendering_lock(
            this->render_start_m, 
            std::defer_lock
        );
        this->thread_pause = false;
    }
    this->transmit_cv.notify_one();
}

void LEDCubeUSBStreamer::pauseBroadcast() {
    if (!this->thread_pause) {
        std::unique_lock<std::mutex> rendering_lock(
            this->render_start_m, 
            std::defer_lock
        );
        this->transmit_cv.wait(rendering_lock);
        this->thread_pause = true;
    }
}

bool LEDCubeUSBStreamer::isBroadcasting() {
    return !this->thread_pause;
}

#endif
