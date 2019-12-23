/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBEVISUALIZER_H_
#define LEDCUBEVISUALIZER_H_

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "LEDCube.h"


class LEDCubeVisualizer : public LEDCube {

private:

    GLFWwindow* window;

    static LEDCubeVisualizer* instance;

    static GLfloat cube_vertices[];
    static GLfloat cube_colors[];
    static const int COLOR_LENGTH;

    // private, use only indirectly (i.e. in getInstance())
    LEDCubeVisualizer(int message_rate);

    // private, do not use
    ~LEDCubeVisualizer();
    LEDCubeVisualizer(const LEDCubeVisualizer&);
    const LEDCubeVisualizer& operator=(const LEDCubeVisualizer&);

    /**
     * [display description]
     */
    static void display();

    /**
     * [drawGrid description]
     */
    static void drawGrid();

    /**
     * [drawVoxel description]
     * @param x  [description]
     * @param y  [description]
     * @param z  [description]
     * @param on [description]
     */
    static void drawVoxel(int x, int y, int z, bool on);

    /**
     * [keyboard description]
     * @param window   [description]
     * @param key      [description]
     * @param scancode [description]
     * @param action   [description]
     * @param mods     [description]
     */
    static void keyboard(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );


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
