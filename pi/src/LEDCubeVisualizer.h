/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBEVISUALIZER_H_
#define LEDCUBEVISUALIZER_H_


#include "LEDCube.h"


class LEDCubeVisualizer : public LEDCube {

private:

    static LEDCubeVisualizer* instance;
    
    // private, use only indirectly (i.e. in getInstance())
    LEDCubeVisualizer(int message_rate);

    // private, do not use
    ~LEDCubeVisualizer();
    LEDCubeVisualizer(const LEDCubeVisualizer&);
    const LEDCubeVisualizer& operator=(const LEDCubeVisualizer&);

    /**
     * [drawGrid description]
     */
    static void drawGrid();

    /**
     * [display description]
     */
    static void display();

    /**
     * [keyboard description]
     * @param key [description]
     * @param x   [description]
     * @param y   [description]
     */
    static void keyboard(unsigned char key, int x, int y);


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

};


#endif
