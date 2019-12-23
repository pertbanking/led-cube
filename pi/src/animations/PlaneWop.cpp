/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef PLANEWOP_CPP_
#define PLANEWOP_CPP_

#include "../Animation.h"


const std::string PLANEWOP_NAME = "PlaneWop";

class PlaneWop : public Animation {
private:
    int planedir;
    int layer;
    bool up;
public:
    PlaneWop()
    : Animation(PLANEWOP_NAME, 50, 120)
    , planedir(0)
    , layer(0)
    , up(true) {}

    void calculateNext(LEDCube* cube) {  // override calculateNext(), NOT next().
        // clear the cube
        cube->clear();
        
        if (up)
            ++layer;
        else
            --layer;

        if (layer == 7) {
            up = false;
        }
        if (layer == 0 && !up) {
            planedir = (planedir + 1) % 3;
            up = true;
        }

        if (planedir == 0)
            cube->drawZPlane(layer);
        else if (planedir == 1)
            cube->drawYPlane(layer);
        else
            cube->drawXPlane(layer);
        // this->rest(1);
    }

    void reset() {
        this->frame = 0;

        planedir = 0;
        layer = 0;
        up = true;
    }
};

#endif
