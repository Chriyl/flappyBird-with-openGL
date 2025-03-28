#bin/sh

exeName="opengl_test"

g++ main.cc -o $exeName -lglfw -lGL && ./$exeName
