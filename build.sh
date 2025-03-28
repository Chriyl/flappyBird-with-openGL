#bin/sh

exeName="superGioco"

g++ main.cc -o $exeName -lglfw -lGL && ./$exeName
