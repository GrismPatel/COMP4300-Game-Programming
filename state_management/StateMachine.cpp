#include "StateMachine.hpp"


StateMachine::StateMachine(std::unique_ptr<State> initialState) {
    std::cout << "Initializing State" << std::endl;
    m_states.push(std::move(initialState));
};


void StateMachine::update() {
    m_states.top()->update();
};


void StateMachine::changeState() {

    /*
    Same as std::unique_ptr<State> nextState = m_states.top()->getNextState();
    */
    auto nextState = std::unique_ptr<State>(m_states.top()->getNextState());

    if (nextState != nullptr) {
        m_states.pop();
        m_states.push(std::move(nextState));
    };
};


void StateMachine::draw() {
    m_states.top()->render();
};


bool StateMachine::isPaused() {
    return m_states.top()->isCurrentStatePaused();
};


StateMachine::~StateMachine() {
    std::cout << "Shutting StateMachine Down." << std::endl;
};


/*
m_states is stack = [currentState]
if replacing then currentState is removed and nextState is pushed.
*/
