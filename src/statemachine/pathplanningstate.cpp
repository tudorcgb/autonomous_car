#include "pathplanningstate.h"

PathPlanningState::PathPlanningState() : State() {}

PathPlanningState::PathPlanningState(FnPtr routine_) : State(routine_) {}

PathPlanningState::~PathPlanningState() {}

PathPlanningState::PathPlanningState(const PathPlanningState& state) {}

PathPlanningState& PathPlanningState::operator=(const PathPlanningState& state) {}

void PathPlanningState::run()
{
    std::cout << "State: PathPlanning" << std::endl;
}