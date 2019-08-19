#ifndef CROSSROADSTATE_H
#define CROSSROADSTATE_H

#include "state.h"

class CrossroadState : public State
{
public:
    CrossroadState();
    CrossroadState(FnPtr routine_);
    ~CrossroadState();
    CrossroadState(const CrossroadState& state);
    CrossroadState& operator=(const CrossroadState& state);
    void run() override;
private:

};

#endif // CROSSROADSTATE_H