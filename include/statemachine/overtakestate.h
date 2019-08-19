#ifndef OVERTAKE_H
#define OVERTAKE_H

#include "state.h"

class OvertakeState : public State
{
public:
    OvertakeState();
    OvertakeState(FnPtr routine_);
    ~OvertakeState();
    OvertakeState(const OvertakeState& state);
    OvertakeState& operator=(const OvertakeState& state);
    void run() override;
private:

};


#endif // OVERTAKE_H