#!/bin/bash

g++ -std=c++17 main.cpp Game.cpp MenuState.cpp State.cpp StateMachine.cpp AnimationState.cpp PhysicsState.cpp -I /usr/local/Cellar/sfml/3.0.1/include -o app -lsfml-graphics -lsfml-window -lsfml-system
./app
