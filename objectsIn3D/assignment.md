# Assignment 3: Objects in 3D

Create an OpenGL program this allows a scene to be viewed in 3D . The program
should allow the scene to be viewed from multiple eye positions directed by
user input.

The scene should consist of several 3D solid objects, and at least two
instances of the same object, but with each instance at a different position,
scale and orientation. The program must create at least one generic object, and
then create each instance by translation, rotation and scaling of the generic
object.

You must generate the objects yourself.  Canned GLU or GLUT objects may not be
used.  At least one of the objects must be something that is not provided in
the class examples, i.e. not a sphere or a cube.

The program should allow the scene to be viewed from different viewpoints using
the cursor keys or mouse.

What to draw is left to your imagination. You could, for example, create a
scene of a town consisting of a number of houses. The generic instance of house
can be transformed so that each house has a different length, width, height and
color.

The program should instruct OpenGL to remove those parts of the objects that
are obstructed by other objects in the scene.

Think ahead. You may want to build on this program for subsequent assignments.
In particular, be aware that GLUT objects do not support textures, which will
be a problem later on.

What to turn in:
1) Source code including makefile that builds hw3;
2) Readme with instructions;
3) Approximate time it took to complete the assignment.

The purpose of this assignment is to give you some experience with creating
generic 3D objects. You may want to stick to just the orthogonal projection for
this assignment as it is the simplest to use.
