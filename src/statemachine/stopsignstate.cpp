#include "stopsignstate.h"

StopSignState::StopSignState() : State() {}

StopSignState::StopSignState(FnPtr routine_) : State(routine_) {}

StopSignState::~StopSignState() 
{
    // TODO: running = false;
}

StopSignState::StopSignState(const StopSignState& state) : State(state) {}

StopSignState& StopSignState::operator=(const StopSignState& state) {}

void StopSignState::run()
{
    std::cout << "State: stopSign" << std::endl;
}