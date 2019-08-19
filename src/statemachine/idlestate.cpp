#include "idlestate.h"

IdleState::IdleState() : State() {}

IdleState::IdleState(FnPtr routine_) : State(routine_) {}

IdleState::~IdleState() {}

IdleState::IdleState(const IdleState& state) {}

IdleState& IdleState::operator=(const IdleState& state) {}

void IdleState::run()
{
    std::cout << "State: Idle" << std::endl;
}