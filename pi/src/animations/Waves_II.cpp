/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef WAVES_II_CPP_
#define WAVES_II_CPP_

#include "../Animation.h"

#include <algorithm>
#include <cmath>


const std::string WAVES_II_NAME = "Waves (cos ripple)";
const int WAVESII_FRAMERATE = 50;
const int WAVESII_DURATION = 7000;

/**
 * Broadcasts a shape of cos(r)
 */
class Waves_II : public Animation {
private:
    double sin_freq;
    double cos_freq;
    static constexpr double CS2 = double(CUBE_SIZE-1) / 2.0;


public:
    Waves_II ()
    : Animation(WAVES_II_NAME, WAVESII_FRAMERATE, WAVESII_DURATION)
    , sin_freq(0.2)
    , cos_freq(1.5) {

    }

    void calculateNext(LEDCube* cube) {
        cube->clear();

        // equation for each point is:
        // 3.5 + 3.5 * (0.25sin(2pi*f_sin*t) + 0.75) * cos(r + 2pi*f_cos*t)
        // where
        // f_cos = cos_freq / framerate, and f_sin = sin_freq / framerate

        double dist;
        double val;
        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {

                dist = sqrt(
                    double(CS2-i)*double(CS2-i)
                    + double(CS2-j)*double(CS2-j)
                );

                val = CS2 +
                    CS2 * (
                        0.25*sin(2.0*M_PI*sin_freq*getFrame() 
                            / WAVESII_FRAMERATE
                        ) + 0.75
                    ) * cos(
                        dist
                        + 2.0*M_PI*cos_freq*getFrame()
                            / WAVESII_FRAMERATE
                    );

                if (val < 7.5 and val > -0.5) {
                    cube->voxelOn(
                        float(i),
                        float(j),
                        val
                    );
                }
            }
        }
    }

    void reset() {}
};

#endif //WAVES_II_CPP_
