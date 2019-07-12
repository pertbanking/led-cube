/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef CUBEWOP_I_CPP_
#define CUBEWOP_I_CPP_

#include "../Animation.h"


const std::string CUBEWOP_I_NAME = "CubeWop Corners";

class CubeWop_I : public Animation {
private:
    int corner;
    bool top;
    int pos;
    bool shrink;

public:

    CubeWop_I()
    : Animation(CUBEWOP_NAME, 50, 700)
    , corner(0)
    , top(false)
    , pos(7)
    , shrink(true) {}

    void calculateNext(LEDCube* cube) {
        cube->clear();

        // TODO: adjust
        this->rest(0);

        if (pos == 0 and shrink) {
            shrink = false;
        } else if (pos == 7 and !shrink) {
            shrink = true;
            // TODO: adjust
            this->rest(8);
            if (corner == 4) {
                corner = 1;
                top = !top;
            } else {
                ++corner;
            }
        } else {
            pos += 1 - 2*(shrink == true);
        }

        float fpos = float(pos);

        // x
        // 
        // 1  2
        // 
        // 3  4   y
        if (top) {
            switch (corner) {
                case 1:
                cube->drawBox(7.0, 0.0, 7.0, 7.0-fpos, fpos, 7.0-fpos);
                break;

                case 2:
                cube->drawBox(7.0, 7.0, 7.0, 7.0-fpos, 7.0-fpos, 7.0-fpos);
                break;
                
                case 3:
                cube->drawBox(0.0, 0.0, 7.0, fpos, fpos, 7.0-fpos);
                break;
                
                case 4:
                cube->drawBox(0.0, 7.0, 7.0, fpos, 7.0-fpos, 7.0-fpos);
            }
        } else {
            switch (corner) {
                
                case 1:
                cube->drawBox(7.0, 0.0, 0.0, 7.0-fpos, fpos, fpos);
                break;
                
                case 2:
                cube->drawBox(7.0, 7.0, 0.0, 7.0-fpos, 7.0-fpos, fpos);
                break;
                
                case 3:
                cube->drawBox(0.0, 0.0, 0.0, fpos, fpos, fpos);
                break;
                
                case 4:
                cube->drawBox(0.0, 7.0, 0.0, fpos, 7.0-fpos, fpos);
            }
        }

    }

    void reset() {
        corner = 0;
        top = false;
        pos = 7;
        shrink = true;
    }
};

#endif // CUBEWOP_CPP_
