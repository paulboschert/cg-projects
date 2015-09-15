#!/bin/bash

set -o posix

tar -cjvf hw2.tar.bz2 answers.txt CMakeLists.txt hw2.cpp README

exit "$?"
