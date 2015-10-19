#!/bin/bash

set -o posix

tar -cjvf hw6.tar.bz2 answers.txt Makefile textures.cpp README.md

exit "$?"
