#!/bin/bash

set -o posix

tar -cjvf hw3.tar.bz2 answers.txt Makefile objectsIn3D.cpp README.md

exit "$?"
