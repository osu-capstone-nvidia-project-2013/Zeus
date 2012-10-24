// Author: Zander Clucas
// 2013 Capstone Project
// graphics.h
// Links the graphics to the window.
#ifndef ZEUS_GRAPHICS_H
#define ZEUS_GRAPHICS_H

#include <windows.h>

// So we load only the right files
#define DX_V_10 // DX_V_11

//Constants
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Wraps up all the graphics functions to interact with 
// SystemClass and windows.
class GraphicsClass {
public:
    //Constructors
    GraphicsClass();
    GraphicsClass(const GraphicsClass&);

    //Destructors
    ~GraphicsClass();

    //Methods
    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame();

private:
    bool Render();

};
/*
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
*/
#endif // ZEUS_GRAPHICS_H
