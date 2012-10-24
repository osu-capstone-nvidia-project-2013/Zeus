// Author: Zander Clucas
// 2013 Capstone Project
// system.h
// Controls windows and the graphics class.
#ifndef ZEUS_SYSTEM_H
#define ZEUS_SYSTEM_H

// Speeds up building
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "graphics.h"

// Handles creation of a window, running the graphics program
// and exiting cleanly.
class SystemClass {
public:
    //Constructors
    SystemClass();
    SystemClass(const SystemClass&);

    //Destructors
    ~SystemClass();

    //Methods
    bool Initialize();
    void Shutdown();
    void Run();

    //LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    //Methods
    bool Frame();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();

    //Data Members
    LPCWSTR m_application_name_;
    HINSTANCE m_hinstance_;
    HWND m_hwnd_;

    GraphicsClass* m_Graphics_;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif // ZEUS_SYSTEM_H

