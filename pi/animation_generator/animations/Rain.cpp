/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef RAIN_CPP_
#define RAIN_CPP_

#include "../Animation.h"

#include <algorithm>
#include <random>
#include <vector>


const std::string RAIN_NAME = "Rain";

class Rain : public Animation {
private:
    // initialize private variables here
    std::vector<std::vector<float>> drops;  // drops in the cube
    std::default_random_engine generator;
    std::uniform_int_distribution<int> drop_generator;
    std::uniform_real_distribution<float> drop_placer;

public:
    Rain()
    : Animation(RAIN_NAME)  // make sure to call this previous constructor with
                         // its name designation, as shown.
    , drops()
    , generator()
    , drop_generator(1, 10)
    , drop_placer(0.0, 7.4) {
        this->recommendedFramerate = 60;
    }

    void calculateNext(LEDCube* cube) {  // override calculateNext(), NOT next().

        // clear the cube
        cube->clear();
        // draw the 'ground'
        cube->drawZPlane(0);

        if (drop_generator(generator) > 7) {  // 2/10 chance of making a drop
            // make a new drop
            drops.push_back(std::vector<float> { 
                drop_placer(generator), 
                drop_placer(generator), 
                7.49
            });
        }
        while (drops.size() > 0 && drops[0][2] < -0.5) {
            drops.erase(drops.begin());
        }
        std::for_each(drops.begin(), drops.end(), [&cube] (std::vector<float>& drip) {
            cube->voxelOn(drip[0], drip[1], drip[2]);
            drip[2] -= 0.25;  // drops loose 0.14 height every 1/fps secs (8.4 @60fps, 4.2 @30fps)
        });
    }

    void reset() {
        // empty the drops vector
        drops = std::vector<std::vector<float>>();
    }
};

#endif
