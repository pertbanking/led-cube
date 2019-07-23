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

#include <iostream>
#include <random>
#include <vector>

using namespace std;


const std::string SUSPENSION_NAME = "Suspension";
const int SUSPENSION_FRAMERATE = 20;
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
    bool retracting;

public:

    // TODO: Add functionality for different axes.

    Suspension()
    : Animation(SUSPENSION_NAME, SUSPENSION_FRAMERATE, SUSPENSION_DURATION)
    , places(CUBE_SIZE, vector<int>(CUBE_SIZE, 0))
    , planned(CUBE_SIZE, vector<int>(CUBE_SIZE))
    , gen(std::chrono::system_clock::now().time_since_epoch().count())
    , placer(0, CUBE_SIZE-1)
    , top(true)  // are we going toward the top?
    , step(0)    // step goes from 0 to 7 when unfolding and from 7 to 0 when
                 // retracting
    , retracting(false) {
        this->place();
    }

    void calculateNext(LEDCube* cube) {

        cube->clear();

        if (this->getFrame() == 0) {
            cube->drawZPlane(0);
            this->rest(7);
            return;
        }

        if (retracting) {
            --step;
            if (top) {
                for (int i = 0; i < CUBE_SIZE; ++i) {
                    for (int j = 0; j < CUBE_SIZE; ++j) {
                        if (places[i][j] < CUBE_SIZE-1) {
                            ++places[i][j];
                        }
                        cube->voxelOn(i, j, places[i][j]);
                    }
                }
            } else {
                for (int i = 0; i < CUBE_SIZE; ++i) {
                    for (int j = 0; j < CUBE_SIZE; ++j) {
                        if (places[i][j] > 0) {
                            --places[i][j];
                        }
                        cube->voxelOn(i, j, places[i][j]);
                    }
                }
            }
            if (step == 0) {
                rest(3);
                top = !top;
                retracting = false;
                this->place();
            }
        } else {
            ++step;
            if (top) {
                for (int i = 0; i < CUBE_SIZE; ++i) {
                    for (int j = 0; j < CUBE_SIZE; ++j) {
                        if (places[i][j] < planned[i][j]) {
                            ++places[i][j];
                        }
                        cube->voxelOn(i, j, places[i][j]);
                    }
                }
            } else {
                for (int i = 0; i < CUBE_SIZE; ++i) {
                    for (int j = 0; j < CUBE_SIZE; ++j) {
                        if (places[i][j] > planned[i][j]) {
                            --places[i][j];
                        }
                        cube->voxelOn(i, j, places[i][j]);
                    }
                }
            }
            if (step == CUBE_SIZE-1) {
                rest(7);
                retracting = true;
            }
        }
    }

    void reset() {
        this->frame = 0;
        
        this->step = 0;
        this->top = true;
        this->retracting = false;
        this->place();
    }

    void place() {
        for (int i = 0; i < CUBE_SIZE; ++i) {
            for (int j = 0; j < CUBE_SIZE; ++j) {
                this->places[i][j] = (this->top) ? 0 : CUBE_SIZE-1;
                this->planned[i][j] = this->placer(this->gen);
            }
        }
    }
};

#endif  // SUSPENSION_CPP_
