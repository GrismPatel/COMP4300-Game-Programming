#!/bin/bash

g++ -std=c++17 $1 -I /usr/local/Cellar/sfml/3.0.1/include -o app -lsfml-graphics -lsfml-window -lsfml-system
./app
