// -------------------------------------------------------------------------------------------------
/*!
 * File:            lorenzAttractor.cpp
 *
 * Project:         CSCI-5229 - Computer Graphics
 * Component Name:  Homework 2 Lorenz Attractor
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
#include <stdarg.h>

using namespace std;

// viewing angles, dimension and aspect ratio
double theta = 0;
double phi = 0;
double dimension = 50;
double aspectRatio = 0;

// base the color of the lorenz attractor line segments on this color factor
double randomColorFactor = 1;

// lorenz equation parameters
array<double, 4> rhos   = {14, 13, 15, 28}; // interesting rhos found on wikipedia
size_t rhoIndex = 3;
const double sigma = 10;
const double beta = 8 / 3.0;
size_t lorenzPointIndex = 100;
const size_t lorenzSize = 50000;

// keep arrays around for drawing lorenz attractor colors
const float red[]   = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
const float green[] = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
const float blue[]  = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0};

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
}

/**
 * Draw a sphere on the screen
 *
 * @param x X-coordinate of the sphere
 * @param y Y-coordinate of the sphere
 * @param z Z-coordinate of the sphere
 * @return None
 */
void drawSphere(const double x, const double y, const double z) {
  const double r = .75; // radius of the sphere

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
}

/**
 * Compute the lorenz attractor data and draw it
 *
 * The basis for this function is from http://www.prinmath.com/csci5229/misc/lorenz.c
 *
 * @return None
 */
void drawLorenzAttractor() {
  // coordinates
  double x = 1;
  double y = 1;
  double z = 1;
  // time step
  double dt = 0.001;

  /* integrate some number of steps at a given dt
   * explicit Euler integration
   */
  // this method of setting the color was based on one of the examples
  const unsigned int colorScaleFactor = 10 * randomColorFactor / dt;
  glBegin(GL_LINE_STRIP);
  for(unsigned int i = 0; i < lorenzSize; ++i) {
    const double dx = sigma * (y - x);
    const double dy = x * (rhos[rhoIndex] - z) - y;
    const double dz = x * y - beta * z;
    x += dt * dx;
    y += dt * dy;
    z += dt * dz;

    // draw the sphere if we've hit the index we want to draw it at
    if(i == lorenzPointIndex) {
      glEnd(); // finish the current line strip
      glColor3f(1, 1, 1); // set the color to white
      drawSphere(x, y, z);
      glBegin(GL_LINE_STRIP); // restart the line strip
    }

    // this method of setting the color was based on one of the examples
    glColor3f(red[i / colorScaleFactor], green[i / colorScaleFactor], blue[i / colorScaleFactor]);

    glVertex3d(x, y, z);
  }
  glEnd();
}

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

  drawLorenzAttractor(); // draw the lorenz attractor

  drawAxes(); // draw x, y, z axes

  // flush and swap buffer
  glFlush();
  glutSwapBuffers();
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

  project(); // update the projection

  glutPostRedisplay();
}

/**
 * A keyboard callback function
 *
 * +/-        : zoom in/out
 * c/C        : change the color to a random color scaling factor based on dt
 * r/R        : change the rho parameter to the lorenz equation
 * q/esc      : close and return 0
 *
 *
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
  } else if(c == 'c' || c == 'C') { // randomly change the color scaling factor
    randomColorFactor = 1 + rand() / static_cast<double>(RAND_MAX);
  } else if(c ==
            'r') { // change rho to something interesting based on the wikipedia page (go forward one)
    rhoIndex = (rhoIndex + 1) % rhos.size();
  } else if(c ==
            'R') { // change rho to something interesting based on the wikipedia page (go back one)
    if(rhoIndex == 0) {
      rhoIndex = rhos.size() - 1;
    } else {
      rhoIndex = (rhoIndex - 1) % rhos.size();
    }
  }

  project(); // update the projection

  glutPostRedisplay();
}

/**
 * A timer callback function to update the position of the sphere
 *
 * @param value The value for this timer, not used
 * @return None
 */
void timerCallback(const int value) {
  lorenzPointIndex = (lorenzPointIndex + 10) % lorenzSize;
  glutPostRedisplay();
  glutTimerFunc(1, timerCallback, value);
}

int main(int argc, char* argv[], char* envp[]) {
  glutInit(&argc, argv); // initialize GLUT

  // request double buffered true color window with z-buffer
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(600, 600);
  // create the window
  glutCreateWindow("Lorenz Attractor");

  // register display, reshape callback functions
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutSpecialFunc(specialCallback);
  glutKeyboardFunc(keyCallback);
  glutTimerFunc(1, timerCallback, 0);

  glEnable(GL_DEPTH_TEST); // enable z-BUFFer depth test

  glutMainLoop(); // pass control to GLUT

  return 0;
}

