cmake_minimum_required(VERSION 2.6)

SET(PROJECTNAME hw2)

project(${PROJECTNAME})

add_executable(${PROJECTNAME} hw2.cpp)

# standard compile flags
add_definitions(-pedantic -O2 -Wall -std=c++11)

# require opengl and glut
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

set(_glut_libraries)
foreach(_lib ${GLUT_LIBRARIES})
  if(_lib)
    list(APPEND _glut_libraries ${_lib})
  endif()
endforeach()
set(GLUT_LIBRARIES ${_glut_libraries})

target_link_libraries(${PROJECTNAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
