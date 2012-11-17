// include the basic windows header files and the Direct3D header files
#include "main.h"
// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DepthStencilView *zbuffer;    
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11Buffer *vCBuffer;                // the pointer to the constant buffer

ID3D11ShaderResourceView *pTexture;    // the pointer to the texture

ID3D11Buffer *pCBuffer;
ObjectClass	*triangleObj;
GeometryClass *geometry;
ShaderClass *shaderclass;
LIGHT *light;
MATRICES *matrices;
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
	sphere_center.color = D3DXVECTOR4(-1.0f, 0.55f, 0.75f, 1.0f);
	sphere_center.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	geometry->LoadObject(dev, devcon, "cow.obj", D3DXVECTOR4(0.45f, 0.65f, 0.20f, 1.0f));
    geometry->CreateSphere(dev, devcon, sphere_center, 1.0f, 30, 30);
	geometry->LoadObject(dev, devcon, "frog.obj",  D3DXVECTOR4(0.0f, 0.65f, 0.20f, 1.0f));
	
	//Set lighting
	light = new LIGHT();

	light->ambientcolor = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	light->diffusecolor = D3DXVECTOR4(1.0f, 0.8f, 0.0f, 1.0f);
	light->specularcolor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	light->specularpower = 32.0f;
	light->lightdirection = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	geometry->SetLight(light, 0);
	geometry->SetLight(light, 1);


	//Setup for updating matrices
	matrices = new MATRICES();


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

		D3DXMATRIX matRotate, matRotateY, matRotateZ, matRotateX, matTrans, matView, matProjection, matScale, matFinal;
		D3DXVECTOR4 tempVec4;

		static float Time = 0.0f; Time += 0.001f;
		static float LightTime = 0.0f; LightTime = +0.001f;

		// create a rotation matrix
		D3DXMatrixRotationY(&matRotateY, Time);
		D3DXMatrixRotationZ(&matRotateZ, Time);

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
	    //matFinal =  matRotate * matTrans *  matView * matProjection;

		matrices->matWorld = matRotateY * matRotateZ * matTrans;
		matrices->matProjection = matProjection;
		matrices->matView = matView;
		matrices->cameraPosition = D3DXVECTOR3(0.0f, 1.5f, 5.5f);

		//update light
		D3DXMatrixRotationY(&matRotateY, LightTime);
		D3DXVec3Transform(&tempVec4, &light->lightdirection, &matRotateY);
		light->lightdirection.x = tempVec4.x;
		light->lightdirection.y = tempVec4.y;
		light->lightdirection.z = tempVec4.z;

		geometry->SetLight(light, 0);
		geometry->SetLight(light, 1);
		geometry->SetLight(light, 2);

		geometry->SetMatrices(matrices, 0);

		// set matrix for second object
		D3DXMatrixRotationX(&matRotateX, Time);
		D3DXMatrixTranslation(&matTrans, 0.0f, 1.5f, 0.0f);


		matrices->matWorld = matRotateX * matTrans;

		geometry->SetMatrices(matrices, 1);



		//set matrix for third
		D3DXMatrixTranslation(&matTrans, 0.0f, 1.5f, 2.0f);

		matrices->matWorld = matRotateY * matRotateZ * matTrans;
		geometry->SetMatrices(matrices, 2);

        geometry->Render(dev, devcon, backbuffer, swapchain, pCBuffer, vCBuffer, zbuffer, pTexture);
						
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
  // create the depth buffer texture
    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = SCREEN_WIDTH;
    texd.Height = SCREEN_HEIGHT;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *pDepthBuffer;
    dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
    pDepthBuffer->Release();


    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

	viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

    devcon->RSSetViewports(1, &viewport);


	

    InitPipeline();
}




// this is the function that cleans up Direct3D and COM
void CleanD3D(ObjectClass *obj)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	
    zbuffer->Release();
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
    bd.ByteWidth = sizeof(MATRICES);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &vCBuffer);
    devcon->VSSetConstantBuffers(0, 1, &vCBuffer);

    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(LIGHT);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);
    devcon->PSSetConstantBuffers(0, 1, &pCBuffer);
    
	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
										   L"Wood.png",    // load Wood.png in the local folder
										   NULL,           // no additional information
										   NULL,           // no multithreading
										   &pTexture,      // address of the shader-resource-view
										   NULL);          // no multithreading
	}
