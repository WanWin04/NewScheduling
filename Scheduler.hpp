#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>

#include "Process.hpp"

const static int CPU_BURST_INDEX = 0;

class Scheduler
{
public:
    Scheduler();
    Scheduler(std::vector<Process *> processes, int timeQuantum);

    // export data to output file
    void exportData(const std::string &filename);

    // pure virtual function
    virtual void execute() = 0;

protected:
    Process temp;
    int timeQuantum;
    std::vector<Process *> _processes;
    std::vector<Process *> _readyQueue;
    std::vector<Process *> _blockedQueue;
    std::vector<Process *> _CPU;
    std::vector<Process *> _R;

    // delete Delete completed process 
    void deleteProcess(std::vector<Process *> &processes, Process *process);

    // chech if all process is terminated
    bool isTerminated(std::vector<Process *> processes, std::vector<Process *> readyQueue, std::vector<Process *> blockedQueue);
};

#endif