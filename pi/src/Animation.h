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
    const string name;
    int restDuration;

    // you can't use this constructor!
    Animation();

protected:

    int frame;  //< The current animation's frame. Change this if you're
                //< resetting your animation (like in `reset()`).
    int recommendedFramerate;  //< The recommended framerate for this animation
    int recommendedDuration;   //< The recommended lifetime for this animation
    
    /**
     * @brief <b>Override this method to make your own animations!</b>
     *
     * Note that this is different from the `next()` method because this one
     * does not deal with the cube's mutex. The `next()` method will call this
     * method, after it has locked the cube and increasaed the frame.
     * 
     * @warning    Do not override the `next()` method. Override this method
     *             instead.
     * @warning    If your animation is too slow, the FPS of the cube will be
     *             affected. Make sure you have a fast way to calculate
     *             successive frames.
     * @warning    It is recommended that you clear the cube at the beginning
     *             of every new frame.
     * @param cube The current instance of the cube. Use this pointer to
     *             make changes within the animation.
     */
    virtual void calculateNext(LEDCube* cube) = 0;

    /**
     * @brief Set the animation to rest for a set number of frames.
     *
     * Note that setting `frames` will totally prevent the
     * calculateNext method from begin called, presumably for
     * `frames / recommendedFramerate` seconds (though the exact
     * number depends on the calling function).
     * 
     * @param frames The number of frames to rest for.
     */
    void rest(int frames) {
        this->restDuration = (frames < 0)? 0 : frames;
    }

    // protect the constructors because only real animations should have access.
    Animation(
        string name = "", 
        int recommendedFramerate = 50, 
        int recommendedDuration = 1000)
    : name(name)
    , restDuration(0)
    , frame(0)
    , recommendedFramerate(recommendedFramerate)
    , recommendedDuration(recommendedDuration) {}

    ~Animation() = default;

public:
    /**
     * @brief Steps the animation forward. 
     * 
     * Call this method in a regular program.
     * @param cube The `cube` instance.
     */
    void next(LEDCube* cube);

    /**
     * @brief Resets the animation to its beginning state. 
     * 
     * Every animation must override this method.
     */
    virtual void reset() = 0;

    /**
     * @return The recommended framerate for this animation.
     */
    int getRecommendedFramerate() const {
        return recommendedFramerate;
    }

    /**
     * @return The recommended lifespan of this animation (in frames)
     */
    int getRecommendedDuration() const {
        return recommendedDuration;
    }

    /**
     * @brief The current frame of the cube.
     *
     * Note that when the animation is initialized, the frame begins at 0,
     * and the first `next()` that is called is frame 1.
     * @return The frame of this animation
     */
    int getFrame() const {
        return this->frame;
    }

    /**
     * @brief The name of this animation.
     *
     * This should be passed in by a `super` call by the subclass
     * 
     * @return the name of the animation.
     * @see Animation#Animation
     */
    const string& getName() const {
        return this->name;
    }
};


#endif
