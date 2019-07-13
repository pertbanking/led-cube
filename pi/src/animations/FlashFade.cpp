/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef FLASH_FADE_CPP_
#define FLASH_FADE_CPP_

#include "../Animation.h"

#include <algorithm>

using namespace std;


const std::string FLASH_FADE_NAME = "Flash->Fade";
const int FLASHF_FRAMERATE = 100;
const int FLASHF_DURATION = 1100;

/**
 * Flashes all lights in the cube for a short time and then turns all of them
 * off gradually.
 */
class FlashFade : public Animation {
private:
    bool on;
    float flashrest;
    int lastFrame;
    int point;
    vector<int> pointOrder;

public:
    FlashFade()
    : Animation(FLASH_FADE_NAME, FLASHF_FRAMERATE, FLASHF_DURATION)
    , on(false)
    , flashrest(40.0)
    , lastFrame(0)
    , point(0)
    , pointOrder(CUBE_SIZE*CUBE_SIZE*CUBE_SIZE) {
        for (int i = 0; i < pointOrder.size(); ++i) {
            pointOrder[i] = i;
        }

        std::random_shuffle(pointOrder.begin(), pointOrder.end());
    }

    void calculateNext(LEDCube* cube) {
        if (flashrest > 1) {
            if (this->getFrame() - lastFrame > flashrest - 1.0) {
                flashrest *= 0.95;
                lastFrame = this->getFrame();
                on = !on;

                for (int i = 0; i < CUBE_SIZE; ++i) {
                    for (int j = 0; j < CUBE_SIZE; ++j) {
                        for (int k = 0; k < CUBE_SIZE; ++k) {
                            cube->setVoxel(i, j, k, on);
                        }
                    }
                }
            }
        } else if (flashrest <= 1 and flashrest != 0.0) {
            // turn the cube on and rest for a little bit before we fade out
            this->rest(30);
            flashrest = 0.0;

            for (int i = 0; i < CUBE_SIZE; ++i) {
                for (int j = 0; j < CUBE_SIZE; ++j) {
                    for (int k = 0; k < CUBE_SIZE; ++k) {
                        cube->voxelOn(i, j, k);
                    }
                }
            }
        } else {
            if (pointOrder.size() > 0) {
                // erase two random points
                cube->voxelOff(
                    pointOrder.back() % CUBE_SIZE,
                    (pointOrder.back() / CUBE_SIZE) % CUBE_SIZE,
                    (pointOrder.back() / (CUBE_SIZE*CUBE_SIZE)) % CUBE_SIZE
                );
                pointOrder.erase(pointOrder.end() - 1);
            }
            if (pointOrder.size() > 0) {
                cube->voxelOff(
                    pointOrder.back() % CUBE_SIZE,
                    (pointOrder.back() / CUBE_SIZE) % CUBE_SIZE,
                    (pointOrder.back() / (CUBE_SIZE*CUBE_SIZE)) % CUBE_SIZE
                );
                pointOrder.erase(pointOrder.end() - 1);
            }
            // if we're done erasing the points, just do nothing.
        }
    }

    void reset() {
        this->frame = 0;
        on = false;
        flashrest = 40;
        lastFrame = 0;
        point = 0;

        pointOrder = vector<int>(CUBE_SIZE*CUBE_SIZE*CUBE_SIZE);

        for (int i = 0; i < pointOrder.size(); ++i) {
            pointOrder[i] = i;
        }

        std::random_shuffle(pointOrder.begin(), pointOrder.end());
    }
};

#endif // FLASH_FADE_CPP_
