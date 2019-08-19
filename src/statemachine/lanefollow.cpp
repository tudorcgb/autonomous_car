# include "lanefollow.h"

LaneFollowState::LaneFollowState() : State() {}

LaneFollowState::LaneFollowState(FnPtr routine_) : State(routine_) {}

LaneFollowState::~LaneFollowState() {}

LaneFollowState::LaneFollowState(const LaneFollowState& state) {}

LaneFollowState& LaneFollowState::operator=(const LaneFollowState& state) {}

void LaneFollowState::run()
{
    std::cout << "State: LaneFollow" << std::endl;
}