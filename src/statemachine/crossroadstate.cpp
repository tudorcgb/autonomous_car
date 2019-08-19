#include "crossroadstate.h"

CrossroadState::CrossroadState() : State() {}

CrossroadState::CrossroadState(FnPtr routine_) : State(routine_) {}

CrossroadState::~CrossroadState() {}

CrossroadState& CrossroadState::operator=(const CrossroadState& state) {}

void CrossroadState::run()
{
    std::cout << "State: CrossroadState" << std::endl;
}