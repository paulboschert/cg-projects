// -------------------------------------------------------------------------------------------------
/*!
 * File:            projections.cpp
 *
 * Author:          Paul Boschert
 * Project:         CSCI-5229 - Computer Graphics
 * Component Name:  Homework 4 Projections
 *
 */
// -------------------------------------------------------------------------------------------------

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <array>
#include <iostream>
#include <random>
#include <cstdarg>
#include <chrono>
#include <string>

using namespace std;

// the vieing mode
unsigned int mode = 0;

// viewing angles, dimension, and aspect ratio
double theta = 90;
double phi = 0;
double dimension = 50;
double aspectRatio = 0;

// clock parameters
float secondsAngle = 0;
float minutesAngle = 0;
float hoursAngle = 0;

/**
 * Check for GL errors and display a message
 *
 * @return None
 */
void ErrCheck(const std::string where) {
  int err = glGetError();
  if(err != 0) {
    cerr << "ERROR: " << gluErrorString(err) << " [" << where << "]" << endl;
  }
}

/**
 * Convenience routine to output raster text
 * Use VARARGS to make this more flexible
 * Taken from an example
 */
void writeToScreen(const char* format , ...) {
  const unsigned int LEN = 8192;
  char    buf[LEN];
  char*   ch = buf;
  va_list args;
  // turn the parameters into a character string
  va_start(args, format);
  vsnprintf(buf, LEN, format, args);
  va_end(args);
  // display the characters one at a time at the current raster position
  while(*ch) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
  }

  ErrCheck(__func__);
}

/*
 * Taken from ex8.c
 *
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
void cube(double x, double y, double z, double dx, double dy, double dz, double th) {
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);
  //  Cube
  glBegin(GL_QUADS);
  //  Front
  glColor3f(1, 0, 0);
  glVertex3f(-1, -1, 1);
  glVertex3f(+1, -1, 1);
  glVertex3f(+1, +1, 1);
  glVertex3f(-1, +1, 1);
  //  Back
  glColor3f(0, 0, 1);
  glVertex3f(+1, -1, -1);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, +1, -1);
  glVertex3f(+1, +1, -1);
  //  Right
  glColor3f(1, 1, 0);
  glVertex3f(+1, -1, +1);
  glVertex3f(+1, -1, -1);
  glVertex3f(+1, +1, -1);
  glVertex3f(+1, +1, +1);
  //  Left
  glColor3f(0, 1, 0);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, -1, +1);
  glVertex3f(-1, +1, +1);
  glVertex3f(-1, +1, -1);
  //  Top
  glColor3f(0, 1, 1);
  glVertex3f(-1, +1, +1);
  glVertex3f(+1, +1, +1);
  glVertex3f(+1, +1, -1);
  glVertex3f(-1, +1, -1);
  //  Bottom
  glColor3f(1, 0, 1);
  glVertex3f(-1, -1, -1);
  glVertex3f(+1, -1, -1);
  glVertex3f(+1, -1, +1);
  glVertex3f(-1, -1, +1);
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

/**
 * Draw a sphere on the screen
 *
 * @param x X-coordinate of the sphere
 * @param y Y-coordinate of the sphere
 * @param z Z-coordinate of the sphere
 * @return None
 */
void drawSphere(const double x, const double y, const double z, const double r) {

  // construct a new quadratic for an eventual sphere
  GLUquadric* quad = gluNewQuadric();
  gluQuadricNormals(quad, GLU_SMOOTH);
  gluQuadricDrawStyle(quad, GLU_FILL);
  gluQuadricOrientation(quad, GLU_OUTSIDE);
  gluQuadricTexture(quad, GL_TRUE);

  glPushMatrix(); // save transformation

  // offset and scale
  glTranslated(x, y, z);
  glScaled(r, r, r);

  // actually draw the sphere
  gluSphere(quad, r, 32, 32);

  glPopMatrix(); // undo transformations

  ErrCheck(__func__);
}

/**
 * Draw x, y, and z axes on the scene
 *
 * @return None
 */
void drawAxes() {
  const double len = 40; // length of axes

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

  // label axes
  glRasterPos3d(len, 0.0, 0.0);
  writeToScreen("X");
  glRasterPos3d(0.0, len, 0.0);
  writeToScreen("Y");
  glRasterPos3d(0.0, 0.0, len);
  writeToScreen("Z");

  ErrCheck(__func__);
}

/**
 * Draw a cylinder at the given coordinates, radius and length
 */
void drawCylinder(const double x, const double y, const double z, const double r, const double l,
                  const float theta) {
  glPushMatrix();
  glTranslated(x, y, z);
  glRotated(theta, 1, 0, 0);
  glScaled(r, r, r);

  // how many sides should this cylinder have
  const unsigned int sides = 20;

  glBegin(GL_QUAD_STRIP);
  for(unsigned int i = 0; i <= sides; ++i) {
    //const float angle = i * 1.0 / sides * (2 * M_PI);
    float const angle = i * 2 * M_PI / static_cast<float>(sides);
    glNormal3d(cos(angle), 0, sin(angle));
    glVertex3d(r * cos(angle), l, r * sin(angle));
    glVertex3d(r * cos(angle), 0, r * sin(angle));
  }
  glEnd();
  glPopMatrix();
}

void drawClock(const double x, const double y, const double z, const double handLength) {
  // draw a clock at the origin
  glColor3f(1, 0, 0);
  drawCylinder(x, y, z, .5, handLength, secondsAngle);
  glColor3f(0, 1, 0);
  drawCylinder(x, y, z, .75, handLength, minutesAngle);
  glColor3f(0, 0, 1);
  drawCylinder(x, y, z, 1, handLength, hoursAngle);
}

/**
 * Display callback function, called when we need to display
 *
 * @return None
 */
void displayCallback() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and Z-buffer
  glLoadIdentity(); // reset transformations

  // set the rotation
  glRotatef(phi, 1, 0, 0);
  glRotatef(theta, 0, 1, 0);

  switch(mode) {
    case(0): // just draw one clock at the origin
      drawClock(0, 0, 0, 20);
      break;
    case(1): // draw three cubes
      cube(10, 10, 10, 5, 5, 5, -30);
      cube(20, 20, 20, 5, 5, 5, 0);
      cube(-10, -10, -10, 5, 5, .5, 30);
      break;
    case(2): // draw a sphere
      drawSphere(5, 5, 5, 2);
      break;
    case(3): // draw everything, 3 cubes, 2 clocks, and a sphere
      cube(10, 10, 10, 5, 5, 5, -30);
      cube(20, 20, 20, 5, 5, 5, 0);
      cube(-10, -10, -10, 5, 5, 5, 30);
      drawClock(0, 0, 0, 20);
      drawClock(-20, -20, -20, 10);
      drawSphere(5, 5, 5, 2);
      break;
    default: // default draw 1 clock
      drawClock(0, 0, 0, 20);
      break;
  }

  drawAxes(); // draw x, y, z axes

  // flush and swap buffer
  glFlush();
  glutSwapBuffers();

  ErrCheck(__func__);
}

/**
 * Update the projection based on the dimension and the aspect ratio
 * based on ex9.c from the examples
 *
 * @return None
 */
void project() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-dimension * aspectRatio, dimension * aspectRatio, -dimension, dimension, -dimension,
          dimension);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  ErrCheck(__func__);
}

/**
 * Reshape callback function, when the window size is changed
 *
 * @param width New width
 * @param height New height
 * @return None
 */
void reshapeCallback(const int width, const int height) {
  // calculate the width to height ratio
  if(height > 0) {
    aspectRatio = width / static_cast<double>(height);
  } else {
    aspectRatio = 1;
  }

  glViewport(0, 0, width, height); // set the viewport as the entire window
  project(); // update the projection

  ErrCheck(__func__);
}

/**
 * A special key callback function
 *
 * left/right : change the view angle about the y axis
 * up/down    : change the view angle about the x axis
 *
 * @param k Special key pressed
 * @param x X-coordinate of the mouse in the window
 * @param y Y-coordinate of the mouse in the window
 * @return None
 */
void specialCallback(const int k, const int x, const int y) {
  switch(k) {
    case(GLUT_KEY_UP):
      phi += 5;
      break;
    case(GLUT_KEY_DOWN):
      phi -= 5;
      break;
    case(GLUT_KEY_RIGHT):
      theta += 5;
      break;
    case(GLUT_KEY_LEFT):
      theta -= 5;
      break;
  }

  // change the angles to be in the range 0-359
  phi = static_cast<int>(phi) % 360;
  theta = static_cast<int>(theta) % 360;

  project(); // update the projection

  glutPostRedisplay();

  ErrCheck(__func__);
}

/**
 * A keyboard callback function
 *
 * +/-        : zoom in/out
 * left/right : change the view angle about the y axis
 * up/down    : change the view angle about the x axis
 * q/esc      : close and return 0
 * m/M        : change the mode of what is displayed
 * r/R        : Reset the viewing angle to the starting angles
 *
 * @param c Character pressed
 * @param x X-coordinate of the mouse in the window
 * @param y Y-coordinate of the mouse in the window
 * @return None
 */
void keyCallback(const unsigned char c, const int x, const int y) {
  if(c == 'q' || c == 27) { // q or esc, exit
    exit(0);
  } else if(c == '+' && c < 179) { // zoom in
    dimension -= 1;
  } else if(c == '-' && c > 1) { // zoom out
    dimension += 1;
  } else if(c == 'm') { // change modes forward
    mode += 1;
  } else if(c == 'M') { // change modes back
    mode -= 1;
  } else if(c == 'r' || c == 'R') { // reset zoom and rotation
    theta = 90;
    phi = 0;
  }

  mode %= 4;

  project(); // update the projection

  glutPostRedisplay();

  ErrCheck(__func__);
}

/**
 * A timer callback function to update the position of the hands of the clock
 *
 * @param value The value for this timer, not used
 * @return None
 */
void timerCallback(const int value) {

  /*
   * Get the current time, then update the angles for seconds, minutes, hours
   */
  chrono::system_clock::time_point currentTime = chrono::system_clock::now();
  time_t tt = chrono::system_clock::to_time_t(currentTime);
  tm local_tm = *localtime(&tt);
  float hours = local_tm.tm_hour;
  float minutes = local_tm.tm_min;
  float seconds = local_tm.tm_sec;
  hoursAngle = 360 * hours / 12.0;
  minutesAngle = 360 * minutes / 60.0;
  secondsAngle = 360 * seconds / 60.0;

  glutPostRedisplay();
  glutTimerFunc(1, timerCallback, value);

  ErrCheck(__func__);
}

int main(int argc, char* argv[], char* envp[]) {
  glutInit(&argc, argv); // initialize GLUT

  // request double buffered true color window with z-buffer
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(600, 600);
  // create the window
  glutCreateWindow("Paul Boschert - Objects In 3D");

  // register display, reshape callback functions
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutSpecialFunc(specialCallback);
  glutKeyboardFunc(keyCallback);
  glutTimerFunc(1, timerCallback, 0);

  glEnable(GL_DEPTH_TEST); // enable z-buffer depth test

  glutMainLoop(); // pass control to GLUT

  ErrCheck(__func__);

  return 0;
}

