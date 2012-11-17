// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include "d3dxclass.h"
#include "modelclass.h"
#include "objectclass.h"



// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{

	ModelClass *sphereModel;
	ObjectClass *sphereObject;
	D3dxClass *device;

    HWND hWnd;
    WNDCLASSEX wc;
	
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
	ZeroMemory(&device, sizeof(D3dxClass));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Our First Direct3D Program",
                          WS_OVERLAPPEDWINDOW,
                          300,
                          300,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    device = new D3dxClass(hWnd);

	sphereObject = new ObjectClass();

	sphereModel = new ModelClass();
	//// set up the models
	sphereModel->CreateSphere(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), device);

	sphereModel->SetSphere(sphereObject);

    MSG msg;

    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

        sphereObject->RenderObject(device);
    }

    // clean up DirectX and COM
    device->CleanD3D();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}



