// Author: Zander Clucas
// 2013 Capstone Project
// main.cpp
// Entry point for the program.
#include "system.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
    SystemClass* System;
    bool result;

    // Create the system object.
    System = new SystemClass;
    if(!System) {
        return 0;
    }

    // Initialize and run the system object.
    result = System->Initialize();
    if(result) {
        System->Run();
    }

    // Shutdown and release the system object.
    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}