// Author: Zander Clucas
// 2013 Capstone Project
// graphics.h
// Links the graphics to the window.
#ifndef ZEUS_GRAPHICS_H
#define ZEUS_GRAPHICS_H

#include "d3d.h"
#include "geometry.h"
#include "colorshader.h"
#include "camera.h"

//Constants
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Wraps up all the graphics functions to interact with 
// SystemClass and windows.
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};

#endif // ZEUS_GRAPHICS_H
