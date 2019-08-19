#ifndef PARKINGSTATE_H
#define PARKINGSTATE_H

#include "state.h"

class ParkingState : public State
{
public:
    ParkingState();
    ParkingState(FnPtr routine_);
    ~ParkingState();
    ParkingState(const ParkingState& state);
    ParkingState& operator=(const ParkingState& state);
    void run() override;
private:
};

#endif // PARKINGSTATE_H