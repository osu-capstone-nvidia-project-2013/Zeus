// include the basic windows header files and the Direct3D header files
#include "main.h"
// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer
ID3D11Buffer *pCameraBuffer;		   // the pointer to the camera constant buffer
ObjectClass	*triangleObj;
GeometryClass *geometry;
ShaderClass *shaderclass;

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void CleanD3D(ObjectClass *obj);        // closes Direct3D and releases memory
void InitPipeline(void);    // loads and prepares the shaders

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

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
    InitD3D(hWnd);
	
	shaderclass = new ShaderClass();
	shaderclass->SetShaders(dev, devcon);

	triangleObj = new ObjectClass();
    SetTriangle(triangleObj, dev, devcon);
    geometry = new GeometryClass();
    VERTEX sphere_center;
	sphere_center.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	sphere_center.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	sphere_center.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	geometry->LoadObject(dev, devcon, "cow.obj");
    geometry->CreateSphere(dev, devcon, sphere_center, 1.0f, 30, 30);
	

    // enter the main loop:

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

		D3DXMATRIX matRotate, matTrans, matView, matProjection, matFinal;

		static float Time = 0.0f; Time += 0.001f;

		// create a rotation matrix
		D3DXMatrixRotationY(&matRotate, Time);

		// create a translation matrix
		D3DXMatrixTranslation(&matTrans, 1.5, 0.0f, 0.0f);

		// create a view matrix
		D3DXMatrixLookAtLH(&matView,
						   &D3DXVECTOR3(0.0f, 1.5f, 5.5f),    // the camera position
						   &D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
						   &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

		// create a projection matrix
		D3DXMatrixPerspectiveFovLH(&matProjection,
								   (FLOAT)D3DXToRadian(45),                    // field of view
								   (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
								   1.0f,                                       // near view-plane
								   100.0f);                                    // far view-plane

		// create the final transform
		matFinal =  matRotate * matTrans *  matView * matProjection;

		geometry->SetMatrix(matFinal, 0);

		// set matrix for second object

		D3DXMatrixTranslation(&matTrans, 0.0f, 1.5f, 0.0f);

		D3DXMatrixRotationZ(&matRotate, Time);

		matFinal = matRotate * matTrans * matView * matProjection;

		geometry->SetMatrix(matFinal, 1);
		
        geometry->Render(dev, devcon, backbuffer, swapchain, pCBuffer);
						
	}

    // clean up DirectX and COM
    CleanD3D(triangleObj);

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


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);


    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
}




// this is the function that cleans up Direct3D and COM
void CleanD3D(ObjectClass *obj)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    obj->vBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}



// this function loads and prepares the shaders
void InitPipeline()
{   
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 64;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);
    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
    
}
