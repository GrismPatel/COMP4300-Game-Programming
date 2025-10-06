#!/bin/bash

g++ -std=c++17 assignment_1.cpp -I /usr/local/Cellar/sfml/3.0.1/include -o app -lsfml-graphics -lsfml-window -lsfml-system
./app
