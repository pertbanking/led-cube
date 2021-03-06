/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef WAVES_I_CPP_
#define WAVES_I_CPP_

#include "../Animation.h"

#include <algorithm>
#include <cmath>


const std::string WAVES_I_NAME = "Waves (sine * cosine)";
const int WAVES_FRAMERATE = 50;
const int WAVES_DURATION = 700;

/**
 * Broadcasts the shape of sin*cos
 */
class Waves_I : public Animation {
private:
    double sin_phase;
    double cos_phase;
    double sin_freq;
    double cos_freq;


public:
    Waves_I ()
    : Animation(WAVES_I_NAME, WAVES_FRAMERATE, WAVES_DURATION)
    , sin_phase(0.0)
    , cos_phase(0.0)
    , sin_freq(0.4)
    , cos_freq(0.35) {

    }

    void calculateNext(LEDCube* cube) {
        cube->clear();

        // equation for each point is 3.5 + 4 * sin(wx+theta*t) * cos(wy+theta*t)

        // sin(wt + theta)
        sin_phase = double(this->getFrame()) / 30.0;  // theta = t (seconds)
        // cos(wt + theta)
        cos_phase = double(this->getFrame()) / 5.0;   // theta = t/10

        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {
                cube->voxelOn(
                    float(i),
                    float(j),
                    3.5 + 
                        4.0 
                        * sin(sin_freq*i + sin_phase)
                        * cos(cos_freq*j + cos_phase)
                );
            }
        }
    }

    void reset() {
        this->frame = 0;
    }
};

#endif //WAVES_I_CPP_
