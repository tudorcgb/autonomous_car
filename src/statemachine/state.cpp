#include "state.h"

State::State() : running(false), routine(NULL) {}

State::State(FnPtr routine_) : running(false), routine(NULL) 
{
    this->routine = routine_;
}

State::~State()
{
    this->running = false;
}

State::State(const State& state)
{
    this->routine = state.routine;
    this->running = state.running;
}

State& State::operator=(const State& state)
{
    this->routine = routine;
    this->running = running;
    return *this;
}

void State::changeRoutine(FnPtr routine_)
{
    this->routine = routine_;
}