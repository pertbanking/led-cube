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

#include <iostream>
#include <thread>
#include <cstdio>
#include <cstdlib>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// this file computes the shaders for ouv visualizer
#include "opengl/shader.hpp"


using namespace std;


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

void LEDCubeVisualizer::drawVoxel(float x, float y, float z, bool on) {
    glPushMatrix();

    glTranslatef(x, y, z);
    if (on)
        glColor3f(1, 1, 1);
    else
        glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidCube(0.2f);

    glPopMatrix();
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

    drawGrid();

    for (int i = 0; i < CUBE_SIZE; ++i)
        for (int j = 0; j < CUBE_SIZE; ++j)
            for (int k = 0; k < CUBE_SIZE; ++k)
                drawVoxel(float(i), float(j), float(k), instance->data[i][j][k]);

    glutSwapBuffers();

    
}

void LEDCubeVisualizer::keyboard(unsigned char key, int x, int y) {
    // this is here if we need it?
}


void LEDCubeVisualizer::show() {
    // start the display thread
    render_thread = std::thread([this](void) {
        if(!glfwInit()) {
            throw new std::system_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window = glfwCreateWindow(1024, 768, "Cube Visualization", NULL, NULL);
        if(window == NULL) {
            glfwTerminate();
            throw new std::system_error("Failed to make a GLFW window");
        }
        glfwMakeContextCurrent(window);

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
        
        glutKeyboardFunc(keyboard);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(35, 1.0f, 0.1f, 1000);  // ?
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        std::cout << ":";

        glutMainLoop();
    });
}

#endif
