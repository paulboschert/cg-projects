#!/bin/bash

set -o posix

tar -cjvf hw4.tar.bz2 answers.txt Makefile projections.cpp README.md

exit "$?"
