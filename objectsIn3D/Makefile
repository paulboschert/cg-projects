CXX=g++
TARGETS=hw3
CXXFLAGS=-O2 -Wall -pedantic -std=c++11

# MinGW
ifeq "$(OS)" "Windows_NT"
	LIBS=-lglut32cu -lglu32 -lopengl32
	CLEAN=del $(TARGETS) *.o *.a
else
  CLEAN=rm -vf $(TARGETS) *.o *.a

	ifeq "$(shell uname)" "Darwin" # OSX
		LIBS=-framework GLUT -framework OpenGL
	else # Linux/Unix/Solaris
		LIBS=-lglut -lGLU -lGL -lm
	endif
endif

#  Compile and link
hw3:objectsIn3D.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

#  Clean
clean:
	$(CLEAN)
