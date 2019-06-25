/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "LEDCube.h"


using namespace std;


// forward declaration
class LEDCube;


class Animation {
private:
    int frame;
    int rest;
    const string name;

protected:
    /**
     * Override this method to make your own animations!
     *
     * Note that this is different from the `next()` method because this one
     * does not deal with the cube's mutex. This is a more pure implementation.
     * @warning    Do not override the `next()` method. Override this method.
     * @warning    If your animation is too slow, the FPS of the cube will be
     *             affected. Make sure you have a fast way to calculate
     *             successive frames.
     * @warning    It is recommended that you clear the cube at the beginning
     *             of every new frame.
     * @param cube The current instance of the cube. Use this pointer to
     *             make changes within the animation.
     */
    virtual void _next(LEDCube* cube) = 0;

    /**
     * Set the animation to rest for a set number of frames.
     * @param frames The number of frames to rest for.
     */
    void _rest(int frames) {
        this->rest = frames;
    }

public:

    Animation() = default;
    Animation(string name) : name(name), frame(0), rest(0) {}
    ~Animation() = default;

    /**
     * Steps this animation forward. 
     * 
     * Call this method in a regular program.
     * @param cube The `cube` instance.
     */
    void next(LEDCube* cube);

    /**
     * The current frame of the cube.
     *
     * Note that when the animation is initialized, the frame begins at 0,
     * and the first `next()` that is called 
     * @return 
     */
    int getFrame() const {
        return this->frame;
    }

    const string& getName() const {
        return this->name;
    }
};


#endif