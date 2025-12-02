#include <iostream>
#include "State.hpp"
#include <stack>
#include <memory>


class StateMachine {
    private:
        std::stack<std::unique_ptr<State>> m_states;
    public:
        StateMachine(std::unique_ptr<State> initialState);

        bool isPaused();
        void update();
        void changeState();
        void draw();

        ~StateMachine();
};
