#if 0

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <thread>
#include <chrono>

#include "timer.h"
#include "image.h"

// static void funct1(void)
// {
//     printf("1\n");
// }

// static void timerHandler(int sig, siginfo_t *si, void *uc)
// {
//     timer_t *tidp;
//     tidp = si->si_value.sival_ptr;
//     funct1();
// }

// static int makeTimer( char *name, timer_t *timerID, int expireMS, int intervalMS )
// {
//     struct sigevent         te;
//     struct itimerspec       its;
//     struct sigaction        sa;
//     int                     sigNo = SIGRTMIN;

//     /* Set up signal handler. */
//     sa.sa_flags = SA_SIGINFO;
//     sa.sa_sigaction = timerHandler;
//     sigemptyset(&sa.sa_mask);
//     if (sigaction(sigNo, &sa, NULL) == -1)
//     {
//         fprintf(stderr, "Failed to setup signal handling for %s.\n", name);
//         return(-1);
//     }

//     /* Set and enable alarm */
//     te.sigev_notify = SIGEV_SIGNAL;
//     te.sigev_signo = sigNo;
//     te.sigev_value.sival_ptr = timerID;
//     timer_create(CLOCK_REALTIME, &te, timerID);

//     its.it_interval.tv_sec = 0;
//     its.it_interval.tv_nsec = intervalMS * 1000000;
//     its.it_value.tv_sec = 0;
//     its.it_value.tv_nsec = expireMS * 1000000;
//     timer_settime(*timerID, 0, &its, NULL);

//     return(0);
// }

// timer_t firstTimerID;

// int main(int argc, char **argv)
// {
//     int rc;
//     rc = makeTimer("First Timer", &firstTimerID, 40, 40);
//     std::chrono::milliseconds timespan(111605); // or whatever

//     std::this_thread::sleep_for(timespan);
//     return 0;
// }

Image img("/home/mihai/Workspace/BOSCH_2019/Dataset/16_apr/output_1555404694.9536414.avi");

int main(int argc, char** argv)
{
    // auto callback = std::bind(&Image::nextFrame, img);
    Timer t("t", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();

    sleep(20);
    t.stop();
    img.close();

    return 0;
}

#endif