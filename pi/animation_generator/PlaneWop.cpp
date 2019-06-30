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


class PlaneWop : public Animation {
private:
    int planedir;
    int layer;
    bool up;
    PlaneWop() : planedir(0), layer(0), up(false) {
    }

    void _next(LEDCube* cube) {  // override _next(), NOT next().
        if (planedir == 0) {
            if (planedir == 0)
                cube->drawZPlane(layer);
            else if (planedir == 1)
                cube->drawYPlane(layer);
            else
                cube->drawXPlane(layer);
            if (layer == 7)
                up = false;
            if (layer == 0 && !up)
                planedir = (planedir + 1) % 3
            if (up)
                ++layer;
            else
                --layer;
            this->rest = 5;
        }
    }
}