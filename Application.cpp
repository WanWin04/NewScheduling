#include "Application.hpp"

Application::Application() {}

void Application::run(char* source, char* destination) {
    InputHandler input(source);

    int algorithmID = input.algorithmID;

    Scheduler* scheduler = nullptr;

    switch (algorithmID) {
    case 1:
        
        break;
    case 2:
        
        break;
    case 3:
        scheduler = new SJF(input);
        break;
    case 4:
        scheduler = new SRTN(input);
        break;
    default:
        std::cerr << "Unknown algorithm ID." << std::endl;
        return;
    }

    if (scheduler) {
        scheduler->execute();
        scheduler->exportData(destination);
        std::cout << "Build Successfully!" << std::endl;
        delete scheduler;
    }
}