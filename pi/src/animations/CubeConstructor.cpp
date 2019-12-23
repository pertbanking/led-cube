/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef CUBECONSTRUCTOR_CPP_
#define CUBECONSTRUCTOR_CPP_

#include "../Animation.h"

#include <algorithm>
#include <tuple>
#include <vector>
#include <cmath>


const std::string CUBECONSTRUCTOR_NAME = "Cube constructor";
const int CUBECONSTRUCTOR_FRAMERATE = 60;
const int CUBECONSTRUCTOR_DURATION = 250;

/**
 * Slowly builds a cube out of its constituent line segments
 */
class CubeConstructor : public Animation {
private:
    const float CUBE_EDGE = float(CUBE_SIZE) - 1.5f;
    const float MOVE_SPEED = 0.85f;
    std::vector<std::tuple<float,float,float,float,float,float>> segments;
    std::vector<float> p1;
    std::vector<float> p2;
    int edge_counter;


public:
    CubeConstructor()
    : Animation(
        CUBECONSTRUCTOR_NAME, 
        CUBECONSTRUCTOR_FRAMERATE, 
        CUBECONSTRUCTOR_DURATION)
    , segments(
        std::vector<std::tuple<float,float,float,float,float,float>> {
            std::make_tuple(0.0f, 0.0f, 0.0f, CUBE_EDGE, 0.0f, 0.0f),
            std::make_tuple(0.0f, 0.0f, 0.0f, 0.0f, CUBE_EDGE, 0.0f),
            std::make_tuple(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, CUBE_EDGE),
            std::make_tuple(CUBE_EDGE, 0.0f, 0.0f, CUBE_EDGE, CUBE_EDGE, 0.0f),
            std::make_tuple(CUBE_EDGE, 0.0f, 0.0f, CUBE_EDGE, 0.0f, CUBE_EDGE),
            std::make_tuple(0.0f, CUBE_EDGE, 0.0f, CUBE_EDGE, CUBE_EDGE, 0.0f),
            std::make_tuple(0.0f, CUBE_EDGE, 0.0f, 0.0f, CUBE_EDGE, CUBE_EDGE),
            std::make_tuple(0.0f, 0.0f, CUBE_EDGE, CUBE_EDGE, 0.0f, CUBE_EDGE),
            std::make_tuple(0.0f, 0.0f, CUBE_EDGE, 0.0f, CUBE_EDGE, CUBE_EDGE),
            std::make_tuple(CUBE_EDGE, CUBE_EDGE, 0.0f, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE),
            std::make_tuple(CUBE_EDGE, 0.0f, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE),
            std::make_tuple(0.0f, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE, CUBE_EDGE),
        })
    , p1(std::vector<float> {0.0, 0.0, 0.0})
    , p2(std::vector<float> {0.0, 0.0, 0.0})
    , edge_counter(0) {
        std::random_shuffle(segments.begin(), segments.end());
    }

    void calculateNext(LEDCube* cube) {
        cube->clear();

        for (int i = 0; i < edge_counter; ++i) {
            cube->drawLine(
                std::get<0>(segments[i]), 
                std::get<1>(segments[i]),
                std::get<2>(segments[i]),
                std::get<3>(segments[i]), 
                std::get<4>(segments[i]),
                std::get<5>(segments[i])
            );
        }

        cube->drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);

        bool next = true;
        // move each point toward its destination
        if (p1[0] < std::get<0>(segments[edge_counter])) {
            next = false;
            p1[0] += MOVE_SPEED;
        }
        if (p1[1] < std::get<1>(segments[edge_counter])) {
            next = false;
            p1[1] += MOVE_SPEED;
        }
        if (p1[2] < std::get<2>(segments[edge_counter])) {
            next = false;
            p1[2] += MOVE_SPEED;
        }
        if (p2[0] < std::get<3>(segments[edge_counter])) {
            next = false;
            p2[0] += MOVE_SPEED;
        }
        if (p2[1] < std::get<4>(segments[edge_counter])) {
            next = false;
            p2[1] += MOVE_SPEED;
        }
        if (p2[2] < std::get<5>(segments[edge_counter])) {
            next = false;
            p2[2] += MOVE_SPEED;
        }

        if (next and edge_counter < segments.size() - 1) {
            ++edge_counter;
            p1 = std::vector<float> {0.0, 0.0, 0.0};
            p2 = std::vector<float> {0.0, 0.0, 0.0};
            rest(5);
        } else if (next) {
            // halt
            rest(100000);
        }
    }

    void reset() {
        this->frame = 0;
        rest(0);  // stop resting

        std::random_shuffle(segments.begin(), segments.end());
        edge_counter = 0;
        p1 = std::vector<float> {0.0, 0.0, 0.0};
        p2 = std::vector<float> {0.0, 0.0, 0.0};
    }

};

#endif