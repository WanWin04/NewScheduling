#include "RR.hpp"

RR::RR() : Scheduler() {}

RR::RR(InputHandler &input) : Scheduler(input.processes, input.timeQuantum) {}

void RR::execute()
{
    std::vector<Process *> tempReadyQueue; // used to handle priority in RR
    std::vector<Process *> processes = _processes;
    std::sort(processes.begin(), processes.end(), [](Process *a, Process *b)
              { return a->arrivalTime < b->arrivalTime; });

    int currentTime = 0;
    int quantum = timeQuantum;

    while (!isTerminated(processes, _readyQueue, _blockedQueue))
    {
        // add process into ready queue
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i]->arrivalTime == currentTime)
            {
                _readyQueue.push_back(processes[i]);
            }
        }

        std::reverse(tempReadyQueue.begin(), tempReadyQueue.end());
        for (int i = 0; i < tempReadyQueue.size(); i++)
        {
            _readyQueue.push_back(tempReadyQueue[i]);
        }
        tempReadyQueue.clear();

        // choose process from ready queue
        if (currentProcessOnCPU == nullptr && _readyQueue.size() != 0)
        {
            currentProcessOnCPU = _readyQueue.front();
            _readyQueue.erase(_readyQueue.begin());
            currentProcessOnCPU->waitingTime += currentTime - currentProcessOnCPU->startReadyQueue;
        }

        // choose process from blocked queue
        if (currentProcessOnR == nullptr && _blockedQueue.size() != 0)
        {
            currentProcessOnR = _blockedQueue.front();
            _blockedQueue.erase(_blockedQueue.begin());
        }

        // process on CPU
        if (currentProcessOnCPU != nullptr)
        {
            _CPU.push_back(currentProcessOnCPU);
            currentProcessOnCPU->CPUBurst.front()--;
            quantum--;

            if (currentProcessOnCPU->CPUBurst[0] == 0)
            {
                // remove CPU burst done
                currentProcessOnCPU->CPUBurst.erase(currentProcessOnCPU->CPUBurst.begin());
                // if process has resource burst
                if (currentProcessOnCPU->resourceBurst.size() != 0)
                {
                    _blockedQueue.push_back(currentProcessOnCPU);
                }
                else
                {
                    currentProcessOnCPU->turnAroundTime = currentTime + 1 - currentProcessOnCPU->arrivalTime;
                    if (currentProcessOnCPU->CPUBurst.size() == 0)
                    {
                        // delete process from the list processes
                        deleteProcess(processes, currentProcessOnCPU);
                    }
                }
                currentProcessOnCPU = nullptr;
            }
            // handle quantum time
            if (currentProcessOnCPU == nullptr)
            {
                quantum = timeQuantum;
            }
            if (currentProcessOnCPU != nullptr && quantum == 0)
            {
                tempReadyQueue.push_back(currentProcessOnCPU);
                currentProcessOnCPU->startReadyQueue = currentTime + 1;
                currentProcessOnCPU = nullptr;
                quantum = timeQuantum;
            }
        }
        else
        {
            _CPU.push_back(&emptyProcess);
        }

        // process on resource
        if (currentProcessOnR != nullptr)
        {
            _R.push_back(currentProcessOnR);
            currentProcessOnR->resourceBurst.front()--;
            if (currentProcessOnR->resourceBurst[0] == 0)
            {
                currentProcessOnR->resourceBurst.erase(currentProcessOnR->resourceBurst.begin());

                if (currentProcessOnR->CPUBurst.size() != 0)
                {
                    tempReadyQueue.push_back(currentProcessOnR);
                    currentProcessOnR->startReadyQueue = currentTime + 1;
                }
                else
                {
                    currentProcessOnR->turnAroundTime = currentTime + 1 - currentProcessOnR->arrivalTime;
                    if (currentProcessOnR->resourceBurst.size() == 0)
                    {
                        // delete process from the list processes
                        deleteProcess(processes, currentProcessOnR);
                    }
                }
                currentProcessOnR = nullptr;
            }
        }
        else
        {
            _R.push_back(&emptyProcess);
        }

        currentTime++;
    }
}