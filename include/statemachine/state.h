#ifndef STATE_H
#define STATE_H

#include <functional>

#include "observer.h"

#ifndef FN_PTR
// typedef void* (*FnPtr)(void*);
typedef std::function<void(void*)> FnPtr;
#endif // FNPTR

class State
{
public:
    State();
    State(FnPtr routine_);
    ~State();
    State(const State& state);
    State& operator=(const State& state);
    void changeRoutine(FnPtr routine_);
    virtual void run() = 0;
private:
    FnPtr routine;
    bool running;
};

#endif // STATE_H