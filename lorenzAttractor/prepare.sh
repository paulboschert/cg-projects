#!/bin/bash

set -o posix

tar -cjvf hw2.tar.bz2 answers.txt Makefile hw2.cpp README.md

exit "$?"
