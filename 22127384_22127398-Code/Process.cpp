#include "Process.hpp"

Process::Process() : ID(0), arrivalTime(0), turnAroundTime(0), waitingTime(0), startReadyQueue(0), state(false) {}

Process::~Process()
{
    CPUBurst.clear();
    resourceBurst.clear();
}