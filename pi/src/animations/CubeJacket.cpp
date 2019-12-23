/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef SNAKESOLVER_CPP_
#define SNAKESOLVER_CPP_

#include "../Animation.h"

#include <pair>
#include <vector>
#include <cmath>


const std::string CUBEJACKET_NAME = "Cube jacket";
const int CUBEJACKET_FRAMERATE = 60;
const int CUBEJACKET_DURATION = 500;

/**
 * Makes lit and un-lit layers of the cube appear in an interesting pattern
 */
class CubeJacket : public Animation {
private:
    int percolating_layer;
    int sustained_layer;

public:

    CubeJacket()
    : Animation(
        CUBEJACKET_NAME, 
        CUBEJACKET_FRAMERATE, 
        CUBEJACKET_DURATION)
    , percolating_layer(1)
    , sustained_layer(0)
    {}

    void calculateNext(LEDCube* cube) {
        cube->clear();

        // TODO: Draw the cube, given the sustained layer.

        if (sustained_layer < 5) {
            float z0 = 4.5 - static_cast<float>(percolating_layer);
            float z1 = z0 + static_cast<float>(percolating_layer);
            for (float i = z0; i < z1 - 0.001; ++i) {
                for (float j = z0; j < z1 - 0.001; ++j) {
                    cube->drawLine(i, j, z0, i, j, z1);
                }
            }
        }
    }
}