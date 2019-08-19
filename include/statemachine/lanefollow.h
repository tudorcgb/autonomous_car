#ifndef LANEFOLLOW_H
#define LANEFOLLOW_H

#include "state.h"

class LaneFollowState : public State
{
public:
    LaneFollowState();
    LaneFollowState(FnPtr routine_);
    ~LaneFollowState();
    LaneFollowState(const LaneFollowState& state);
    LaneFollowState& operator=(const LaneFollowState& state);
    void run() override;
private:

};

#endif // LANEFOLLOW_H