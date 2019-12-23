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
const int SNAKESOLVER_DURATION = 500;

/**
 * Plays Snake with itself
 */
class SnakeSolver : public Animation {
private:
    enum SnakeDirection {
        POSITIVE_X = 0,
        POSITIVE_Y,
        POSITIVE_Z,
        NEGATIVE_X,
        NEGATIVE_Y,
        NEGATIVE_Z
    }

    const int MOVE_SPEED = 2;
    const int FOOD_BLINK_SPEED = 3;

    std::default_random_engine gen;
    std::uniform_int_distribution<int> food_placer;
    std::uniform_int_distribution<int> snake_director;

    std::vector<std::tuple<int,int,int>> snake_points;
    std::tuple<int,int,int> head_point;
    std::tuple<int,int,int> food_point;
    int snake_starting_direction;
    
    bool food_on;
    int food_on_counter;

public:
    SnakeSolver()
    : Animation(
        SNAKESOLVER_NAME, 
        SNAKESOLVER_FRAMERATE, 
        SNAKESOLVER_DURATION)
    , gen(std::chrono::system_clock::now().time_since_epoch().count())
    , food_placer(0,7)
    , snake_director(0,5);
    , snake_points(5)
    , head_point(food_placer(gen), food_placer(gen), food_placer(gen))
    , food_point(food_placer(gen), food_placer(gen), food_placer(gen))
    , snake_starting_direction(snake_director(gen))
    , food_on(true)
    , food_on_counter(0) {
        
    }

    void calculateNext(LEDCube* cube) {
        cube->clear();

        for (int i : snake_points) {
            cube->drawPoint(std::get<0>(i), std::get<1>(i), std::get<2>(i));
        }
        
        if (food_on) {
            cube->drawPoint(
                std::get<0>(food_point),
                std::get<1>(food_point),
                std::get<2>(food_point)
            );
        }

        food_on_counter = (food_on_counter + 1) % FOOD_BLINK_SPEED;

        if (food_point == head_point) {
            std::get<0>(food_point) = food_placer(gen);
            std::get<1>(food_point) = food_placer(gen);
            std::get<2>(food_point) = food_placer(gen);
            // add a bogus point to the end of the vector to be shifted out
            // later
            snake_points.emplace_back(0,0,0);
        }

        // make a list of directions the snake can go
        std::set<SnakeDirection> viable_directions;
        for (int i = 0; i <= static_cast<int>(SnakeDirection::NEGATIVE_Z); ++i)
        {
            viable_directions.insert(static_cast<SnakeDirection>(i));
        }
        // x-axis
        if (std::get<0>(head_point))

        // the snake will randomly change directions, but will only do so if the
        // direction the randomizer provides is still beneficial to finding the
        // food
        SnakeDirection dir_attempt = static_cast<SnakeDirection>



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