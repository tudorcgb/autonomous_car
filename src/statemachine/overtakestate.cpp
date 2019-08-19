#include "overtakestate.h"

OvertakeState::OvertakeState() : State() {}

OvertakeState::OvertakeState(FnPtr routine_) : State(routine_) {}

OvertakeState::~OvertakeState() {}

OvertakeState::OvertakeState(const OvertakeState& state) {}

OvertakeState& OvertakeState::operator=(const OvertakeState& state) {}

void OvertakeState::run()
{
    std::cout << "State: Overtake" << std::endl;
}