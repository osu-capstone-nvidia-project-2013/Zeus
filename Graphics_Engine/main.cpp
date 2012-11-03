// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define PI 3.14159265358979323846f

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11PixelShader *pShaderInterp;	   // the pointer to the pixel shader that interpolates
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer
ID3D11Buffer *circleBuff;			   // the pointer to the circle vertex buffer
ID3D11Buffer *circleIndex;			   // the pointer to the circle index buffer
ID3D11Buffer *sphereBuff;			   // the pointer to the sphere vertex buffer
ID3D11Buffer *sphereIndex;			   // the pointer to the sphere index buffer
ID3D11RasterizerState *noCull;		   // the pointer to the rasterizer state
									   // this will disable back side culling


// various buffer structs
struct VERTEX{D3DXVECTOR3 vec; D3DXCOLOR Color;};
struct PERFRAME{D3DXCOLOR Color; FLOAT X, Y, Z;};

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void createSphere(float radius);
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

        RenderFrame();
    }

    // clean up DirectX and COM
    CleanD3D();

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
    scd.SampleDesc.Count = 4;                             // how many multisamples
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
    InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(void)
{
    D3DXMATRIX matTranslate, matRotate, matRotX, matRotY, matRotZ, matView, matProjection, matFinal;

	D3D11_RASTERIZER_DESC rastDesc;
	
	//Set up for no back culling
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;

	dev->CreateRasterizerState(&rastDesc, &noCull);

	devcon->RSSetState(noCull);
	
    static float Time = 0.0f; Time += 0.001f;

    // create a rotation matrix
    D3DXMatrixRotationY(&matRotate, Time);

    // create a view matrix
    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3(1.5f, 0.5f, 1.5f),    // the camera position
                       &D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

    // create a projection matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               (FLOAT)D3DXToRadian(45),                    // field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,                                       // near view-plane
                               100.0f);                                    // far view-plane

	// create translate matrix
	D3DXMatrixTranslation(&matTranslate, 0.5f, 0.5f, 0.0f);

    // create the final transform
    matFinal = matRotate * matTranslate * matView * matProjection;

    // set the new values for the constant buffer
    devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal, 0, 0);


    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

        // select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// set desired pixel shader
		devcon->PSSetShader(pPS, 0, 0);
        // draw the vertex buffer to the back buffer
        devcon->Draw(3, 0);

	//================================================================
	// Draw Circle

		devcon->IASetVertexBuffers(0, 1, &circleBuff, &stride, &offset);

		devcon->IASetIndexBuffer(circleIndex, DXGI_FORMAT_R32_UINT, 0);
		// create the final transform
		
		D3DXMatrixTranslation(&matTranslate, -0.5f, 0.5f, 0.0f);
		
		D3DXMatrixRotationZ(&matRotZ, Time*10);

		matRotate *= matRotZ;

		matFinal = matRotate * matTranslate * matView * matProjection;

		// set the new values for the constant buffer
		devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal, 0, 0);

		// set desired pixel shader
		devcon->PSSetShader(pShaderInterp, 0, 0);

		devcon->DrawIndexed(1080,0,0);
	//================================================================

	rastDesc.CullMode = D3D11_CULL_FRONT;

	dev->CreateRasterizerState(&rastDesc, &noCull);

	devcon->RSSetState(noCull);
	//================================================================
	// draw sphere
		devcon->IASetVertexBuffers(0,1,&sphereBuff,&stride, &offset);

		devcon->IASetIndexBuffer(sphereIndex, DXGI_FORMAT_R32_UINT, 0);

		D3DXMatrixTranslation(&matTranslate, 0.0f, 0.5f, 0.0f);
	
		D3DXMatrixRotationZ(&matRotZ, Time);
		D3DXMatrixRotationY(&matRotY, Time);

		matRotate = matRotZ * matRotY;

		matFinal = matRotate * matView * matProjection;

		// set the new values for the constant buffer
		devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal, 0, 0);
		devcon->DrawIndexed(16284,0,0);


	//
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    pCBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics()
{
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
        {D3DXVECTOR3(0.0f, 0.25f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {D3DXVECTOR3(0.25f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {D3DXVECTOR3(-0.25f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
    };


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer


	//==========================================================
	// Create circle vertices
	VERTEX Circle[181];
	D3DXVECTOR3 normal, current, direction_vec;
	D3DXVECTOR4 result;
	D3DXMATRIX finalRot, rotateMatX, rotateMatY, rotateMatZ;

	float radius, rad, x, y;
	float thetaX, thetaY, thetaZ;
	int i;
	//set the center in local coords
	Circle[0].vec.x = 0.0f;
	Circle[0].vec.y = 0.0f;
	Circle[0].vec.z = 0.0f;
	Circle[0].Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	radius = 0.25f;
	rad = 0.0f;
	
	float deltaRad = PI/180.0f;

	//our current normal (0,0,-1) and normalize
	current = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize(&current, &current);

	//normal we want and normalize it
	normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&normal, &normal);

	direction_vec = normal - current;

	thetaX = 2.0f*atan2(direction_vec.z, direction_vec.y);
	thetaY = 2.0f*atan2(direction_vec.x, direction_vec.z);
	thetaZ = 2.0f*atan2(direction_vec.x, direction_vec.y);

	D3DXMatrixRotationX(&rotateMatX, thetaX);
	D3DXMatrixRotationY(&rotateMatY, thetaY);
	D3DXMatrixRotationZ(&rotateMatZ, thetaZ);

	finalRot  = rotateMatX * rotateMatY * rotateMatZ;


	for(i = 1; i < 181; i++)
	{
		y = sin(rad) * radius;
		x = cos(rad) * radius;
		
		Circle[i].vec.x = x + Circle[0].vec.x;
		Circle[i].vec.y = y + Circle[0].vec.y;
		Circle[i].vec.z = 0.0f + Circle[0].vec.z;

		D3DXVec3Transform(&result, &Circle[i].vec, &finalRot);

		Circle[i].vec.x = result.x;
		Circle[i].vec.y = result.y; 
		Circle[i].vec.z = result.z;

		if (i % 5 == 0)
			Circle[i].Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		else if (i % 2 == 0)
			Circle[i].Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		else
			Circle[i].Color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		rad += deltaRad*2.0f;
	}
	
	//Assign vertex indices for the index buffer
	unsigned int indices[540];
	int highInd = 2;

	for (i = 0; i < 540; i+=3) {

		indices[i] = 0;
		if(highInd == 181){
			indices[i + 1] = 1;
			indices[i + 2] = highInd - 1;
		}
		else{
			indices[i + 1] = highInd;
			indices[i + 2] = highInd - 1;
		}
		highInd++;
	}
	//====================================================
	createSphere(0.5f); 

	//====================================================================
    // create the vertex buffer for the Circle
    D3D11_BUFFER_DESC bd2;
    ZeroMemory(&bd2, sizeof(bd2));

    bd2.Usage = D3D11_USAGE_DYNAMIC;                  // write access access by CPU and GPU
    bd2.ByteWidth = sizeof(VERTEX) * 181;             // size is the VERTEX struct * 3
    bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;	      // use as a vertex buffer
    bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	  // allow CPU to write in buffer

    dev->CreateBuffer(&bd2, NULL, &circleBuff);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms2;
    devcon->Map(circleBuff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);		// map the buffer
    memcpy(ms2.pData, Circle, sizeof(Circle));								// copy the data
    devcon->Unmap(circleBuff, NULL);										// unmap the buffer
	//====================================================================

	//====================================================================
	// Create index buffer for the circle
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( unsigned int ) * 540;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	dev->CreateBuffer(&bufferDesc, &initData, &circleIndex);
	//=====================================================================
}


// this function loads and prepares the shaders
void InitPipeline()
{

    // load and compile the two shaders
    ID3D10Blob *VS, *PS, *PSI;
    D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, 0, 0);
    D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShaderInterp", "ps_5_0", 0, 0, 0, &PSI, 0, 0);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	dev->CreatePixelShader(PSI->GetBufferPointer(), PSI->GetBufferSize(), NULL, &pShaderInterp);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 64;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	 
    dev->CreateBuffer(&bd, NULL, &pCBuffer);
    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
}

void createSphere(float radius)
{
	VERTEX sphere[180*15+2];
	int layers = 16;
	int verticesPerCircle = 180;
	int count = 0;
	float theta, phi, deltaTheta, deltaPhi;

	deltaTheta = PI/90.0f;
	deltaPhi = PI/16.0f;
	phi = 0.0f;
	theta = 0.0f;
	//create cap point
	sphere[count].vec = D3DXVECTOR3((radius*sin(phi)*cos(theta)), 
									(radius*sin(phi)*sin(theta)),
									(radius*cos(phi)));
	sphere[count++].Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	theta += deltaTheta;
	phi += deltaPhi;
	
	for(int i = 0; i < layers-1; i++)
	{
		theta = 0.0f;

		for(int j = 0; j < verticesPerCircle; j++)
		{
			sphere[count].vec = D3DXVECTOR3((radius*sin(phi)*cos(theta)), 
											(radius*sin(phi)*sin(theta)),
											(radius*cos(phi)));

		if (i% 10 == 0)
			sphere[count++].Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		else if (i % 5 == 0)
			sphere[count++].Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		else
			sphere[count++].Color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

			theta += deltaTheta;
		}
		phi += deltaPhi;
	}

	//create other cap point
	sphere[count].vec = D3DXVECTOR3((radius*sin(phi)*cos(theta)), 
									(radius*sin(phi)*sin(theta)),
									(radius*cos(phi)));
	sphere[count++].Color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);



	//assign indices
	unsigned int indices[(540*2)+(3*(14*360))+84];
	int highInd = 2;
	int countInd;
	count = 0;
	//for one cap of sphere
	for (int i = 0; i < 540; i+=3) {
		indices[count++] = 0;
		if(highInd == 181){
			indices[count++] = 1;
			indices[count++] = highInd - 1;
		}
		else{
			indices[count++] = highInd;
			indices[count++] = highInd - 1;
		}
		highInd++;
	}
	

	int J, j;
	//body of the sphere
	for(int i = 0; i < layers - 2; i++)
	{
		for(j = 0; j < 180; j++)
		{
			J = j + (i*verticesPerCircle)+1;
			indices[count++] = J;
			indices[count++] = J + verticesPerCircle + 1;
			indices[count++] = J + verticesPerCircle;

			indices[count++] = J;
			indices[count++] = J + 1;
			indices[count++] = J + verticesPerCircle + 1;
		}
		J = j + (i*verticesPerCircle);
		indices[count++] = J; 
		indices[count++] = J + 1;
		indices[count++] = J + verticesPerCircle;

		indices[count++] = J;
		indices[count++] = J - (verticesPerCircle - 1);
		indices[count++] = J + 1;

	}

	highInd = 2699;
	//other cap of sphere
	for (int i = 0; i < 540; i+=3) {

		indices[count++] = 2701;

		if(highInd == 2701-180){
			
			indices[count++] = highInd - 1;
			indices[count++] = 2699;
		}
		else{
			indices[count++] = highInd - 1;
			indices[count++] = highInd;
			
		}
		
		highInd--;
	}



	//====================================================================
    // create the vertex buffer for the Circle
    D3D11_BUFFER_DESC bd2;
    ZeroMemory(&bd2, sizeof(bd2));

    bd2.Usage = D3D11_USAGE_DYNAMIC;                  // write access access by CPU and GPU
    bd2.ByteWidth = sizeof(VERTEX) * 2702;             // size is the VERTEX struct * 3
    bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;	      // use as a vertex buffer
    bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	  // allow CPU to write in buffer

    dev->CreateBuffer(&bd2, NULL, &sphereBuff);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms2;
    devcon->Map(sphereBuff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);		// map the buffer
    memcpy(ms2.pData, sphere, sizeof(sphere));								// copy the data
    devcon->Unmap(sphereBuff, NULL);										// unmap the buffer
	//====================================================================

	//====================================================================
	// Create index buffer for the circle
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( unsigned int ) * 16284;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	dev->CreateBuffer(&bufferDesc, &initData, &sphereIndex);
	//=====================================================================
}