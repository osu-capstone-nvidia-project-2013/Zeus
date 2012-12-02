// include the basic windows header files and the Direct3D header files
#include "main.h"
// define the screen resolution


// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DepthStencilView *zbuffer;    
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11Buffer *vCBuffer;                // the pointer to the constant buffer

ID3D11ShaderResourceView *pTexture;    // the pointer to the texture

ID3D11Buffer *mCBuffer; 
ID3D11Buffer *pCBuffer;
GeometryClass *geometry;
ShaderClass *shaderclass;
LIGHT *light;
MATRICES *matrices;

int textoggle = true;
int normtoggle = true;



float timeAtGameStart;
UINT64 ticksPerSecond;

// state objects
ID3D11RasterizerState *pRS;            // the default rasterizer state
ID3D11SamplerState *pSS;               // the default sampler state
ID3D11BlendState *pBS;                 // a typicl blend state


// Global Camera
D3DXMATRIX matView, matWorldX;

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void CleanD3D(ObjectClass *obj);        // closes Direct3D and releases memory
void InitPipeline(void);    // loads and prepares the shaders
void InitStates();
void InitGameTime();
float GetGameTime();
float Ranf( float low, float high );


// Global variables for measuring fps
float lastUpdate        = 0;
float fpsUpdateInterval = 0.5f;
UINT  numFrames         = 0;
float fps               = 0;
// Called once for every frame
void UpdateFPS()
{
  numFrames++;
  float currentUpdate = GetGameTime();
  if( currentUpdate - lastUpdate > fpsUpdateInterval )
  {
    fps = numFrames / (currentUpdate - lastUpdate);
    lastUpdate = currentUpdate;
    numFrames = 0;
  }
}


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
                          L"Zeus Graphics Engine",
                          WS_OVERLAPPEDWINDOW,
                          0,
                          0,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

	//create new camera with default values
	CameraClass *camera = new CameraClass();

    // set up and initialize Direct3D
    InitD3D(hWnd);

	InitGameTime();
    
    shaderclass = new ShaderClass();
    shaderclass->SetShaders(dev, devcon);

	//Set lighting
    light = new LIGHT();

    light->ambientcolor = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
    light->diffusecolor = D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f);
    light->specularcolor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    light->specularpower = 32.0f;
    light->lightdirection = D3DXVECTOR3(0.0f, -2.0f, -2.0f);

	//Setup for updating matrices
    matrices = new MATRICES();

	D3DXMATRIX matRotate, matRotateY, matRotateZ, matRotateX, matTrans, matProjection, matScale, matFinal, matTransposed;
    D3DXVECTOR4 tempVec4;
		
    D3DXMatrixIdentity(&matWorldX);



	D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 0.0f);

	// Set the particles
    geometry = new GeometryClass();
	

	D3DXMatrixTranslation(&matTrans, 0.0f, 0.5f, 0.0f);
	MATRICES *mats = new MATRICES();
	
	mats->matProjection = camera->matProjection;
	mats->matView = camera->matView;
	mats->cameraPosition = D3DXVECTOR3(0.0f, 0.0f, 5.5f);


	D3DXMatrixTranslation(&matTrans, 0.0f, 0.4f, 0.0f);
	mats->matWorld = matTrans;
	geometry->CreateQuad(dev, devcon, 1.0f,1.0f,1.0f,1.5f);
	geometry->SetTexture(dev, L"firtexture.jpg", 0);
	geometry->SetAlpha(dev, L"firalpha.jpg", 0);
	geometry->SetMapping(1.,1.,0.,0.,0.,0);
	geometry->SetLight(light, 0);
	geometry->SetMatrices(mats,0);
	
    geometry->objects[0]->x = 0.0f;
    geometry->objects[0]->y = 0.4f;
    geometry->objects[0]->z = 0.0f;
	

	//A cow
	geometry->LoadObject( dev, devcon, "cow.obj", D3DXVECTOR4(0.1f,0.8f,0.3f,1.0f) );
	geometry->SetTexture(dev,L"cowtex.png",1);
	D3DXMatrixTranslation(&matTrans, 2.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&matRotateY, D3DX_PI/2);
	geometry->SetMapping(1.0f, 0.0f, 0.0f,0.0f,0., 1);
	geometry->objects[1]->x = 2.0f;
    geometry->objects[1]->y = 0.0f;
    geometry->objects[1]->z = 0.0f;
	mats->matWorld = matRotateY * matTrans;
	geometry->SetLight(light, 1);
	geometry->SetMatrices(mats,1);

//Create the master particle
	geometry->CreateQuad(dev,devcon, 1.0f,1.0f,1.0f, 0.01f, "snowParticle");
	geometry->SetAlpha(dev, L"snowalphasmallest.png", 2);
	geometry->SetMapping(0.,1.,0.,1.,0.,2);
	geometry->SetLight(light, 2);

	// Physics Init
	geometry->objects[2]->x0 = Ranf(-4.0f, 4.0f);
    geometry->objects[2]->y0 = 10.0f;
    geometry->objects[2]->z0 = Ranf(-4.0f, 4.0f);
    geometry->objects[2]->x = geometry->objects[2]->x0;
    geometry->objects[2]->y = geometry->objects[2]->y0;
    geometry->objects[2]->z = geometry->objects[2]->z0;

    geometry->objects[2]->vx0 = Ranf(-0.0015f, 0.0015f);
    geometry->objects[2]->vy0 = Ranf(-0.35f, -0.085f);
    geometry->objects[2]->vz0 = Ranf(-0.0015f, 0.0015f);
    geometry->objects[2]->vx = geometry->objects[2]->vx0; 
    geometry->objects[2]->vy = geometry->objects[2]->vy0;
    geometry->objects[2]->vz = geometry->objects[2]->vz0;
		
	D3DXMatrixTranslation(&matTrans, geometry->objects[2]->x, geometry->objects[2]->y, geometry->objects[2]->z );
	mats->matWorld = matTrans * matWorldX;
		
	geometry->SetMatrices(mats, 2);

	for(int i = 3; i < MAXPARTICLES; i++)
	{
		geometry->LoadObject(dev, devcon, "snowParticle", D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f));

		// Physics Init
		geometry->objects[i]->x0 = Ranf(-10.0f, 10.0f);
        geometry->objects[i]->y0 = Ranf(-1.0f, 10.0f);
        geometry->objects[i]->z0 = Ranf(-10.0f, 10.0f);
        geometry->objects[i]->x = geometry->objects[i]->x0;
        geometry->objects[i]->y = geometry->objects[i]->y0;
        geometry->objects[i]->z = geometry->objects[i]->z0;

        geometry->objects[i]->vx0 = Ranf(-0.001f, 0.001f);
        geometry->objects[i]->vy0 = Ranf(-0.55f, -0.025f);
        geometry->objects[i]->vz0 = Ranf(-0.001f, 0.001f);
        geometry->objects[i]->vx = geometry->objects[i]->vx0;
        geometry->objects[i]->vy = geometry->objects[i]->vy0;
        geometry->objects[i]->vz = geometry->objects[i]->vz0;
		
		D3DXMatrixTranslation(&matTrans, geometry->objects[i]->x, geometry->objects[i]->y, geometry->objects[i]->z );
		mats->matWorld = matTrans * matWorldX;
		
		geometry->SetMatrices(mats, i);
	}

	//Create the plane last to make sorting easier
	geometry->CreatePlane(dev, devcon, -1.0f);
	D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 0.0f);
	matrices->matWorld = matTrans;
	matrices->matProjection = camera->matProjection;
    matrices->matView = camera->matView;;
    matrices->cameraPosition = D3DXVECTOR3(0.0f, 1.0f, 5.5f);
	geometry->SetMatrices(matrices,MAXPARTICLES);
	geometry->SetTexture(dev,L"TER_ForestGroundPlayable_B.png",MAXPARTICLES);
	geometry->SetNormal(dev,L"TER_ForestGroundPlayable_B_NM.png",MAXPARTICLES);
	geometry->SetAlpha(dev, L"snowalphasmallest.png", MAXPARTICLES);
	geometry->SetMapping(1.,0.,1.,0.,0.,MAXPARTICLES);
	geometry->SetLight(light, MAXPARTICLES);
	geometry->objects[MAXPARTICLES]->x = 0.0f;
    geometry->objects[MAXPARTICLES]->y = -1.0f;
    geometry->objects[MAXPARTICLES]->z = 0.0f;

    // enter the main loop:
	//MATRICES *mats = new MATRICES();

    MSG msg;
	double angle;
	float rotation;
    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

        //static float Time = 0.0f; Time += 0.000125f;
        //static float LightTime = 0.0f; LightTime = +0.0005f;

		float Time = GetGameTime();

		UpdateFPS();

		int feeps = fps;
		
		D3DXMatrixTranslation(&matTrans, 0.0f, -2.0f, 0.0f);
		mats->matWorld = matTrans * matWorldX;

		geometry->SetMatrices(mats,2);
		D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 0.0f);
		mats->matWorld = matTrans * matWorldX;
		geometry->SetMatrices(mats,MAXPARTICLES);


		D3DXVECTOR3 modelPosition = D3DXVECTOR3(geometry->objects[0]->x, geometry->objects[0]->y, geometry->objects[0]->z);
		D3DXVec3TransformCoord(&modelPosition, &modelPosition, &matWorldX);
		// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
		angle = atan2(modelPosition.x - 0.0f, modelPosition.z - 5.5f) * (180.0 / D3DX_PI);

		// Convert rotation into radians.
		rotation = 2 * (float)angle * 0.0174532925f;

		// Setup the rotation the billboard at the origin using the world matrix.
		D3DXMatrixRotationY(&matFinal, rotation);

		// Setup the translation matrix from the billboard model.
		D3DXMatrixTranslation(&matTrans, modelPosition.x, modelPosition.y, modelPosition.z);

		// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
		D3DXMatrixMultiply(&matFinal, &matFinal, &matTrans); 

		mats->matWorld = matFinal;
		geometry->SetMatrices(mats,0);

		// Setup the rotation the billboard at the origin using the world matrix.
		D3DXMatrixRotationY(&matRotateY, Time * D3DX_PI/2);
		D3DXMatrixRotationY(&matRotateX, D3DX_PI/2);
		D3DXMatrixTranslation(&matTrans, 2.0f, (sinf(Time * 10.0f)/4.0f) - 0.2f, 0.0f);
		mats->matWorld = matRotateX * matTrans * matRotateY * matWorldX;
		geometry->SetMatrices(mats,1);

		for(int i = 3; i < MAXPARTICLES; i++)
		{
			// Physics Init
			
			if(geometry->objects[i]->y < -1.0f || geometry->objects[i]->x < -15.0f || geometry->objects[i]->x > 15.0f || geometry->objects[i]->z < -15.0f || geometry->objects[i]->z > 15.0f)
			{
				geometry->objects[i]->y = 10.0f;
				geometry->objects[i]->x0 = Ranf(-10.0f, 10.0f);
				geometry->objects[i]->z0 = Ranf(-10.0f, 10.0f);
				geometry->objects[i]->x = geometry->objects[i]->x0;
				geometry->objects[i]->z = geometry->objects[i]->z0;

			} else {
				geometry->objects[i]->x += fmodf(geometry->objects[i]->vx * Time, 0.002f) - 0.001;
				geometry->objects[i]->y = (fmodf(geometry->objects[i]->y0 + geometry->objects[i]->vy * Time - 11.0, 11.0)) + 10.0f;
				geometry->objects[i]->z += fmodf(geometry->objects[i]->vz * Time, 0.002f) - 0.001;
			}
			
			
			D3DXMatrixTranslation(&matTrans, geometry->objects[i]->x, geometry->objects[i]->y, geometry->objects[i]->z );

			//D3DXMatrixRotationY(&matRotateY,
			modelPosition = D3DXVECTOR3(geometry->objects[i]->x, geometry->objects[i]->y, geometry->objects[i]->z);
			D3DXVec3TransformCoord(&modelPosition, &modelPosition, &matWorldX);
			// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
			angle = atan2(modelPosition.x - 0.0f, modelPosition.z - 5.5f) * (180.0 / D3DX_PI);

			// Convert rotation into radians.
			rotation = 2 * (float)angle * 0.0174532925f;

			// Setup the rotation the billboard at the origin using the world matrix.
			D3DXMatrixRotationY(&matFinal, rotation);

			// Setup the translation matrix from the billboard model.
			D3DXMatrixTranslation(&matTrans, modelPosition.x, modelPosition.y, modelPosition.z);

			// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
			D3DXMatrixMultiply(&matFinal, &matFinal, &matTrans); 

			mats->matWorld = matFinal;
			mats->matProjection = camera->matProjection;
			mats->matView = camera->matView;;
			mats->cameraPosition = D3DXVECTOR3(0.0f, 0.0f, 5.5f);
			geometry->SetMatrices(mats, i);
		}

        geometry->Render(dev, devcon, backbuffer, swapchain, pCBuffer, vCBuffer, mCBuffer, zbuffer, pTexture,
                        pBS, pSS, pRS);
    }


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
        // Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}break;
        case WM_KEYUP:
            {
                switch(wParam)
                {
                    // Effects toggling
                    case 0x54: // T key has been pressed
                    {
                        if(textoggle)
                        {
                            geometry->SetMapping(0.,1.,0.,0.,0.,0);
                            textoggle = false;
                        }
                        else
                        {
                            geometry->SetMapping(1.,1.,0.,0.,0.,0);
                            textoggle = true;
                        }
                    }
                    break;
                    case 0x4E: // N key has been pressed
                    {
                        if(normtoggle)
                        {
                            geometry->SetMapping(0.,0.,0.,0.,0.,1);
                            geometry->SetMapping(0.,0.,0.,0.,0.,4);
                            normtoggle = false;
                        }
                        else
                        {
                            geometry->SetMapping(0.,0.,1.,0.,0.,1);
                            geometry->SetMapping(0.,0.,1.,0.,0.,4);
                            normtoggle = true;
                        }
                    }break;
                }
            }
            break;
        case WM_KEYDOWN:
        {
            switch(wParam)
             {
                // Movement
                case 0x57: // W key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixTranslation(&transform, 0.0f, 0.0f, 0.1f);
                    matWorldX = transform * matWorldX;
                }
                break;
                case 0x53: // S key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixTranslation(&transform, 0.0f, 0.0f, -0.1f);
                    matWorldX = transform * matWorldX;
                }
                break;
                case 0x41: // A key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixTranslation(&transform, -0.1f, 0.0f, 0.0f);
                    matWorldX = transform * matWorldX;
                }
                break;
                case 0x44: // D key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixTranslation(&transform, 0.1f, 0.0f, 0.0f);
                    matWorldX = transform * matWorldX;
                }
                break;

                // Look
                case 0x25: // Left arrow key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixRotationY(&transform, 0.01);
                    matWorldX = matWorldX * transform;
                }
                break;
                case 0x26: // Up arrow key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixRotationX(&transform, 0.01);
                    matWorldX = matWorldX * transform;
                }
                break;
                case 0x27: // Right arrow key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixRotationY(&transform, -0.01);
                    matWorldX = matWorldX * transform;
                }
                break;
                case 0x28: // Down arrow key has been pressed
                {
                    D3DXMATRIX transform;
                    D3DXMatrixRotationX(&transform, -0.01);
                    matWorldX = matWorldX * transform;
                }
                break;
            }
        }
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

void InitGameTime()
{
	// We need to know how often the clock is updated
	if( !QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) )
    ticksPerSecond = 1000;
	// If timeAtGameStart is 0 then we get the time since
	// the start of the computer when we call GetGameTime()
	timeAtGameStart = 0;
	timeAtGameStart = GetGameTime();
}

float GetGameTime()
{
  UINT64 ticks;
  float time;
  // This is the number of clock ticks since start
  if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) )
    ticks = (UINT64)timeGetTime();
  // Divide by frequency to get the time in seconds
  time = (float)(__int64)ticks/(float)(__int64)ticksPerSecond;
  // Subtract the time at game start to get
  // the time since the game started
  time -= timeAtGameStart;
  return time;
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

	/*ID3D11Texture2D *pReflectMap;
	dev->CreateRenderTargetView(*/

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


    
    InitStates();
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
    
	dev->CreateBuffer(&bd, NULL, &mCBuffer);
    devcon->PSSetConstantBuffers(1, 1, &mCBuffer);
	
   }

// initializes the states
void InitStates()
{
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.MultisampleEnable = FALSE;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 0.0f;

    dev->CreateRasterizerState(&rd, &pRS);

    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    dev->CreateSamplerState(&sd, &pSS);

    D3D11_BLEND_DESC bd;
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bd.IndependentBlendEnable = FALSE;
    bd.AlphaToCoverageEnable = FALSE;

    dev->CreateBlendState(&bd, &pBS);
}

#define TOP	2147483647.		// 2^31 - 1	

float
Ranf( float low, float high )
{
	long random();		// returns integer 0 - TOP
	float r;		// random number	

	r = (float)rand();

	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}