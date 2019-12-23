/**
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * @author Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#ifndef LEDCUBEVISUALIZER_CPP_
#define LEDCUBEVISUALIZER_CPP_


#include "LEDCubeVisualizer.h"

#include <exception>
#include <iostream>
#include <system_error>
#include <thread>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// this file computes the shaders for our visualizer
// #include "opengl/shader.hpp"

// forward decl for static member
GLfloat LEDCubeVisualizer::cube_vertices[] = {
    -0.1, -0.1, -0.1,   -0.1, -0.1,  0.1,   -0.1,  0.1,  0.1,   -0.1,  0.1, -0.1,
     0.1, -0.1, -0.1,    0.1, -0.1,  0.1,    0.1,  0.1,  0.1,    0.1,  0.1, -0.1,
    -0.1, -0.1, -0.1,   -0.1, -0.1,  0.1,    0.1, -0.1,  0.1,    0.1, -0.1, -0.1,
    -0.1,  0.1, -0.1,   -0.1,  0.1,  0.1,    0.1,  0.1,  0.1,    0.1,  0.1, -0.1,
    -0.1, -0.1, -0.1,   -0.1,  0.1, -0.1,    0.1,  0.1, -0.1,    0.1, -0.1, -0.1,
    -0.1, -0.1,  0.1,   -0.1,  0.1,  0.1,    0.1,  0.1,  0.1,    0.1, -0.1,  0.1
};

GLfloat LEDCubeVisualizer::cube_colors[] = {
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0
};

const int LEDCubeVisualizer::COLOR_LENGTH = 72;

LEDCubeVisualizer* LEDCubeVisualizer::instance = nullptr;

// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

LEDCubeVisualizer::LEDCubeVisualizer(int message_rate)
    : LEDCube(message_rate) {
    
}

LEDCubeVisualizer::~LEDCubeVisualizer() {
}

LEDCubeVisualizer* LEDCubeVisualizer::getInstance(int framerate) {
    if (LEDCubeVisualizer::instance == nullptr)
        LEDCubeVisualizer::instance = new LEDCubeVisualizer(framerate);
    return LEDCubeVisualizer::instance;
}

void LEDCubeVisualizer::destroyInstance() {
    if (LEDCubeVisualizer::instance != nullptr) {
        delete LEDCubeVisualizer::instance;
        LEDCubeVisualizer::instance = nullptr;
    }
}


// ==================================
// display functions
// ==================================

// helper function for drawGrid() below
void LEDCubeVisualizer::drawVoxel(int x, int y, int z, bool on) {
    
}

void LEDCubeVisualizer::drawGrid() {

    for (int i = 0; i < 40; i++) {
        glPushMatrix();

        if (i < 20) { 
            glTranslatef(0, 0, i); 
        }

        if (i >= 20) { 
            glTranslatef(i - 20, 0, 0);
            glRotatef(-90, 0, 1, 0); 
        }

        glBegin(GL_LINES);
        glColor3f(1, 1, 1); 
        glLineWidth(1);
        glVertex3f(0, -0.1, 0); 
        glVertex3f(19, -0.1, 0);
        glEnd();
        glPopMatrix();
    }
}

void LEDCubeVisualizer::display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-13, 0, -45);
    glRotatef(40, 1, 1, 0);

    // drawGrid();

    for (int i = 0; i < CUBE_SIZE; ++i) {
        for (int j = 0; j < CUBE_SIZE; ++j) {
            for (int k = 0; k < CUBE_SIZE; ++k) {
                for (int i = 0; i < LEDCubeVisualizer::COLOR_LENGTH; ++i) {
                    LEDCubeVisualizer::cube_colors[i]
                        = (instance->data[i][j][k])? 0.8 : 0.2;

                    glPushMatrix();

                    glTranslatef(
                        static_cast<float>(i),
                        static_cast<float>(j),
                        static_cast<float>(k)
                    );

                    // if (instance->data[i][j][k]) {
                    //     glColor3f(1.0f, 1.0f, 1.0f);
                    // } else {
                    //     glColor3f(0.2f, 0.2f, 0.2f);
                    // }

                    glEnableClientState(GL_VERTEX_ARRAY);
                    glEnableClientState(GL_COLOR_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, LEDCubeVisualizer::cube_vertices);
                    glColorPointer(3, GL_FLOAT, 0, LEDCubeVisualizer::cube_colors);

                    glDrawArrays(GL_QUADS, 0, 24);

                    glDisableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_VERTEX_ARRAY);

                    glPopMatrix();
                }
            }
        }
    }


    glfwSwapBuffers(LEDCubeVisualizer::getInstance()->window);
}

void LEDCubeVisualizer::keyboard(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {
    // this is here if we need it?
}


void LEDCubeVisualizer::show() {
    // start the display thread
    render_thread = std::thread([this](void) {
        if(!glfwInit()) {
            throw std::system_error(
                ECANCELED,
                std::generic_category(),
                "Failed to initialize GLFW"
            );
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // make MacOS happy
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window = glfwCreateWindow(1024, 768, "Cube Visualization", NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
            throw std::system_error(
                ECANCELED,
                std::generic_category(),
                "Failed to make a GLFW window"
            );
        }
        glfwMakeContextCurrent(window);

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
        
        glfwSetKeyCallback(window, &LEDCubeVisualizer::keyboard);
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(35, 1.0f, 0.1f, 1000);  // ?
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        std::cout << ":";

        while (!glfwWindowShouldClose(window)) {
            instance->display();
        }

        glfwTerminate();

        std::exit(0);
    });
}

#endif
