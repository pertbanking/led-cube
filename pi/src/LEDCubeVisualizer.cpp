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
#include <cstdint>

#include <GL/glut.h>


using namespace std;


int cn;


void drawVoxel(float x, float y, float z, bool on) {
  glPushMatrix();

  if (on)
    glColor3f(1, 1, 1);
  else
    glColor3f(0.2f, 0.2f, 0.2f);

  glTranslatef(x, y, z);

  glutSolidCube(0.2f);

  glPopMatrix();
}


void drawGrid() {

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


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13, 0, -45);
    glRotatef(40, 1, 1, 0);

    drawGrid();
    drawQuads();
    theCube();
    glutSwapBuffers();
}


void init() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    //glClearColor(0.1, 0.1, 0.1, 1);
    // glClearColor(0.2, 0.2, 0.2, 1);

}

void keyboard(unsigned char key, int x, int y) {
    // this is here if we need it
}


LEDCubeVisualizer* LEDCubeVisualizer::instance = nullptr;

// ==================================
// constants, (con/de)structors, copiers, and instances
// ==================================

LEDCubeVisualizer::LEDCubeVisualizer(
    int message_rate, 
    shared_ptr<serial::Serial> usb, 
    string magic)
        : LEDCube(message_rate) {
}

LEDCubeVisualizer::~LEDCubeVisualizer() {
}

LEDCubeVisualizer* LEDCubeVisualizer::getInstance() {
    return LEDCubeVisualizer::instance;
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



int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Sample Grid Window");
  glutDisplayFunc(display);

  glutKeyboardFunc(keyboard);

  init();

  glutMainLoop();

  return 0;


}

#endif
