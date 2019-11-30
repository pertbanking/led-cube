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


const std::string SNAKESOLVER_NAME = "Snake solver";
const int SNAKESOLVER_FRAMERATE = 60;
const int SNAKESOLVER_DURATION = 250;

/**
 * Slowly builds a cube out of its constituent line segments
 */
class SnakeSolver : public Animation {
private:
    const float CUBE_EDGE = float(CUBE_SIZE) - 1.5f;
    const float MOVE_SPEED = 0.85f;
    std::vector<std::pair<float, float>> snake_points;
    std::pair<float, float> head_point;
    std::pair<float, float> food_point;

public:
    CubeConstructor()
    : Animation(
        SNAKESOLVER_NAME, 
        SNAKESOLVER_FRAMERATE, 
        SNAKESOLVER_DURATION)
    , snake_points({
            {}
        })
    , head_point()
    , food_point() {
        std::random_shuffle(segments.begin(), segments.end());
    }

    void calculateNext(LEDCube* cube) {
        cube->clear();

        for (unsigned int i = 0; i < edge_counter; ++i) {
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
            rest(10000000);
        }
    }

    void reset() {
        std::random_shuffle(segments.begin(), segments.end());
        edge_counter = 0;
        p1 = std::vector<float> {0.0, 0.0, 0.0};
        p2 = std::vector<float> {0.0, 0.0, 0.0};
    }

};

#endif