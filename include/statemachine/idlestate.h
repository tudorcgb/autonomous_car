#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "state.h"

class IdleState : public State
{
public:
    IdleState();
    IdleState(FnPtr routine_);
    ~IdleState();
    IdleState(const IdleState& state);
    IdleState& operator=(const IdleState& state);
    void run() override;
private:

};

#endif // IDLESTATE_H