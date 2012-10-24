// Author: Zander Clucas
// 2013 Capstone Project
// system.cpp
// Initializes, runs and shuts down windows control.
#include "system.h"

SystemClass::SystemClass() {

    m_Graphics_ = NULL;
}

SystemClass::SystemClass(const SystemClass& other) {}

SystemClass::~SystemClass() {}

// Sets up the application, goes into a message processing loop.
bool SystemClass::Initialize() {
    int screen_width = 0;
    int screen_height = 0;
    bool result = false;

    //Initialize the windows api
    InitializeWindows(screen_width, screen_height);

    //Create the graphics object.
    m_Graphics_ = new GraphicsClass;
    if(!m_Graphics_) {
        return false;
    }

    //Initialize the graphics object
    result = m_Graphics_->Initialize(screen_width, screen_height, m_hwnd_);
    if(!result) {
        return false;
    }

    return true;
}

//Performs the cleanup. Releases and shuts down everything associated with it.
void SystemClass::Shutdown() {
    // Release the graphics object
    if(m_Graphics_) {
        m_Graphics_->Shutdown();
        delete m_Graphics_;
        m_Graphics_ = NULL;
    }

    //Shutdown the window
    ShutdownWindows();

    return;
}

//Loops and performs all the processing until it quits. 
void SystemClass::Run() {
    MSG msg;
    bool done = false;
    bool result = false;

    //Initialize the message structure
    ZeroMemory(&msg, sizeof(MSG));

    //Loop until there is a quit message from the window or the user.
    while(!done) {
        //Handle the windows messages.
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //If windows signalss to end the application then exit out
        if(msg.message == WM_QUIT) {
            done = true;
        } else {
            //Otherwise do the frame processing
            result = Frame();
            if(!result) {
                done = true;
            }
        }
    }

    return;
}

// Processes the graphics for the frame.
bool SystemClass::Frame() {
    // Do the frame processing for the graphics object.
    bool result = m_Graphics_->Frame();
    if(!result) {
        return false;
    }

    return true;
}

//// Handles messages from the windows system.
//LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
//    switch(umsg) {
//    //Later handle keyboard inputs
//    default: {
//            return DefWindowProc(hwnd, umsg, wparam, lparam);
//        }
//    }
//}

//Builds the window
void SystemClass::InitializeWindows(int& screen_width, int& screen_height) {
    // Get and external pointer to this object
    ApplicationHandle = this;

    // Get the instance of this application
    m_hinstance_ = GetModuleHandle(NULL);

    // Give the application a name.
    m_application_name_ = L"Zeus Engine";

    // Setup the windows class with default settings.
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance_;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_application_name_;
    wc.cbSize = sizeof(WNDCLASSEX);
    
    // Register the window class.
    RegisterClassEx(&wc);

    // Determine the resolution of the clients desktop screen
    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
    DEVMODE dm_screen_settings;
    int pos_x = 0;
    int pos_y = 0;
    if(FULL_SCREEN) {
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&dm_screen_settings, 0 , sizeof(dm_screen_settings));
        dm_screen_settings.dmSize = sizeof(dm_screen_settings);
        dm_screen_settings.dmPelsWidth = (unsigned long)screen_width;
        dm_screen_settings.dmPelsHeight = (unsigned long)screen_height;
        dm_screen_settings.dmBitsPerPel = 32;
        dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        pos_x = pos_y = 0;
    } else {
        // If windowed then set it to 800x600 resolution.
        screen_width = 800;
        screen_height = 600;

        // Place the window in the middle of the screen.
        pos_x = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
        pos_y = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
    }

    //Create the window with the screen settings and get the handle to it.
    /*m_hwnd_ = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, m_application_name_, 
        m_application_name_, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        pos_x, pos_y, screen_width, screen_height, NULL, NULL, m_hinstance_, NULL);*/
    m_hwnd_ = CreateWindow( m_application_name_, m_application_name_, WS_OVERLAPPEDWINDOW,
        pos_x, pos_y, screen_width, screen_height, NULL, NULL, m_hinstance_, NULL );

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_hwnd_, SW_SHOW);
    SetForegroundWindow(m_hwnd_);
    SetFocus(m_hwnd_);

    return;
}

// Returns the screen settings to normal and releases the window and handles
void SystemClass::ShutdownWindows() {
    // Show the mouse cursor.
    ShowCursor(true);

    // Fix the display settings if leaving full screen mode
    if(FULL_SCREEN) {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove the window
    DestroyWindow(m_hwnd_);
    m_hwnd_ = NULL;

    // Remove the application instance.
    UnregisterClass(m_application_name_, m_hinstance_);
    m_hinstance_ = NULL;

    // Release the pointer to this class
    ApplicationHandle = NULL;

    return;
}

// Recieves messages from windows
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    switch(umessage) {
        // Check if the window is being destroyed.
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        // Check if the window is being closed
        case WM_CLOSE: {
            PostQuitMessage(0);
            return 0;
        }

        // All other messages pass to the message handle in the system class.
        default: {
            return DefWindowProc( hwnd, umessage, wparam, lparam);
        }
    }
}