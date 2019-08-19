#include "parkingstate.h"

ParkingState::ParkingState() : State() {}

ParkingState::ParkingState(FnPtr routine_) : State(routine_) {}

ParkingState::~ParkingState() 
{
    // TODO: running = false;
}

ParkingState::ParkingState(const ParkingState& state) : State(state) {}

ParkingState& ParkingState::operator=(const ParkingState& state) {}

void ParkingState::run()
{
    std::cout << "State: parking" << std::endl;
}