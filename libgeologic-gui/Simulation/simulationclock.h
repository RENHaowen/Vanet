#ifndef SIMULATIONCLOCK_H
#define SIMULATIONCLOCK_H

#include <sys/time.h>

class SimulationClock {
private:
    double time_scale;
    double initial_ts;
    unsigned long long initial_time;
    double current_time;
    struct timeval initial_rt; // Used to compute the time between two updates.
    bool running;

public:
    SimulationClock();
    virtual ~SimulationClock();
    virtual void start();
    virtual void pause();
    virtual void stop();
    virtual void update();
    unsigned long long getCurrentTime() const;
    unsigned long long getInitialTime() const;
    void setInitialTime(unsigned long long initialTime);
    void setTimeScale(double timeScale, bool isRunning=true);
    void setCurrentTime(unsigned long long currentTime);
};

#endif // SIMULATIONCLOCK_H
