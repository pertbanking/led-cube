/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef SUSPENSION_CPP_
#define SUSPENSION_CPP_

#include "../Animation.h"

#include <random>
#include <vector>

using namespace std;


const std::string SUSPENSION_NAME = "Suspension";
const int SUSPENSION_FRAMERATE = 30;
const int SUSPENSION_DURATION = 500;

/**
 * Suspends all points across 
 */
class Suspension : public Animation {
private:
    vector< vector<int> > places;
    vector< vector<int> > planned;
    std::default_random_engine gen;
    std::uniform_int_distribution<int> placer;
    bool top;
    int step;

public:
    Suspension()
    : Animation(SUSPENSION_NAME, SUSPENSION_FRAMERATE, SUSPENSION_DURATION)
    , places(CUBE_SIZE, vector<int>(CUBE_SIZE, 0))
    , planned(CUBE_SIZE, vector<int>(CUBE_SIZE))
    , gen(std::chrono::system_clock::now().time_since_epoch().count())
    , placer(0, CUBE_SIZE)
    , top(false)
    , step(0) {
        this->reset();
    }

    void calculateNext(LEDCube* cube) {
        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {
                // if ()
            }
        }
    }

    void reset() {
        this->frame = 0;
        this->reset(this->top);
    }

    void reset(bool top_) {
        step = (top_) ? CUBE_SIZE-1 : 0;
        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {
                places[i][j] = (top_) ? CUBE_SIZE-1 : 0;
                planned[i][j] = placer(gen);
            }
        }
    }
};