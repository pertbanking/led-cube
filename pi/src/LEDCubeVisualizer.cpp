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
#include <cstdint>

#include <GL/glut.h>


using namespace std;


LEDCubeVisualizer* LEDCubeVisualizer::instance = nullptr;

// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

LEDCubeVisualizer::LEDCubeVisualizer(int message_rate)
    : LEDCube(message_rate) {
    int phoneynum = 1;
    // std::string phoney = "Jandy in the house";
    // const char* blah[] = { phoney.c_str() };
    glutInit(&phoneynum, NULL);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LED Cube Visualizer");
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);  // ?
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
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
        glutMainLoop();
    });
}

#endif
