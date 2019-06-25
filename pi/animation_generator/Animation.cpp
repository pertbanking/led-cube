/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef ANIMATION_CPP_
#define ANIMATION_CPP_

#include "Animation.h"



void Animation::next(LEDCube* cube) {
    if (this->rest <= 0) {
        cube->lock();
        this->_next(cube);
        cube->unlock();
    } else {
        --this->rest;
    }
    ++this->frame;
}



#endif
