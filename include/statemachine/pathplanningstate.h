#ifndef PATHPLANNINGSTATE_H
#define PATHPLANNINGSTATE_H

#include "state.h"

class PathPlanningState : public State
{
public:
    PathPlanningState();
    PathPlanningState(FnPtr routine_);
    ~PathPlanningState();
    PathPlanningState(const PathPlanningState& state);
    PathPlanningState& operator=(const PathPlanningState& state);
    void run() override;
private:

};

#endif // PATHPLANNINGSTATE_H