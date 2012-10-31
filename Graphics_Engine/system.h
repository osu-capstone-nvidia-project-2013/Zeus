// Author: Zander Clucas
// 2013 Capstone Project
// system.h
// Controls windows and the graphics class.
#ifndef ZEUS_SYSTEM_H
#define ZEUS_SYSTEM_H

// Speeds up building
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "input.h"
#include "graphics.h"

// Handles creation of a window, running the graphics program
// and exiting cleanly.
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif // ZEUS_SYSTEM_H

