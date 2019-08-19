#ifndef STOPSIGNSTATE_H
#define STOPSIGNSTATE_H

#include "state.h"

class StopSignState : public State
{
public:
    StopSignState();
    StopSignState(FnPtr routine_);
    ~StopSignState();
    StopSignState(const StopSignState& state);
    StopSignState& operator=(const StopSignState& state);
    void run() override;
};

#endif // STOPSIGNSTATE_H