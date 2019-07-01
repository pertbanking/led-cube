/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef FIREWORKS_CPP_
#define FIREWORKS_CPP_

#include <random>
#include <tuple>
#include <vector>
#include <cmath>

#include "../Animation.h"


const std::string FIREWORKS_NAME = "Fireworks";

const float ROCKET_SPEED = 0.2;

class Fireworks : public Animation {
private:
    // the rockets (x,y,z)
    std::vector<std::tuple<float,float,float>> rockets;
    // the debris from the rockets (x, y, z, v_x, v_y, v_z, life)
    std::vector<std::tuple<float,float,float,float,float,float,int>> debris;
    std::default_random_engine gen;
    std::uniform_int_distribution<int> rocket_generator;
    std::uniform_real_distribution<float> rocket_placer;
    std::uniform_int_distribution<int> rocket_exploder;
    std::uniform_int_distribution<int> debris_amount;
    std::normal_distribution<float> debris_phi;
    std::uniform_real_distribution<float> debris_theta;
    std::uniform_real_distribution<float> debris_speed;
    std::uniform_int_distribution<int> debris_life;


    void explodeRocket(int index) {
        int amount = debris_amount(gen);
        for (int i = 0; i < amount; ++i) {
            float t = debris_theta(gen);
            float p = debris_phi(gen);
            float s = debris_speed(gen);

            debris.push_back(std::make_tuple(
                std::get<0>(rockets[index]), 
                std::get<1>(rockets[index]), 
                std::get<2>(rockets[index]),
                s*sin(p)*cos(t),
                s*sin(p)*sin(t),
                s*cos(p),
                debris_life(gen)
            ));
        }
        rockets.erase(rockets.begin() + index);
    }

public:
    Fireworks()
    : Animation(FIREWORKS_NAME)
    , rockets()
    , debris()
    , gen()
    , rocket_generator(1, 100)
    , rocket_placer(3.5, 5.5) 
    , rocket_exploder(1, 100)
    , debris_amount(10, 25)
    , debris_phi(M_PI_2, 0.4)
    , debris_theta(0.0, 2.0 * M_PI)
    , debris_speed(0.2, 0.8)
    , debris_life(30, 100) {
        this->recommendedFramerate = 50;
    }


    void calculateNext(LEDCube* cube) {
        
        // clear the cube
        cube->clear();

        if (rocket_generator(gen) > 95) {  // 4/100 chance of making a rocket
            // make a new rocket
            rockets.push_back(std::tuple<float,float,float> { 
                rocket_placer(gen),
                rocket_placer(gen),
                0.0
            });
        }

        // advance the rockets
        for (auto i = rockets.begin(); i != rockets.end(); ++i) {
            if (std::get<2>(*i) > 5.5) {
                this->explodeRocket(i - rockets.begin());
            } else if (std::get<2>(*i) > 3.0 && rocket_exploder(gen) > 80) {
                // explode with prob. 19/100 each frame from 3 to 5.5 
                this->explodeRocket(i - rockets.begin());
            }
            std::get<2>(*i) += ROCKET_SPEED;

            if (std::get<0>(*i) > -0.51 && std::get<0>(*i) < 7.49
                && std::get<1>(*i) > -0.51 && std::get<1>(*i) < 7.49
                && std::get<2>(*i) > -0.51 && std::get<2>(*i) < 7.49) {
                // draw the rocket if it's in the cube
                cube->voxelOn(std::get<0>(*i), std::get<1>(*i), std::get<2>(*i));
            }
        }

        // advance the debris
        for (auto i = debris.begin(); i != debris.end(); ++i) {
            std::get<0>(*i) += std::get<3>(*i);
            std::get<1>(*i) += std::get<4>(*i);
            std::get<2>(*i) += std::get<5>(*i);
            std::get<3>(*i) *= 0.8; // air resistance
            std::get<4>(*i) *= 0.8;
            std::get<5>(*i) += 0.02; // gravity
            std::get<5>(*i) *= 0.8;

            if (std::get<0>(*i) > -0.51 && std::get<0>(*i) < 7.49
                && std::get<1>(*i) > -0.51 && std::get<1>(*i) < 7.49
                && std::get<2>(*i) > -0.51 && std::get<2>(*i) < 7.49) {
                // draw the debris if it's in the cube
                cube->voxelOn(std::get<0>(*i), std::get<1>(*i), std::get<2>(*i));
            }
        }
    }

    void reset() {
        rockets = std::vector<std::tuple<float,float,float>>();
        debris = std::vector<std::tuple<float,float,float,float,float,float,int>>();
    }
};

#endif
