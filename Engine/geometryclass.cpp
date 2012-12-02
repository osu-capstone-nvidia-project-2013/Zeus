#include "objectclass.h"
#include "geometryclass.h"
#include "main.h"
GeometryClass::GeometryClass()
{
}

void GeometryClass::Initialize() 
{

}

void GeometryClass::CopyObject( ObjectClass *objTo, ObjectClass *objFrom )
{
	objTo->iBuffer = objFrom->iBuffer;
	objTo->vBuffer = objFrom->vBuffer;
	objTo->numIndices = objFrom->numIndices;
	
	objTo->matrices = new MATRICES();
	objTo->light = new LIGHT();
	objTo->mapping = new MAPPING();

	objTo->mapping->textureflag = 0.;
	objTo->mapping->alphaflag = 0.;
	objTo->mapping->normalflag = 0.;

	objTo->alphamap		= objFrom->alphamap;
	objTo->texturemap	= objFrom->texturemap;
	objTo->normalmap	= objFrom->normalmap;

	objTo->mapping->alphaflag	= objFrom->mapping->alphaflag;
	objTo->mapping->normalflag	= objFrom->mapping->normalflag;
	objTo->mapping->particle	= objFrom->mapping->particle;
	objTo->mapping->textureflag = objFrom->mapping->textureflag;

	objTo->matrices->cameraPosition		= objFrom->matrices->cameraPosition;
	objTo->matrices->matProjection		= objFrom->matrices->matProjection;
	objTo->matrices->matView			= objFrom->matrices->matView;
	objTo->matrices->matWorld			= objFrom->matrices->matWorld;

	objTo->light		= objFrom->light;

	objTo->r			= objFrom->r;
	objTo->g			= objFrom->g;
	objTo->b			= objFrom->b;

	objTo->r0			= objFrom->r0;
	objTo->g0			= objFrom->g0;
	objTo->b0			= objFrom->b0;

	objTo->x			= objFrom->x;
	objTo->y			= objFrom->y;
	objTo->z			= objFrom->z;

	objTo->x0			= objFrom->x0;
	objTo->y0			= objFrom->y0;
	objTo->z0			= objFrom->z0;

	objTo->vx			= objFrom->vx;
	objTo->vy			= objFrom->vy;
	objTo->vz			= objFrom->vz;

	objTo->vx0			= objFrom->vx0;
	objTo->vy0			= objFrom->vy0;
	objTo->vz0			= objFrom->vz0;
}


void GeometryClass::CreateObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, vector<VERTEX> vertices, vector<int> indices) 
{
	CreateObject( dev, devcon, vertices, indices, -1, "null");
}

// Creates the ObjectClass and pushes it into the objects vector.
void GeometryClass::CreateObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, vector<VERTEX> vertices, vector<int> indices, int fileExists, string filename) 
{
    ObjectClass *obj = new ObjectClass();

	if( fileExists > -1 )
	{
		CopyObject( obj, objects[fileExists] );
	}
	else
	{

		obj->numIndices = indices.size();
		obj->matrices = new MATRICES();
		obj->light = new LIGHT();
		obj->mapping = new MAPPING();
		obj->mapping->textureflag = 0.;
		obj->mapping->alphaflag = 0.;
		obj->mapping->normalflag = 0.;
		obj->texturemap = NULL;
		obj->alphamap = NULL;
		obj->normalmap = NULL;
	
		// create the vertex buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * vertices.size();             // size is the vertices
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		dev->CreateBuffer(&bd, NULL, &obj->vBuffer);       // create the buffer

		// copy the vertices into the buffer
		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(obj->vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, &vertices[0], (sizeof(VERTEX) * vertices.size()));                 // copy the data
		devcon->Unmap(obj->vBuffer, NULL);    

		// create the index buffer
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(int) * indices.size();             // size is the indices
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as an index buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		dev->CreateBuffer(&bd, NULL, &obj->iBuffer);       // create the buffer

		// copy the vertices into the buffer
		devcon->Map(obj->iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, &indices[0], sizeof(int) * indices.size());                 // copy the data
		devcon->Unmap(obj->iBuffer, NULL);
	}

	obj->name = filename;

	sorted_objects.push_back(obj);
    objects.push_back(obj);
    return;
}

void GeometryClass::SetMatrices(MATRICES *mats, int objNum)
{
	D3DXMatrixTranspose(&objects[objNum]->matrices->matProjection, &mats->matProjection);
	D3DXMatrixTranspose(&objects[objNum]->matrices->matView, &mats->matView);
	D3DXMatrixTranspose(&objects[objNum]->matrices->matWorld, &mats->matWorld);
	objects[objNum]->matrices->cameraPosition = mats->cameraPosition;
}


void GeometryClass::SetLight(LIGHT *light, int objNum)
{
	objects[objNum]->light = light;
}

void GeometryClass::SetMapping(float texture, float alpha, float normal, float particle, float reflect, int objNum) 
{
	objects[objNum]->mapping->textureflag = texture;
	objects[objNum]->mapping->alphaflag = alpha;
	objects[objNum]->mapping->normalflag = normal;
	objects[objNum]->mapping->particle = particle;
	objects[objNum]->mapping->reflective = reflect;
}

void GeometryClass::SetAlpha(ID3D11Device *dev, LPCWSTR alphafile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           alphafile,                       // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->alphamap,      // address of the shader-resource-view
                                           NULL);                           // no multithreading
}

void GeometryClass::SetTexture(ID3D11Device *dev, LPCWSTR texturefile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           texturefile,                     // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->texturemap,    // address of the shader-resource-view
                                           NULL);                           // no multithreading
}

void GeometryClass::SetNormal(ID3D11Device *dev, LPCWSTR normalfile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           normalfile,                     // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->normalmap,    // address of the shader-resource-view
                                           NULL);                           // no multithreading
}

float GeometryClass::objdist(int index)
{
	D3DXVECTOR3 distance = D3DXVECTOR3(sorted_objects[index]->x,sorted_objects[index]->y,sorted_objects[index]->z) -
		D3DXVECTOR3(0.0f,1.0f,5.5f);

	return sqrt( (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) );
}

ObjectClass* tempObject;
int GeometryClass::partition(int left, int right, int pivotIndex)
{
    float pivot = objdist(pivotIndex);
    do
    {
        while (objdist(left) < pivot) left++;
        while (objdist(right) > pivot) right--;
        if (left < right && objdist(left) != objdist(right))
        {
			tempObject = sorted_objects[left];
			sorted_objects[left] = sorted_objects[right];
			sorted_objects[right] = tempObject;
        }
        else
        {
            return right;
        }
    }
    while (left < right);
    return right;
}

void GeometryClass::quicksort(int left, int right)
{
    if (left < right)
    {
        int pivot = (left + right) / 2; // middle
        int pivotNew = partition(left, right, pivot);
        quicksort(left, pivotNew - 1);
        quicksort(pivotNew + 1, right);
    }
}


void GeometryClass::Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, 
							IDXGISwapChain *swapchain, ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
							ID3D11Buffer *mCBuffer,
							ID3D11DepthStencilView *zbuffer, ID3D11ShaderResourceView *pTexture,
							ID3D11BlendState *pBS,ID3D11SamplerState *pSS, ID3D11RasterizerState *pRS)
{

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
    // set the various states
    devcon->RSSetState(pRS);
    devcon->PSSetSamplers(0, 1, &pSS);
    devcon->OMSetBlendState(pBS, 0, 0xffffffff);

	quicksort(0,sorted_objects.size()-2);

    // Draw it all
    for(int i = sorted_objects.size() - 1; i >= 0; i-- )
    {
		// update constant buffer
		devcon->UpdateSubresource(vCBuffer, 0, 0, sorted_objects[i]->matrices, 0, 0);	
		devcon->UpdateSubresource(pCBuffer, 0, 0, sorted_objects[i]->light, 0, 0);
		devcon->UpdateSubresource(mCBuffer, 0, 0, sorted_objects[i]->mapping, 0, 0);
		sorted_objects[i]->Render(dev, devcon, backbuffer, swapchain, pTexture);
    }
	
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


void GeometryClass::SetRenderTarget(ID3D11Device *dev, ID3D11DeviceContext *devcon, CameraClass *camera, ID3D11DepthStencilView *zbuffer)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource
	textureDesc.Width = SCREEN_WIDTH;
	textureDesc.Height = SCREEN_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	dev->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);


	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	dev->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);



	/////////////////////// Map's Shader Resource View
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	dev->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);

	////////////////////////// Draw Terrain Onto Map
	// Here we will draw our map, which is just the terrain from the mapCam's view

	// Set our maps Render Target
	devcon->OMSetRenderTargets( 1, &renderTargetViewMap, zbuffer);

	// Now clear the render target
	devcon->ClearRenderTargetView(renderTargetViewMap, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f) );

}
