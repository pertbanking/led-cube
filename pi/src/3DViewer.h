/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef THREEDVIEWER_H_
#define THREEDVIEWER_H_


#include <serial/serial.h>

#include "LEDCube.h"


class LEDCubeVisualizer : public LEDCube {

private:

    static LEDCubeVisualizer* instance;

    // the render_start_m mutex is used only by the transmit_cv condition_variable.
    // it is used to halt the render thread at the stopBroadcast() call and 
    // continue it at the startBroadcast() call.
    std::mutex render_start_m;
    std::condition_variable transmit_cv;

    bool thread_pause; //< Set and unset by the pauseBroadcast() function. Use
                       //< in your thread object body.
    bool thread_kill; //< Only unset in the destroyInstance() function. Use
                      //< in your thread object body.
    
    // private, use only indirectly (i.e. in getInstance())
    LEDCubeVisualizer(int message_rate);

    // private, do not use
    ~LEDCubeVisualizer();
    LEDCubeVisualizer(const LEDCubeVisualizer&);
    const LEDCubeVisualizer& operator=(const LEDCubeVisualizer&);


public:

    /**
     * Returns a new instance of the cube with the given parameters if there 
     * doesn't already exist one; otherwise ignores all parameters and returns
     * the current instance of the cube.
     * 
     * @param  framerate The desired framerate of the cube.
     * @return           The instance of the cube (new or old)
     */
    static LEDCubeVisualizer* getInstance(int framerate = 60);

    /**
     * Destroy this instance of `LEDCubeVisualizer` and set the instance value 
     * to `nullptr`. If there is no instance, does nothing.
     */
    static void destroyInstance();

    /**
     * @brief Display the visualization window.
     */
    void show();

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
