/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBEUSBSTREAMER_H_
#define LEDCUBEUSBSTREAMER_H_


#include <serial/serial.h>

#include "LEDCube.h"


class LEDCubeUSBStreamer : public LEDCube {

private:

    static LEDCubeUSBStreamer* instance;

    std::shared_ptr<serial::Serial> usb;

    std::string magic;
    uint8_t* usb_message;

    // the render_start_m mutex is used only by the transmit_cv condition_variable.
    // it is used to halt the render thread at the stopBroadcast() call and 
    // continue it at the startBroadcast() call.
    std::mutex render_start_m;
    std::condition_variable transmit_cv;

    bool thread_pause; //< Set and unset by the pauseBroadcast() function. Use
                       //< in your thread object body.
    bool thread_kill; //< Only unset in the destroyInstance() function. Use
                      //< in your thread object body.
    
    LEDCubeUSBStreamer(
        int message_rate,
        shared_ptr<serial::Serial> usb,
        string magic);

    // private, do not use
    ~LEDCubeUSBStreamer();
    LEDCubeUSBStreamer(const LEDCubeUSBStreamer&);
    const LEDCubeUSBStreamer& operator=(const LEDCubeUSBStreamer&);

    /**
     * @brief Send data to the cube's Arduino. 
     *
     * Takes the 8x8x8 bool array and converts it into a string of `uint8_t` bytes
     * with `this->magic` at the front and `'\n'` at the end. Then sends the data to
     * the Arduino.
     */
    void usbSend();


public:

    /**
     * Returns the current instance of the cube. Returns `nullptr` if there has
     * been no instance created or if the cube has been destroyed.
     * 
     * @return The instance of the cube; `nullptr` if it does not exist.
     */
    static LEDCubeUSBStreamer* getInstance();

    /**
     * Returns a new instance of the cube with the given parameters if there 
     * doesn't already exist one; otherwise ignores all parameters and returns
     * the current instance of the cube.
     * @param  usb       a `shared_ptr` to a `serial::Serial` object, which is 
     *                   where the LEDCube will broadcast to.
     * @param  framerate The desired framerate of the cube.
     * @param  magic     The magic bytes placed at the beginning of a 
     *                   transmission.
     * @return           The instance of the cube (new or old)
     */
    static LEDCubeUSBStreamer* getInstance(
        shared_ptr<serial::Serial> usb, 
        int framerate = 60, 
        string magic = "JANDY");

    /**
     * Destroy this instance of `LEDCubeUSBStreamer` and set the instance value 
     * to `nullptr`. If there is no instance, does nothing.
     */
    static void destroyInstance();

    /**
     * @brief Begin broadcasting to the cube at the set `message_rate`.
     * 
     * The data will begin to lock intermittently when this method is
     * called.
     */
    void startBroadcast();

    /**
     * @brief Stop broadcasting to the serial::Serial object.
     *
     * The data is guaranteed to no longer be locked after this method is 
     * called.
     */
    void pauseBroadcast();

    /**
     * @return `true` if the cube is broadcasting, `false` if it is not.
     */
    bool isBroadcasting();

};


#endif
