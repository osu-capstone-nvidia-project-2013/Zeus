// Author: Zander Clucas
// 2013 Capstone Project
// graphics.cpp
// Connects the graphics to the window control in systemclass.
#include "graphics.h"

GraphicsClass::GraphicsClass() {}

GraphicsClass::GraphicsClass(const GraphicsClass& other) {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
    return true;
}

void GraphicsClass::Shutdown() {
    return;
}

bool GraphicsClass::Frame() {
    return true;
}

bool GraphicsClass::Render() {
    return true;
}