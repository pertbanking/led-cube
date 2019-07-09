/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef CUBEWOP_CPP_
#define CUBEWOP_CPP_

#include "../Animation.h"


const std::string CUBEWOP_NAME = "CubeWop";

class CubeWop : public Animation {
private:
    int corner;
    bool up;
    int x;
    int y;
    int z;

public:

    CubeWop() : Animation(CUBEWOP_NAME, 50, 700), corner(0), up(false) {}

    void calculateNext(LEDCube* cube) {
        cube->clear();

        // etc.
        // I'll come back to this later.
        switch (corner) {
            case 0:
            if (up) {
            }
            break;
            
            case 1:
            break;
            
            case 2:
            break;
            
            case 3:
            break;
            
            case 4:
            break;
            
            case 5:
            break;
            
            case 6:
            break;

            case 7:
            break;

            case 8:
            break;
        }

        // cube->drawLine(0.0, 0.0, 0.0, 0.0, 0.0, 7.0);
        // cube->drawLine(0.0, 0.0, 7.0, 0.0, 7.0, 7.0);
        // cube->drawLine(0.0, 7.0, 7.0, 0.0, 7.0, 0.0);
        // cube->drawLine(0.0, 7.0, 0.0, 0.0, 0.0, 0.0);
        // cube->drawLine(0.0, 7.0, 7.0, 7.0, 7.0, 7.0);
        // cube->drawLine(7.0, 7.0, 7.0, 7.0, 7.0, 0.0);
        // cube->drawLine(7.0, 7.0, 0.0, 0.0, 7.0, 0.0);
        // cube->drawLine(0.0, 0.0, 7.0, 7.0, 0.0, 7.0);
        // cube->drawLine(7.0, 0.0, 7.0, 7.0, 7.0, 7.0);
        // cube->drawLine(7.0, 7.0, 0.0, 7.0, 0.0, 0.0);
        // cube->drawLine(7.0, 0.0, 0.0, 7.0, 0.0, 7.0);
        // cube->drawLine(7.0, 0.0, 0.0, 0.0, 0.0, 0.0);

        // cube->drawLine(7.0, 7.0, 7.0, 0.0, 0.0, 0.0);
        // cube->drawLine(7.0, 0.0, 0.0, 0.0, 7.0, 7.0);
        // cube->drawLine(0.0, 7.0, 0.0, 7.0, 0.0, 7.0);
        // cube->drawLine(0.0, 0.0, 7.0, 7.0, 7.0, 0.0);
    }

    void reset() {}
};

#endif // CUBEWOP_CPP_
