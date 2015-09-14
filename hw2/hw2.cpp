// -------------------------------------------------------------------------------------------------
/*!
 * File:            hw2.cpp
 *
 * Project:         CSCI-5229 - Computer Graphics: Homework 2 Lorenz Attractor
 * Component Name:  Homework 2 driver main function
 *
 * License: No License
 * Licensed Material - Property of Paul Boschert
 *
 * (c) Copyright 2015 Paul Boschert.  All rights reserved.
 *
 */
// -------------------------------------------------------------------------------------------------

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include <iostream>
#include <stdarg.h>

using namespace std;

double theta = 0;
double phi = 0;


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...) {
  char    buf[LEN];
  char*   ch = buf;
  va_list args;
  //  Turn the parameters into a character string
  va_start(args, format);
  vsnprintf(buf, LEN, format, args);
  va_end(args);
  //  Display the characters one at a time at the current raster position
  while(*ch) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
  }
}

void drawLorenzAttractor() {
  /*
   * The basis for this function is from http://www.prinmath.com/csci5229/misc/lorenz.c
   */

  double s = 10;
  double b = 2.6666666;
  double r = 28;

  // coordinates
  double x = 1;
  double y = 1;
  double z = 1;
  // time step
  double dt = 0.001;

  /* integrate some number of steps at a given dt
   * explicit Euler integration
   */
  const unsigned int steps = 50000;

  float red[]   = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
  float green[] = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
  float blue[]  = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0};

  //const unsigned int scaleFactor = 1 / dt;
  const unsigned int scaleFactor = 1 / dt;
  glBegin(GL_LINE_STRIP);
  for(unsigned int i = 0; i < steps; ++i) {
    const double dx = s * (y - x);
    const double dy = x * (r - z) - y;
    const double dz = x * y - b * z;
    x += dt * dx;
    y += dt * dy;
    z += dt * dz;

    glColor3f(red[i / scaleFactor], green[i / scaleFactor], blue[i / scaleFactor]);

    glVertex3d(x, y, z);
  }
  glEnd();
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and Z-buffer
  glLoadIdentity(); // reset transformations

  // set the rotation
  glRotatef(phi, 1, 0, 0);
  glRotatef(theta, 0, 1, 0);

  drawLorenzAttractor();

  const double len = 40; //  Length of axes

  // draw axes
  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(len, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, len, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, len);
  glEnd();

  //  Label axes
  glRasterPos3d(len, 0.0, 0.0);
  Print("X");
  glRasterPos3d(0.0, len, 0.0);
  Print("Y");
  glRasterPos3d(0.0, 0.0, len);
  Print("Z");

  // flush and swap buffer
  glFlush();
  glutSwapBuffers();
}


void reshape(const int width, const int height) {
#if 0
  // calculate the width to height ratio
  double w2h = 0;
  if(height > 0) { w2h = width / static_cast<double>(height); }
  else { w2h = 1; }

  // set the viewport as the entire window
  glViewport(0, 0, width, height);

  // select projection matrix
  glMatrixMode(GL_PROJECTION);

  // set projection to identiy
  glLoadIdentity();

  // orthogonal projection: unit cube adjusted for aspect ratio
  glOrtho(-w2h, w2h, -1, 1, -1, 1);

  // select model view matrix
  glMatrixMode(GL_MODELVIEW);

  // set model view to identity
  glLoadIdentity();
#endif
  double dim = 50;
  double asp = 1;
  //  Window aspect ration
  asp = (width && height) ? width / (float)height : 1;
  //  Viewport is whole screen
  glViewport(0, 0, width, height);
  //  Set projection

  //  Orthogonal projection to dim
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(asp > 1) {
    glOrtho(-dim * asp, +dim * asp, -dim, +dim, -3 * dim, +3 * dim);
  } else {
    glOrtho(-dim, +dim, -dim / asp, +dim / asp, -3 * dim, +3 * dim);
  }

  //  Back to model view
  glMatrixMode(GL_MODELVIEW);

}

void special(const int k, const int x, const int y) {
  switch(k) {
    case GLUT_KEY_UP:
      phi += 5;
      break;
    case GLUT_KEY_DOWN:
      phi -= 5;
      break;
    case GLUT_KEY_RIGHT:
      theta += 5;
      break;
    case GLUT_KEY_LEFT:
      theta -= 5;
      break;
    default:
      return;
  }

  glutPostRedisplay();
}


int main(int argc, char* argv[], char* envp[]) {
  glutInit(&argc, argv); // initialize GLUT

  // request double buffered true color window with z-buffer
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(600, 600);
  // create the window
  glutCreateWindow("Lorenz Attractor");

  // register display, reshape callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);

  glEnable(GL_DEPTH_TEST); // enable z-BUFFer depth test

  glutMainLoop(); // pass control to GLUT

  return 0;
}


