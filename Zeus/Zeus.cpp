//***************************************************************************************
// Zeus.cpp
//
//
// could build a BuildCubeFaceCamera(float x, float y, float z) and mCubeMapCamera[6];
// for each object that will have dynamic cube mapping. 
//
// Might be able to reset BuildCubeFaceCamera(float x, float y, float z) each pass and set
// to a different object.
//
//***************************************************************************************

#include "d3dApp.h"
#include "PhysX.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "Sky.h"
#include "Terrain.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include <XInput.h>                       // Defines XBOX controller API
#include "FontSheet.h"
#include "SpriteBatch.h"
#include "ParticleSystem.h"
#include "xnacollision.h"
#include "FBXObj.h"

#pragma comment(lib, "XInput.lib")        // Library containing necessary 360 functions


enum RenderOptions
{
    RenderOptionsBasic = 0,
    RenderOptionsNormalMap = 1,
    RenderOptionsDisplacementMap = 2
};

struct InstancedData
{
    XMFLOAT4X4 World;
    XMFLOAT4 Color;
};

struct BoundingSphere
{
    BoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
    XMFLOAT3 Center;
    float Radius;
};


class ZeusApp : public D3DApp 
{
public:
    ZeusApp(HINSTANCE hInstance);
    ~ZeusApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene(); 

    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

private:
    void DrawScene(const Camera& camera, bool drawSphere, bool drawSkull, bool drawText);
    void BuildCubeFaceCamera(float x, float y, float z);
    void BuildDynamicCubeMapViewsSphere();
    void BuildDynamicCubeMapViewsSkull();
    void BuildDynamicCubeMapViewsMirror();

    void BuildCubeFaceShadowTransforms(float x, float y, float z);

    void DrawSceneToShadowMap();
    void BuildShadowTransform(int source, bool omni);
    void BuildShapeGeometryBuffers();
    void BuildSkullGeometryBuffers();
    void BuildScreenQuadGeometryBuffers();
    void LoadTreeBuffer();
    void LoadFBXBuffers();

    void CreateTreeMatrixes();
    void CreateFBXMatrixes();
    void BuildInstancedBuffer();


    void PxtoXMMatrix(PxTransform input, XMMATRIX* start);
    void CreatePhysXTriangleMesh(	ObjectNumbers objnum, int numVerts, std::vector<XMFLOAT3> verts,
                                    int numInds, std::vector<int> inds, float x, float y, float z, float scale);
    void CreatePhysXTriangleMeshTerrain( int numVerts, std::vector<XMFLOAT3> verts, int numInds, std::vector<int> inds);

private:

    Sky* mSky;
    Terrain mTerrain;

    PhysX* mPhysX;

    ID3D11Buffer* mInstancedBuffer;

    ID3D11Buffer* mShapesVB;
    ID3D11Buffer* mShapesIB;

    ID3D11Buffer* mSkullVB;
    ID3D11Buffer* mSkullIB;

    ID3D11Buffer* mTreeVB;
    ID3D11Buffer* mTreeIB;

    ID3D11Buffer* mFBXVB;
    ID3D11Buffer* mFBXIB;
	
    vector<FBXObj*> mFBXObjects;

    ID3D11Buffer* mSkySphereVB;
    ID3D11Buffer* mSkySphereIB;

    ID3D11Buffer* mScreenQuadVB;
    ID3D11Buffer* mScreenQuadIB;

    ID3D11ShaderResourceView* mStoneTexSRV;
    ID3D11ShaderResourceView* mBrickTexSRV;

	ID3D11ShaderResourceView* mTreeTexSRV;
	ID3D11ShaderResourceView* mClothTexSRV;

	ID3D11ShaderResourceView* mCommandoArmor;
	ID3D11ShaderResourceView* mCommandoSkin;
	ID3D11ShaderResourceView* mCommandoArmorNM;
	ID3D11ShaderResourceView* mCommandoSkinNM;

    ID3D11ShaderResourceView* mStoneNormalTexSRV;
    ID3D11ShaderResourceView* mBrickNormalTexSRV;
    ID3D11ShaderResourceView* mTreeNormalTexSRV;

    ID3D11ShaderResourceView* mFlareTexSRV;
    //ID3D11ShaderResourceView* mRainTexSRV;
    ID3D11ShaderResourceView* mRandomTexSRV;

    ParticleSystem mFire;
    //ParticleSystem mRain;

    BoundingSphere mSceneBounds;

    // Keep a system memory copy of the world matrices for culling.
    std::vector<InstancedData> mInstancedData;

    bool mFrustumCullingEnabled;
    UINT mVisibleObjectCount;

    // Bounding box of the skull.
    XNA::AxisAlignedBox mSkullBox;
    XNA::Frustum mCamFrustum;

    static const int SMapSize = 2048;
    ShadowMap* mSmap;
    ShadowMap* mSmap2;
    XMFLOAT4X4 mLightView;
    XMFLOAT4X4 mLightProj;
    XMFLOAT4X4 mShadowTransform;
    XMFLOAT4X4 mShadowTransform2;

    ShadowMap* mOmniSmaps[6];
    XMFLOAT4X4 mShadowTransformOmni[6];

    static const int CubeMapSizeSphere = 512;
    static const int CubeMapSizeSkull = 512;
    static const int CubeMapSizeMirror = 512;
    ID3D11DepthStencilView* mDynamicCubeMapDSVSphere;
    ID3D11DepthStencilView* mDynamicCubeMapDSVSkull;
    ID3D11DepthStencilView* mDynamicCubeMapDSVMirror;
    ID3D11RenderTargetView* mDynamicCubeMapRTVSphere[6];
    ID3D11RenderTargetView* mDynamicCubeMapRTVSkull[6];
    ID3D11RenderTargetView* mDynamicCubeMapRTVMirror[6];
    ID3D11ShaderResourceView* mDynamicCubeMapSRVSphere;
    ID3D11ShaderResourceView* mDynamicCubeMapSRVSkull;
    ID3D11ShaderResourceView* mDynamicCubeMapSRVMirror;
    D3D11_VIEWPORT mCubeMapViewport;

    XMFLOAT3 mOriginalLightDir[3];
    DirectionalLight mDirLights[3];
    PointLight mPointLights[1];
    DirectionalLight mNoLight[3];
    PointLight mNoPointLight[1];
    Material mGridMat;
    Material mBoxMat;
    Material mCylinderMat;
    Material mSphereMat;
    Material mSkullMat;
    Material mTreeMat;

    Material* mFBXMats;

    // Define transformations from local spaces to world space.
    XMFLOAT4X4 mSphereWorld;
    XMFLOAT4X4 mCylWorld[2];
    XMFLOAT4X4 mBoxWorld[MAX_BOXES];
    XMFLOAT4X4 mBoxScale;
    XMFLOAT4X4 mGridWorld;
    XMFLOAT4X4 mSkullWorld;
    XMFLOAT4X4 *mTreeWorld;
    XMFLOAT4X4 *mFBXWorld;

    int mBoxVertexOffset;
    int mGridVertexOffset;
    int mSphereVertexOffset;
    int mCylinderVertexOffset;

    float mLightRotationAngle;
    float mSkullRotationAngle;
    float mSkullPos;

    std::vector<XMFLOAT3> mTreepositions;
    std::vector<int> mTreeIndices;
    int mTreeVertCount;
    UINT mTreeIndexCount;

    UINT* mFBXVertexOffset;
    UINT* mFBXIndexOffset;
    UINT* mFBXIndexCount;
    UINT mFBXObjCount;
    UINT mFBXWorldCount;

    UINT mBoxIndexOffset;
    UINT mGridIndexOffset;
    UINT mSphereIndexOffset;
    UINT mCylinderIndexOffset;

    UINT mBoxIndexCount;
    UINT mGridIndexCount;
    UINT mSphereIndexCount;
    UINT mCylinderIndexCount;
    UINT mSkullIndexCount;
 
    RenderOptions mRenderOptions;

    Camera mCam;
    Camera mCubeMapCamera[6];
    XMFLOAT3 camPosition;

    /************ Toggles ***************/
    bool mWalkCamMode;
    bool directionalLight;      // Turns on or off directional lights
    bool pointLight;            // Turns on or off point lights
    bool pointLightMove;        // Turns on or off point light movement
    bool shootBox;              // Switches between firing boxes and creating at origin.
    bool showHelp;              // Toggles display of help screen
    /************************************/

    POINT mLastMousePos;

    FontSheet mFont;
    FontSheet mFontc;
    SpriteBatch mSpriteBatch;

    float accumulator;
    float stepsize;
    bool  toggleable;

    int mTreecount;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
                   PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif

    ZeusApp theApp(hInstance);
    
    if( !theApp.Init() )
        return 0;
    
    return theApp.Run();
}
 

ZeusApp::ZeusApp(HINSTANCE hInstance)
: D3DApp(hInstance), mSky(0), mWalkCamMode(true), mShapesVB(0), mShapesIB(0), mSkullVB(0), mSkullIB(0), 
  mScreenQuadVB(0), mScreenQuadIB(0), mStoneTexSRV(0), mBrickTexSRV(0), mTreeTexSRV(0), mClothTexSRV(0), mStoneNormalTexSRV(0), 
  mBrickNormalTexSRV(0), mTreeNormalTexSRV(0), mDynamicCubeMapDSVSphere(0), mDynamicCubeMapSRVSphere(0), mDynamicCubeMapDSVSkull(0), 
  mDynamicCubeMapSRVSkull(0), mDynamicCubeMapDSVMirror(0), mDynamicCubeMapSRVMirror(0), mSkullIndexCount(0), mInstancedBuffer(0),
  mRenderOptions(RenderOptionsNormalMap), mSmap(0), mSmap2(0), mPhysX(0), mLightRotationAngle(0.0f), mFrustumCullingEnabled(true), mVisibleObjectCount(0)
{
    mMainWndCaption = L"Zeus";
    
    mSkullRotationAngle = 0;
    mSkullPos = 0;
    mLastMousePos.x = 0;
    mLastMousePos.y = 0;

    mFBXObjCount = 0;
    mFBXIndexOffset = 0;
    mFBXIndexCount = 0;
    mFBXVertexOffset = 0;
    mFBXWorld = 0;
    mFBXWorldCount = 0;


    float accumulator = 0.0f;
    float stepsize = 100000.0f;
    bool  toggleable = true;

    extern float fps;
    mCam.SetPosition(0.0f, 0.5f, -14.0f);
    camPosition = mCam.GetPosition(); 

    for(int i = 0; i < 6; ++i)
    {
        mDynamicCubeMapRTVSphere[i] = 0;
        mDynamicCubeMapRTVSkull[i] = 0;
        mDynamicCubeMapRTVMirror[i] = 0;

        mOmniSmaps[i] = 0;
    }

    int source = 0;
    bool omni = false;

    // Estimate the scene bounding sphere manually since we know how the scene was constructed.
    // The grid is the "widest object" with a width of 20 and depth of 30.0f, and centered at
    // the world space origin.  In general, you need to loop over every world space vertex
    // position and compute the bounding sphere.
    mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mSceneBounds.Radius = sqrtf(2049.*1./2.);

    ////////////////////////////
    //    Objects in scene    //
    ////////////////////////////

    XMMATRIX I = XMMatrixIdentity();
    
    // Floor plane
    XMStoreFloat4x4(&mGridWorld, XMMatrixTranslation(0.0f, 1.0f, 0.0f));

    // Box
    XMMATRIX boxScale = XMMatrixScaling(2.5f, 2.5f, 2.5f);
    XMStoreFloat4x4(&mBoxScale, boxScale);
    XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
    XMMATRIX final = XMMatrixMultiply(boxScale, boxOffset);
    for(int i = 0; i < MAX_BOXES; i++)
    {
        XMStoreFloat4x4(&mBoxWorld[i], final);
    }

    // Cylinders and sphere
    XMStoreFloat4x4(&mCylWorld[0], XMMatrixTranslation(5.0f, 1.5f, 10.0f));
    XMStoreFloat4x4(&mSphereWorld, XMMatrixTranslation(5.0f, 4.0f, 10.0f));
    XMStoreFloat4x4(&mCylWorld[1], XMMatrixTranslation(-5.0f, 1.5f, 10.0f));
    

    ////////////////////////////
    //    Lights/Materials    //
    ////////////////////////////
    mPointLights[0].Ambient = XMFLOAT4(2.8f, 1.0f, 0.2f, 1.0f);
    mPointLights[0].Diffuse = XMFLOAT4(2.7f, 1.0f, 1.6f, 1.0f);
    mPointLights[0].Specular = XMFLOAT4(1.8f, 1.8f, 1.7f, 1.0f);
    mPointLights[0].Position = XMFLOAT3(-5.0f, 4.5f, 10.0f);
    mPointLights[0].Range = 40.3f;
    mPointLights[0].Att = XMFLOAT3(1.0f, .05f, .0075f);

    mDirLights[0].Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[0].Diffuse  = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);
    mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
    mDirLights[0].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    // Shadow acne gets worse as we increase the slope of the polygon (from the perspective of the light).
    /*mDirLights[0].Direction = XMFLOAT3(5.0f/sqrtf(50.0f), -5.0f/sqrtf(50.0f), 0.0f);
    mDirLights[0].Direction = XMFLOAT3(10.0f/sqrtf(125.0f), -5.0f/sqrtf(125.0f), 0.0f);
    mDirLights[0].Direction = XMFLOAT3(10.0f/sqrtf(116.0f), -4.0f/sqrtf(116.0f), 0.0f);
    mDirLights[0].Direction = XMFLOAT3(10.0f/sqrtf(109.0f), -3.0f/sqrtf(109.0f), 0.0f);*/

    mDirLights[1].Ambient  = XMFLOAT4(0.99f, 0.99f, 0.99f, 0.002f);
    mDirLights[1].Diffuse  = XMFLOAT4(0.40f, 0.40f, 0.40f, 1.0f);
    mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    for(int i = 0; i < 3; i++)
    {
        mNoLight[i].Ambient = XMFLOAT4(0.06666666667f, 0.06666666667f, 0.06666666667f, 1.0f);
        mNoLight[i].Diffuse  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        mNoLight[i].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    mNoPointLight[0].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mNoPointLight[0].Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mNoPointLight[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    mOriginalLightDir[0] = mDirLights[0].Direction;
    mOriginalLightDir[1] = mDirLights[1].Direction;
    mOriginalLightDir[2] = mDirLights[2].Direction;

    mGridMat.Ambient  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    mGridMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mGridMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
    mGridMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    mCylinderMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mCylinderMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mCylinderMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
    mCylinderMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    mSphereMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.525f, 1.0f);
    mSphereMat.Diffuse  = XMFLOAT4(0.7f, 0.7f, 0.725f, 1.0f);
    mSphereMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
    mSphereMat.Reflect  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

    mBoxMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mBoxMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mBoxMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
    mBoxMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    mSkullMat.Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mSkullMat.Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mSkullMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
    mSkullMat.Reflect  = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

    mTreeMat.Ambient  = XMFLOAT4(0.99f, 0.99f, 0.99f, 1.0f);
    mTreeMat.Diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mTreeMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.8f);
    mTreeMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

}


ZeusApp::~ZeusApp()
{
    md3dImmediateContext->ClearState();

    SafeDelete(mSky);
    SafeDelete(mSmap);
    SafeDelete(mSmap2);
    SafeDelete(mPhysX);
    ReleaseCOM(mInstancedBuffer);
    ReleaseCOM(mShapesVB);
    ReleaseCOM(mShapesIB);
    ReleaseCOM(mSkullVB);
    ReleaseCOM(mSkullIB);
    ReleaseCOM(mTreeVB);
    ReleaseCOM(mTreeIB);
    ReleaseCOM(mFBXVB);
    ReleaseCOM(mFBXIB);
    ReleaseCOM(mScreenQuadVB);
    ReleaseCOM(mScreenQuadIB);
    ReleaseCOM(mStoneTexSRV);
    ReleaseCOM(mBrickTexSRV);
    ReleaseCOM(mTreeTexSRV);
    ReleaseCOM(mClothTexSRV);
    ReleaseCOM(mStoneNormalTexSRV);
    ReleaseCOM(mBrickNormalTexSRV);
    ReleaseCOM(mTreeNormalTexSRV);
    ReleaseCOM(mRandomTexSRV);
    ReleaseCOM(mFlareTexSRV);
    //ReleaseCOM(mRainTexSRV);
    ReleaseCOM(mDynamicCubeMapDSVSphere);
    ReleaseCOM(mDynamicCubeMapSRVSphere);
    ReleaseCOM(mDynamicCubeMapDSVSkull);
    ReleaseCOM(mDynamicCubeMapSRVSkull);
    ReleaseCOM(mDynamicCubeMapDSVMirror);
    ReleaseCOM(mDynamicCubeMapSRVMirror);
    for(int i = 0; i < 6; ++i){
        ReleaseCOM(mDynamicCubeMapRTVSphere[i]);
        ReleaseCOM(mDynamicCubeMapRTVSkull[i]);
        ReleaseCOM(mDynamicCubeMapRTVMirror[i]);
        SafeDelete(mOmniSmaps[i]);
    }

    Effects::DestroyAll();
    InputLayouts::DestroyAll(); 
    RenderStates::DestroyAll();
}


bool ZeusApp::Init()
{
    mPhysX->Init();

    if(!D3DApp::Init())
        return false;

    // Must init Effects first since InputLayouts depend on shader signatures.
    Effects::InitAll(md3dDevice);
    InputLayouts::InitAll(md3dDevice);
    RenderStates::InitAll(md3dDevice);

    mSky  = new Sky(md3dDevice, L"Textures/mountains1024.dds", 5000.0f);

    Terrain::InitInfo tii;
    tii.HeightMapFilename = L"Textures/terrain3.raw";
    tii.LayerMapFilename0 = L"Textures/grass.dds";
    tii.LayerMapFilename1 = L"Textures/darkdirt.dds";
    tii.LayerMapFilename2 = L"Textures/stone.dds";
    tii.LayerMapFilename3 = L"Textures/lightdirt.dds";
    tii.LayerMapFilename4 = L"Textures/snow.dds";
    tii.BlendMapFilename = L"Textures/blend.dds";
    tii.HeightScale = 100.0f;
    tii.HeightmapWidth = 2049;
    tii.HeightmapHeight = 2049;
    tii.CellSpacing = 1.0f;

    mTerrain.Init(md3dDevice, md3dImmediateContext, tii);

    terrainMeshInfo terMesh = mTerrain.GetMeshInfo();
    //CreatePhysXTriangleMeshTerrain(terMesh.vertcount,terMesh.positions,terMesh.indcount,terMesh.indices);

    mSmap = new ShadowMap(md3dDevice, SMapSize, SMapSize);
    mSmap2 = new ShadowMap(md3dDevice, SMapSize, SMapSize);
    for(int i = 0; i < 6; i++)
    {
        mOmniSmaps[i] = new ShadowMap(md3dDevice, SMapSize, SMapSize);
    }

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/floor.dds", 0, 0, &mStoneTexSRV, 0 ));

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/bricks.dds", 0, 0, &mBrickTexSRV, 0 ));

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/floor.dds", 0, 0, &mTreeTexSRV, 0 ));

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/floor.dds", 0, 0, &mClothTexSRV, 0 ));

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/floor_nmap.dds", 0, 0, &mStoneNormalTexSRV, 0 ));

    HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/bricks_nmap.dds", 0, 0, &mBrickNormalTexSRV, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/CommandoArmor_DM.dds", 0, 0, &mCommandoArmor, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/Commando_DM.dds", 0, 0, &mCommandoSkin, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/CommandoArmor_NM.dds", 0, 0, &mCommandoArmorNM, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/Commando_NM.dds", 0, 0, &mCommandoSkinNM, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
        L"Textures/floor_nmap.dds", 0, 0, &mTreeNormalTexSRV, 0 ));

    HR(mFont.Initialize(md3dDevice, L"Perpetua", 36.0f, FontSheet::FontStyleRegular, true));
    HR(mFontc.Initialize(md3dDevice, L"Perpetua", 48.0f, FontSheet::FontStyleRegular, true));

    HR(mSpriteBatch.Initialize(md3dDevice));

    mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(md3dDevice);

    std::vector<std::wstring> flares;
    flares.push_back(L"Textures/flare0.dds");
    mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, flares);

    mFire.Init(md3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500); 
    mFire.SetEmitPos(XMFLOAT3(-5.0f, 3.75f, 10.0f));


    /*std::vector<std::wstring> raindrops;
    raindrops.push_back(L"Textures/raindrop.dds");
    mRainTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, raindrops);

    mRain.Init(md3dDevice, Effects::RainFX, mRainTexSRV, mRandomTexSRV, 10000);*/

    directionalLight = true;
    pointLight = true;
    pointLightMove = false;
    shootBox = true;
    showHelp = false;

    BuildDynamicCubeMapViewsSphere();
    BuildDynamicCubeMapViewsSkull();
    //BuildDynamicCubeMapViewsMirror();

    BuildShapeGeometryBuffers();
    BuildSkullGeometryBuffers();
    BuildScreenQuadGeometryBuffers();
    LoadTreeBuffer();
    
    LoadFBXBuffers();
    

    BuildInstancedBuffer();

    // Trees
    CreateTreeMatrixes();

    //FBX objects
    CreateFBXMatrixes();

    return true;
}


void ZeusApp::OnResize()
{
    D3DApp::OnResize();
    mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

    // Build the frustum from the projection matrix in view space.
    ComputeFrustumFromProjection(&mCamFrustum, &mCam.Proj());
}


void ZeusApp::UpdateScene(float dt)
{
    // Tell physx to get to work
    bool fetch = mPhysX->advance(dt);

    float shootspeed = 100.0;

    //////////////////////////////////
    //    XINPUT Camera Controls    //
    //////////////////////////////////

    DWORD dwResult;
    XINPUT_STATE state;
 
    ZeroMemory( &state, sizeof(XINPUT_STATE) );
 
    dwResult = XInputGetState( 0, &state );
 
    if( dwResult == ERROR_SUCCESS ){ // Controller is connected.
        float speed = 1.0f;
        if( state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB )
            speed = 2.0f;
        ShowCursor(false);
        // Check to make sure we are not moving during the dead zone
        if((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
           (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)){    
                state.Gamepad.sThumbLX = 0;
                state.Gamepad.sThumbLY = 0;
        }
 
        if((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
           (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)){
                state.Gamepad.sThumbRX = 0;
                state.Gamepad.sThumbRY = 0;
        }

        float leftThumbY = state.Gamepad.sThumbLY;
        float leftThumbX = state.Gamepad.sThumbLX;
        float rightThumbY = state.Gamepad.sThumbRY;
        float rightThumbX = state.Gamepad.sThumbRX;

        // Aiming with left trigger
        if(state.Gamepad.bLeftTrigger && state.Gamepad.bRightTrigger < 256){ // 256 disables the right trigger
            
            //TODO: 
            //
            //pause for weapon animation

            mCam.SetLens(0.12f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f); //setlens(zoom factor, , ,)
            mCam.Walk((leftThumbY / 30000.0f) * dt * speed);
            mCam.Strafe((leftThumbX / 30000.0f) * dt * speed);
            mCam.Pitch((-rightThumbY / 102000.0f) * dt);
            mCam.RotateY((rightThumbX / 105000.0f) * dt);
        }
        else{
            mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
            mCam.Walk((leftThumbY / 3000.0f) * dt * speed);
            mCam.Strafe((leftThumbX / 3000.0f) * dt * speed);
            mCam.Pitch((-rightThumbY / 12000.0f) * dt);
            mCam.RotateY((rightThumbX / 8500.0f) * dt);
        }

        // Shoot block with right trigger     
        if( state.Gamepad.bRightTrigger && state.Gamepad.bLeftTrigger < 256 )
        {
            shootspeed = (state.Gamepad.bRightTrigger / 255) * 40.0f;
            if(shootBox)
            {
                mPhysX->CreateBox( mCam.GetPosition().x, mCam.GetPosition().y, mCam.GetPosition().z,
                                   mCam.GetLook().x, mCam.GetLook().y, mCam.GetLook().z, shootspeed);
            }
            else
            {
                mPhysX->CreateBox( 0., 10.0f, 0., 0., 0., 0., 0.);
            }
        }

    }
    else{ // Controller is disconnected, oh balls
        float speed = 10.0f;
        ShowCursor(true);
        if( GetAsyncKeyState(0x10) & 0x8000 )
            speed = 100.0f;

        if( GetAsyncKeyState('W') & 0x8000 )
            mCam.Walk(speed*dt);

        if( GetAsyncKeyState('S') & 0x8000 )
            mCam.Walk(-speed*dt);

        if( GetAsyncKeyState('A') & 0x8000 )
            mCam.Strafe(-speed*dt);

        if( GetAsyncKeyState('D') & 0x8000 )
            mCam.Strafe(speed*dt);

        //if( GetAsyncKeyState('P') & 0x8000 )
        //   mCam.SetLens(0.01f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f); //setlens(zoom factor, , ,)
    }
        
    // Walk/fly mode
    if( GetAsyncKeyState('8') & 0x8000 )
        mWalkCamMode = true;
    if( GetAsyncKeyState('9') & 0x8000 )
        mWalkCamMode = false;
    if( GetAsyncKeyState('F') & 0x8000 )
        mFrustumCullingEnabled = true;
    if( GetAsyncKeyState('G') & 0x8000 )
        mFrustumCullingEnabled = false;
    if( GetAsyncKeyState('H') & 0x8000 )
    {
        if(showHelp)
        {
            showHelp = false;
        }
        else
        {
            showHelp = true;
        }
    }

    // Set height, crouch, jump
    if( mWalkCamMode )
    {
        //run a check for what material we're on, for sound and height
        XMFLOAT3 camPos = mCam.GetPosition();
        float y = mTerrain.GetHeight(camPos.x, camPos.z); // Clamp camera to terrain surface in walk mode.
        mCam.SetPosition(camPos.x, y + 5.0f, camPos.z);

        //TODO: 
        // 
        // animations of crouch and jump
        
        // Crouch
        if(state.Gamepad.wButtons & XINPUT_GAMEPAD_B){
            mCam.SetPosition(camPos.x, y + 3.0f, camPos.z);
        }

        // Jump
        if(state.Gamepad.wButtons & XINPUT_GAMEPAD_A){
            mCam.SetPosition(camPos.x, y + 5.0f*2, camPos.z);
        }
    }

    // Switch the rendering effect based on key presses.
    if( GetAsyncKeyState('2') & 0x8000 )
        mRenderOptions = RenderOptionsBasic; 

    if( GetAsyncKeyState('3') & 0x8000 )
        mRenderOptions = RenderOptionsNormalMap; 

    if(!toggleable)
    {
        accumulator += dt / (2.0f);
        if(accumulator >= stepsize)
        {
            accumulator = 0.0f;
            toggleable = true;
        }
    }


    // Turn on/off moving point light
    if( (GetAsyncKeyState('M') & 0x8000) && toggleable )
    {
        toggleable = false;
        if(pointLightMove)
        {
            pointLightMove = false;
        }
        else
        {
            pointLightMove = true;
        }
    }

    // Turn on/off point light
    if( (GetAsyncKeyState('P') & 0x8000) && toggleable )
    {
        toggleable = false;
        if(pointLight)
        {
            pointLight = false;
            for(int i = 0; i < 6; i++)
                mOmniSmaps[i]->SetNullRenderTarget(md3dImmediateContext); // Clears the shadows
        }
        else
        {
            pointLight = true;
        }
    }

    // Turn on/off directional light
    if( (GetAsyncKeyState('I') & 0x8000) && toggleable )
    {
        toggleable = false;
        if(directionalLight)
        {
            directionalLight = false;
            mSmap->SetNullRenderTarget(md3dImmediateContext); // Clears the shadows for one
            mSmap2->SetNullRenderTarget(md3dImmediateContext); // Clears the shadows for two
        }
        else
        {
            directionalLight = true;
        }
    }

    // Turn on/off shooting blocks
    if( (GetAsyncKeyState('N') & 0x8000) && toggleable )
    {
        toggleable = false;
        if(shootBox)
        {
            shootBox = false;
        }
        else
        {
            shootBox = true;
        }
    }

  // (state.Gamepad.bRightTrigger < 256) 

    // Shoot block with 'B'
    if( (GetAsyncKeyState('B') & 0x8000) )
    {
        if(shootBox)
        {
            mPhysX->CreateBox( mCam.GetPosition().x, mCam.GetPosition().y, mCam.GetPosition().z,
                               mCam.GetLook().x, mCam.GetLook().y, mCam.GetLook().z, shootspeed);
        }
        else
        {
            mPhysX->CreateBox( 0., 10.0f, 0., 0., 0., 0., 0.);
        }
    }
    


    /////////////////////////////////////
    //    Animated objects in scene    //
    /////////////////////////////////////

    // Box
    for(int i = 0; i < mPhysX->GetNumBoxes(); i++)
    {
        PxTransform pt = mPhysX->GetBoxWorld(i);
        XMMATRIX world = /*XMLoadFloat4x4(&mBoxScale) **/ XMLoadFloat4x4(&mBoxWorld[i]) ;
        PxtoXMMatrix(pt, &world);
        XMStoreFloat4x4(&mBoxWorld[i], world);
    }

    // Cow 
    XMMATRIX SkullScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
    XMMATRIX SkullOffset = XMMatrixTranslation(7.0f, 3.0f, 6.0f);
    XMMATRIX SkullLocalRotate = XMMatrixRotationY(1.5f);
    XMMATRIX SkullGlobalRotate = XMMatrixRotationY(0.5f*mTimer.TotalTime());
    XMStoreFloat4x4(&mSkullWorld, SkullLocalRotate*SkullScale*SkullOffset);

    // Animate the lights (and hence shadows).
    mLightRotationAngle += 0.1f*dt;
    XMMATRIX SkullLocalRotate2 = XMMatrixRotationY( fmodf( mLightRotationAngle * 3, (XM_PI * 2) ) );
    mLightRotationAngle = fmodf(mLightRotationAngle, (XM_PI * 2) );
    XMMATRIX R = XMMatrixRotationY(mLightRotationAngle);
    for(int i = 0; i < 3; ++i)
    {
        XMVECTOR lightDir = XMLoadFloat3(&mOriginalLightDir[i]);
        lightDir = XMVector3TransformNormal(lightDir, R);
        //XMStoreFloat3(&mDirLights[i].Direction, lightDir);
    }

    XMVECTOR lightDir = XMLoadFloat3(&mOriginalLightDir[1]);
    lightDir = XMVector3TransformNormal(lightDir, SkullLocalRotate2);
    //XMStoreFloat3(&mDirLights[1].Direction, lightDir);
    if(pointLightMove)
    {
        lightDir = XMLoadFloat3(&XMFLOAT3(15.0f, 3.0f, 15.0f));
        lightDir = XMVector3TransformNormal(lightDir, XMMatrixRotationY( fmodf( mLightRotationAngle * 15, (XM_PI * 2) ) ));
        XMStoreFloat3(&mPointLights[0].Position, lightDir);
    }

    //mRain.SetEmitPos(mCam.GetPosition());

    //
    // Reset particle systems.
    //
    if(GetAsyncKeyState('R') & 0x8000)
    {
        mFire.Reset();
        //mRain.Reset();
    }
 
    mFire.Update(dt/10., mTimer.TotalTime());
    //mRain.Update(dt/10, mTimer.TotalTime());

    camPosition = mCam.GetPosition();

    //
    // Perform frustum culling.
    //
    mCam.UpdateViewMatrix();
    mVisibleObjectCount = 0;

    if(mFrustumCullingEnabled)
    {
        XMVECTOR detView = XMMatrixDeterminant(mCam.View());
        XMMATRIX invView = XMMatrixInverse(&detView, mCam.View());
    
        D3D11_MAPPED_SUBRESOURCE mappedData; 
        md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

        InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

        for(UINT i = 0; i < mInstancedData.size(); ++i)
        {
            XMMATRIX W = XMLoadFloat4x4(&mInstancedData[i].World);
            XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

            // View space to the object's local space.
            XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
        
            // Decompose the matrix into its individual parts.
            XMVECTOR scale;
            XMVECTOR rotQuat;
            XMVECTOR translation;
            XMMatrixDecompose(&scale, &rotQuat, &translation, toLocal);

            // Transform the camera frustum from view space to the object's local space.
            XNA::Frustum localspaceFrustum;
            XNA::TransformFrustum(&localspaceFrustum, &mCamFrustum, XMVectorGetX(scale), rotQuat, translation);

            // Perform the box/frustum intersection test in local space.
            if(XNA::IntersectAxisAlignedBoxFrustum(&mSkullBox, &localspaceFrustum) != 0)
            {
                // Write the instance data to dynamic VB of the visible objects.
                dataView[mVisibleObjectCount++] = mInstancedData[i];
            }
        }

        md3dImmediateContext->Unmap(mInstancedBuffer, 0);
    }
    else // No culling enabled, draw all objects.
    {
        D3D11_MAPPED_SUBRESOURCE mappedData; 
        md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

        InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

        for(UINT i = 0; i < mInstancedData.size(); ++i)
        {
            dataView[mVisibleObjectCount++] = mInstancedData[i];
        }

        md3dImmediateContext->Unmap(mInstancedBuffer, 0);
    }

    std::wostringstream outs;   
    outs.precision(6);
    outs << L"Zeus - Frustum Culling Test" << 
        L"    " << mVisibleObjectCount << 
        L" objects visible out of " << mInstancedData.size();
    mMainWndCaption = outs.str();

    // If things are ready to get, fetch 'em
    if(fetch)
        mPhysX->fetch();
}


void ZeusApp::DrawScene()
{
    // Draw directional shadow maps
    
    if(directionalLight) {
        BuildShadowTransform(0, false);
        mSmap->BindDsvAndSetNullRenderTarget(md3dImmediateContext);
        DrawSceneToShadowMap();
    
        BuildShadowTransform(1, false);
        mSmap2->BindDsvAndSetNullRenderTarget(md3dImmediateContext);
        DrawSceneToShadowMap();
    }
    
    // Draw omni directional shadow maps
    if(pointLight)
    {
        BuildCubeFaceShadowTransforms(mPointLights[0].Position.x, mPointLights[0].Position.y, mPointLights[0].Position.z); // point light position
    }

    md3dImmediateContext->RSSetState(0);
    ID3D11RenderTargetView* renderTargets[1];

    // Generate the dynamic cube maps
    md3dImmediateContext->RSSetViewports(1, &mCubeMapViewport);
    
    if(!((camPosition.x > 15.0f || camPosition.x < -25.0) ||
         (camPosition.y > 25.0f || camPosition.y < -25.0) ||
         (camPosition.z > 15.0f || camPosition.z < -25.0)) ){
    
        // Sphere with dynamic cube mapping
        BuildCubeFaceCamera(5.0f, 4.0f, 10.0f); // Sphere position
    
        for(int i = 0; i < 6; ++i) // for mirror, just do (int i = 0; i < 1; ++i) for 1 camera mapped to mirror surface
        {
            // Clear cube map face and depth buffer.
            md3dImmediateContext->ClearRenderTargetView(mDynamicCubeMapRTVSphere[i], reinterpret_cast<const float*>(&Colors::Silver));
            md3dImmediateContext->ClearDepthStencilView(mDynamicCubeMapDSVSphere, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

            // Bind cube map face as render target.
            renderTargets[0] = mDynamicCubeMapRTVSphere[i];
            md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDynamicCubeMapDSVSphere);

            // Draw the scene with the exception of the center sphere to this cube map face
            DrawScene(mCubeMapCamera[i], false, false, false);
			md3dImmediateContext->ClearRenderTargetView(mDynamicCubeMapRTVSphere[i], reinterpret_cast<const float*>(&Colors::Silver));
            md3dImmediateContext->ClearDepthStencilView(mDynamicCubeMapDSVSphere, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
			DrawScene(mCubeMapCamera[i], false, false, false);
        }

        // Have hardware generate lower mipmap levels of cube map
        md3dImmediateContext->GenerateMips(mDynamicCubeMapSRVSphere);
    }


    //// Skull with dynamic cube mapping
    //if(!((camPosition.x > 25.0f || camPosition.x < -25.0) ||
    //     (camPosition.y > 25.0f || camPosition.y < -25.0) ||
    //     (camPosition.z > 25.0f || camPosition.z < -25.0)) ){
    //
    //    BuildCubeFaceCamera(7.0f, 3.0f, 6.0f); // Skull position
   
    //    for(int i = 0; i < 6; ++i) // for mirror, just do (int i = 0; i < 1; ++i) for 1 camera mapped to mirror surface
    //    {
    //         // Clear cube map face and depth buffer.
    //        md3dImmediateContext->ClearRenderTargetView(mDynamicCubeMapRTVSkull[i], reinterpret_cast<const float*>(&Colors::Silver));
    //        md3dImmediateContext->ClearDepthStencilView(mDynamicCubeMapDSVSkull, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    //        // Bind cube map face as render target.
    //        renderTargets[0] = mDynamicCubeMapRTVSkull[i];
    //        md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDynamicCubeMapDSVSkull);
    //
    //        // Draw the scene with the exception of the center sphere to this cube map face.
    //        DrawScene(mCubeMapCamera[i], true, false, false);	
    //    }

    //    // Have hardware generate lower mipmap levels of cube map.
    //    md3dImmediateContext->GenerateMips(mDynamicCubeMapSRVSkull);
    //}


    /*// Mirror with dynamic cube mapping
    for(int i = 0; i < 1; ++i) // for mirror, just do (int i = 0; i < 1; ++i) for 1 camera mapped to mirror surface
    {
        BuildCubeFaceCamera(0.0f, 2.0f, 0.0f); // Skull position

        // Clear cube map face and depth buffer.
        md3dImmediateContext->ClearRenderTargetView(mDynamicCubeMapRTVMirror[i], reinterpret_cast<const float*>(&Colors::Silver));
        md3dImmediateContext->ClearDepthStencilView(mDynamicCubeMapDSVMirror, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Bind cube map face as render target.
        renderTargets[0] = mDynamicCubeMapRTVMirror[i];
        md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDynamicCubeMapDSVMirror);

        // Draw the scene with the exception of the center sphere to this cube map face.
        DrawScene(mCubeMapCamera[i], true, false);
    }

    // Have hardware generate lower mipmap levels of cube map.
    md3dImmediateContext->GenerateMips(mDynamicCubeMapSRVMirror);*/

    // Restore the back and depth buffer to the OM stage.
    md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
    renderTargets[0] = mRenderTargetView;
    md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
    
    md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
   
    DrawScene(mCam, true, true, false); // Switch to false if turning off dynamic cube mapping
    //md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
    DrawScene(mCam, true, true, false);
    float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f}; 

    // Draw particle systems last so it is blended with scene.
    mFire.SetEyePos(mCam.GetPosition());
    mFire.Draw(md3dImmediateContext, mCam);
    md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default

    //mRain.SetEyePos(mCam.GetPosition());
    //mRain.Draw(md3dImmediateContext, mCam);

    // restore default states.
    md3dImmediateContext->RSSetState(0);
    md3dImmediateContext->OMSetDepthStencilState(0, 0);
    md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); 


    ////////////////////////////////////////////////////////////////////////////////////////////
    // Screen Text 
    md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);

    std::wstring text = L"FPS: " + mfps_string;
    std::wstring hair = L"+";


    // Convert the pos to a w string
    std::wstring xs,ys,zs;
        std::wstringstream posx,posy,posz;
        posx << mCam.GetPosition().x;
        xs = posx.str();
        posy << mTerrain.GetHeight(mCam.GetPosition().x, mCam.GetPosition().z);
        ys = posy.str();
        posz << mCam.GetPosition().z;
        zs = posz.str();
    std::wstring pos = xs + L", " + ys + L", " + zs;
    

        // Calculate the text width.
        int textWidth = 0;
        for(UINT i = 0; i < text.size(); ++i)
        {
            WCHAR character = text[i];
            if(character == ' ') 
            {
                textWidth += mFont.GetSpaceWidth();
            }
            else{
                const CD3D11_RECT& r = mFont.GetCharRect(text[i]);
                textWidth += (r.right - r.left + 1);
            }
        }
        
        // Calculate the hair width.
        int hairWidth = 0;
        for(UINT i = 0; i < hair.size(); ++i)
        {
            WCHAR character = hair[i];
            if(character == ' ') 
            {
                hairWidth += mFont.GetSpaceWidth();
            }
            else{
                const CD3D11_RECT& r = mFont.GetCharRect(hair[i]);
                hairWidth += (r.right - r.left);
            }
        }

        // Calculate the pos width
        int posWidth = 0;
        for(UINT i = 0; i < pos.size(); ++i)
        {
            WCHAR character = pos[i];
            if(character == ' ') 
            {
                hairWidth += mFont.GetSpaceWidth();
            }
            else{
                const CD3D11_RECT& r = mFont.GetCharRect(pos[i]);
                posWidth += (r.right - r.left + 1);
            }
        }

        // Put the text in the top right of the screen.
        POINT textPos;
        textPos.x = (mClientWidth - textWidth) - 2.0;
        textPos.y = 1.0;
    
        // Center the text in the screen.
        POINT hairPos;
        hairPos.x = (mClientWidth - hairWidth) / 2;
        hairPos.y = (mClientHeight - mFont.GetCharHeight()) / 2 ;

        // Put the position in the top left of the screen
        POINT posPos;
        posPos.x = 2.0;
        posPos.y = 1.0;

        mSpriteBatch.DrawString(md3dImmediateContext, mFont, text, textPos, XMCOLOR(0xffffffff));
        mSpriteBatch.DrawString(md3dImmediateContext, mFont, hair, hairPos, XMCOLOR(0xff2C2CEE));
        mSpriteBatch.DrawString(md3dImmediateContext, mFont, pos, posPos, XMCOLOR(0xffffffff));
        // End of text draw	
        ///////////////////////////////////////////////////////////////////////////////////////////////

    HR(mSwapChain->Present(0, 0));
}
    

void ZeusApp::DrawScene(const Camera& camera, bool drawSphere, bool drawSkull, bool drawText)
{
    if( GetAsyncKeyState('1') & 0x8000 )
        md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

    XMMATRIX shadowTransform = XMLoadFloat4x4(&mShadowTransform);
    XMMATRIX shadowTransform2 = XMLoadFloat4x4(&mShadowTransform2);

    
    if(pointLight)
    {
        // Draw Terrain
        Effects::TerrainFX->SetPointLights(mPointLights);
        Effects::TerrainFX->SetOmniShadowMaps(mOmniSmaps[0]->DepthMapSRV(), mOmniSmaps[1]->DepthMapSRV(),
                mOmniSmaps[2]->DepthMapSRV(), mOmniSmaps[3]->DepthMapSRV(), mOmniSmaps[4]->DepthMapSRV(), mOmniSmaps[5]->DepthMapSRV());
        Effects::TerrainFX->SetShadowTransforms(XMLoadFloat4x4(&mShadowTransformOmni[0]),XMLoadFloat4x4(&mShadowTransformOmni[1]),
                                                XMLoadFloat4x4(&mShadowTransformOmni[2]),XMLoadFloat4x4(&mShadowTransformOmni[3]),
                                                XMLoadFloat4x4(&mShadowTransformOmni[4]),XMLoadFloat4x4(&mShadowTransformOmni[5]));

        // Set per frame constants.
        Effects::BasicFX->SetPointLights(mPointLights);
        /*Effects::BasicFX->SetOmniShadowMaps(mOmniSmaps[0]->DepthMapSRV(), mOmniSmaps[1]->DepthMapSRV(),
            mOmniSmaps[2]->DepthMapSRV(), mOmniSmaps[3]->DepthMapSRV(), mOmniSmaps[4]->DepthMapSRV(), mOmniSmaps[5]->DepthMapSRV());*/
        
        Effects::NormalMapFX->SetPointLights(mPointLights);
        /*Effects::NormalMapFX->SetOmniShadowMaps(mOmniSmaps[0]->DepthMapSRV(), mOmniSmaps[1]->DepthMapSRV(),
            mOmniSmaps[2]->DepthMapSRV(), mOmniSmaps[3]->DepthMapSRV(), mOmniSmaps[4]->DepthMapSRV(), mOmniSmaps[5]->DepthMapSRV());*/

        Effects::DisplacementMapFX->SetPointLights(mPointLights);
        /*Effects::DisplacementMapFX->SetOmniShadowMaps(mOmniSmaps[0]->DepthMapSRV(), mOmniSmaps[1]->DepthMapSRV(),
            mOmniSmaps[2]->DepthMapSRV(), mOmniSmaps[3]->DepthMapSRV(), mOmniSmaps[4]->DepthMapSRV(), mOmniSmaps[5]->DepthMapSRV());*/
    }
    else
    {
        Effects::TerrainFX->SetPointLights(mNoPointLight);

        Effects::BasicFX->SetPointLights(mNoPointLight);
        Effects::NormalMapFX->SetPointLights(mNoPointLight);
        Effects::DisplacementMapFX->SetPointLights(mNoPointLight);
    }

    if(directionalLight)
    {
        // Draw Terrain
        Effects::TerrainFX->SetShadowMap(mSmap->DepthMapSRV());
        Effects::TerrainFX->SetShadowMap2(mSmap2->DepthMapSRV());
        Effects::TerrainFX->SetShadowTransform(shadowTransform);
        Effects::TerrainFX->SetShadowTransform2(shadowTransform2);
    
        mTerrain.Draw(md3dImmediateContext, camera, mDirLights);

        // Set per frame constants.
        Effects::BasicFX->SetDirLights(mDirLights);
        Effects::BasicFX->SetEyePosW(mCam.GetPosition());
        //Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());
        Effects::BasicFX->SetShadowMap(mSmap->DepthMapSRV());
        Effects::BasicFX->SetShadowMap2(mSmap2->DepthMapSRV());

        Effects::NormalMapFX->SetDirLights(mDirLights);
        Effects::NormalMapFX->SetEyePosW(mCam.GetPosition());
        //Effects::NormalMapFX->SetCubeMap(mSky->CubeMapSRV());
        Effects::NormalMapFX->SetShadowMap(mSmap->DepthMapSRV());
        Effects::NormalMapFX->SetShadowMap2(mSmap2->DepthMapSRV());

        Effects::DisplacementMapFX->SetDirLights(mDirLights);
        Effects::DisplacementMapFX->SetEyePosW(mCam.GetPosition());
        //Effects::DisplacementMapFX->SetCubeMap(mSky->CubeMapSRV());
        Effects::DisplacementMapFX->SetShadowMap(mSmap->DepthMapSRV());
        Effects::DisplacementMapFX->SetShadowMap2(mSmap2->DepthMapSRV());
    }
    else
    {
        mTerrain.Draw(md3dImmediateContext, camera, mNoLight);

        Effects::BasicFX->SetDirLights(mNoLight);
        Effects::NormalMapFX->SetDirLights(mNoLight);
        Effects::DisplacementMapFX->SetDirLights(mNoLight);
    }

    ID3D11Buffer* vbs[2] = {mSkullVB, mInstancedBuffer};

    XMMATRIX view     = camera.View();
    XMMATRIX proj     = camera.Proj();
    XMMATRIX viewProj = camera.ViewProj();

    float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

    
    // These properties could be set per object if needed.
    Effects::DisplacementMapFX->SetHeightScale(0.07f);
    Effects::DisplacementMapFX->SetMaxTessDistance(1.0f);
    Effects::DisplacementMapFX->SetMinTessDistance(12.0f);
    Effects::DisplacementMapFX->SetMinTessFactor(1.0f);
    Effects::DisplacementMapFX->SetMaxTessFactor(3.0f);
 
    // Figure out which technique to use for different geometry.
    ID3DX11EffectTechnique* activeTech       = Effects::DisplacementMapFX->Light3TexTech;
    ID3DX11EffectTechnique* activeObjTech    = Effects::BasicFX->Light3Tech;
    ID3DX11EffectTechnique* activeSphereTech = Effects::BasicFX->Light3ReflectTech;
    ID3DX11EffectTechnique* activeSkullTech  = Effects::BasicFX->Light3ReflectTech;
    switch(mRenderOptions)
    {
    case RenderOptionsBasic:
        activeTech = Effects::BasicFX->Light3TexTech;
        activeObjTech = Effects::BasicFX->Light3TexTech;
        md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    case RenderOptionsNormalMap:
        activeTech = Effects::NormalMapFX->Light3TexTech;
        activeObjTech = Effects::NormalMapFX->Light3TexTech;
        md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    }

    XMMATRIX world;
    XMMATRIX worldInvTranspose;
    XMMATRIX worldViewProj;


    // Draw the loaded objects
    UINT stride = sizeof(Vertex::PosNormalTexTan);
    UINT offset = 0;
    D3DX11_TECHNIQUE_DESC techDesc;

    // Draw Loaded Objects
    md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mTreeVB, &stride, &offset);
    md3dImmediateContext->IASetIndexBuffer(mTreeIB, DXGI_FORMAT_R32_UINT, 0);
     
    activeObjTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        //int ofs = 0;
        // Draw the trees.
        for(int i = 0; i < mTreecount; i++)
        {
            world = XMLoadFloat4x4(&mTreeWorld[i]);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;
			vector<ID3D11ShaderResourceView*> texVec;
			vector<ID3D11ShaderResourceView*> normVec;

			texVec.push_back(mCommandoArmor);
			texVec.push_back(mCommandoSkin);

			normVec.push_back(mCommandoArmorNM);
			normVec.push_back(mCommandoSkinNM);

            switch(mRenderOptions)
            {
            case RenderOptionsBasic:
                Effects::BasicFX->SetWorld(world);
                Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::BasicFX->SetWorldViewProj(worldViewProj);
                Effects::BasicFX->SetShadowTransform(world*shadowTransform);
                Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
                Effects::BasicFX->SetMaterial(mTreeMat);
                Effects::BasicFX->SetDiffuseMap(mTreeTexSRV);
				Effects::BasicFX->SetTextureArray(texVec);
                break;
            case RenderOptionsNormalMap:
                Effects::NormalMapFX->SetWorld(world);
                Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
                Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
                Effects::NormalMapFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::NormalMapFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
                Effects::NormalMapFX->SetMaterial(mTreeMat);
                Effects::NormalMapFX->SetDiffuseMap(mCommandoArmor);
                Effects::NormalMapFX->SetNormalMap(mCommandoArmorNM);
				Effects::NormalMapFX->SetNormalArray(normVec);
				Effects::NormalMapFX->SetTextureArray(texVec);
                break;
            }

            activeObjTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mTreeIndexCount, 0, 0);
        }
    }

    

    activeObjTech->GetDesc( &techDesc );
    // Draw the FBX objects
    for(int i = 0; i < 1; i++)
    {
		// Draw the FBX objects
		mFBXVB = mFBXObjects[i]->GetVB();
		mFBXIB = mFBXObjects[i]->GetIB();
		md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mFBXVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mFBXIB, DXGI_FORMAT_R32_UINT, 0);
		
		vector<ID3D11ShaderResourceView*> texVec = mFBXObjects[i]->GetTextureArray();
		vector<ID3D11ShaderResourceView*> normVec = mFBXObjects[i]->GetNormalArray();
		for(int j = 0; j < 5; j++)
		{
			for(UINT p = 0; p < techDesc.Passes; ++p)
			{
				world = XMLoadFloat4x4(&mFBXWorld[j]);
				worldInvTranspose = MathHelper::InverseTranspose(world);
				worldViewProj = world*view*proj;


				switch(mRenderOptions)
				{
				case RenderOptionsBasic:
					Effects::BasicFX->SetWorld(world);
					Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
					Effects::BasicFX->SetWorldViewProj(worldViewProj);
					Effects::BasicFX->SetShadowTransform(world*shadowTransform);
					Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
					/*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
			world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
			world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
					Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
					Effects::BasicFX->SetMaterial(mFBXMats[0]);
					Effects::BasicFX->SetTextureArray(texVec);
					break;
				case RenderOptionsNormalMap:
					Effects::NormalMapFX->SetWorld(world);
					Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
					Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
					Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
					Effects::NormalMapFX->SetShadowTransform2(world*shadowTransform2);
					/*Effects::NormalMapFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
			world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
			world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
					Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
					Effects::NormalMapFX->SetMaterial(mFBXMats[0]);
					Effects::NormalMapFX->SetNormalArray(normVec);
					Effects::NormalMapFX->SetTextureArray(texVec);
					break;
				}
        
				activeObjTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
				md3dImmediateContext->DrawIndexed(mFBXObjects[i]->GetIndexCount(), 0, 0);
			}
		}
    }

    // Draw the grid, cylinders, and box without any cubemap reflection.
    stride = sizeof(Vertex::PosNormalTexTan);
    offset = 0;
     
    md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
    md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

    activeTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
        md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

        // Draw the boxes.
        for(int i = 0; i < mPhysX->GetNumBoxes(); i++)
        {
            world = XMLoadFloat4x4(&mBoxWorld[i]);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            switch(mRenderOptions)
            {
            case RenderOptionsBasic:
                Effects::BasicFX->SetWorld(world);
                Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::BasicFX->SetWorldViewProj(worldViewProj);
                Effects::BasicFX->SetShadowTransform(world*shadowTransform);
                Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::BasicFX->SetTexTransform(XMMatrixScaling(2.0f, 1.0f, 1.0f));
                Effects::BasicFX->SetMaterial(mBoxMat);
                Effects::BasicFX->SetDiffuseMap(mBrickTexSRV);
                break;
            case RenderOptionsNormalMap:
                Effects::NormalMapFX->SetWorld(world);
                Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
                Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
                Effects::NormalMapFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::NormalMapFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(2.0f, 1.0f, 1.0f));
                Effects::NormalMapFX->SetMaterial(mBoxMat);
                Effects::NormalMapFX->SetDiffuseMap(mBrickTexSRV);
                Effects::NormalMapFX->SetNormalMap(mBrickNormalTexSRV);
                break;
            }

            activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);
        }

        // Draw the cylinders.
        for(int i = 0; i < 2; ++i)
        {
            world = XMLoadFloat4x4(&mCylWorld[i]);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            switch(mRenderOptions)
            {
            case RenderOptionsBasic:
                Effects::BasicFX->SetWorld(world);
                Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::BasicFX->SetWorldViewProj(worldViewProj);
                Effects::BasicFX->SetShadowTransform(world*shadowTransform);
                Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 2.0f, 1.0f));
                Effects::BasicFX->SetMaterial(mCylinderMat);
                Effects::BasicFX->SetDiffuseMap(mBrickTexSRV);
                break;
            case RenderOptionsNormalMap:
                Effects::NormalMapFX->SetWorld(world);
                Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
                Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
                Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
                Effects::NormalMapFX->SetShadowTransform2(world*shadowTransform2);
                /*Effects::NormalMapFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
        world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
                Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 2.0f, 1.0f));
                Effects::NormalMapFX->SetMaterial(mCylinderMat);
                Effects::NormalMapFX->SetDiffuseMap(mBrickTexSRV);
                Effects::NormalMapFX->SetNormalMap(mBrickNormalTexSRV);
                break;
            }

            activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
        }
    }

    // FX sets tessellation stages, but it does not disable them.  So do that here to turn off tessellation.
    md3dImmediateContext->HSSetShader(0, 0, 0);
    md3dImmediateContext->DSSetShader(0, 0, 0);

    // Draw the spheres with cubemap reflection.
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    if(drawSphere)
    {
        activeSphereTech->GetDesc( &techDesc );
        for(UINT p = 0; p < techDesc.Passes; ++p)
        {
            // Draw the spheres.
            world = XMLoadFloat4x4(&mSphereWorld);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            Effects::BasicFX->SetWorld(world);
            Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
            Effects::BasicFX->SetWorldViewProj(worldViewProj);
            Effects::BasicFX->SetShadowTransform(world*shadowTransform);
            Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
            /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
    world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
    world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
            Effects::BasicFX->SetTexTransform(XMMatrixIdentity());
            Effects::BasicFX->SetMaterial(mSphereMat);
            Effects::BasicFX->SetCubeMap(mDynamicCubeMapSRVSphere);

            activeSphereTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
        }
    }
    else{
        activeSphereTech->GetDesc( &techDesc );
        for(UINT p = 0; p < techDesc.Passes; ++p)
        {
            // Draw the spheres.
            world = XMLoadFloat4x4(&mSphereWorld);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            Effects::BasicFX->SetWorld(world);
            Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
            Effects::BasicFX->SetWorldViewProj(worldViewProj);
            Effects::BasicFX->SetShadowTransform(world*shadowTransform);
            Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
            /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
    world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
    world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
            Effects::BasicFX->SetTexTransform(XMMatrixIdentity());
            Effects::BasicFX->SetMaterial(mSphereMat);
            //Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());

            activeSphereTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
        }
    }

    // Draw Skull with cubemap reflection.
    //stride = sizeof(Vertex::Basic32);
    //offset = 0;

    //md3dImmediateContext->RSSetState(0);

    //md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    //md3dImmediateContext->IASetVertexBuffers(0, 2, vbs, &stride, &offset); //maybe (0, 1, ...)
    //md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

    //activeSkullTech->GetDesc( &techDesc );
    //if(drawSkull)
    //{	
    //    for(UINT p = 0; p < techDesc.Passes; ++p)
    //    {
    //        // Draw the Skull.
    //        world = XMLoadFloat4x4(&mSkullWorld);
    //        worldInvTranspose = MathHelper::InverseTranspose(world);
    //        worldViewProj = world*view*proj;

    //        Effects::BasicFX->SetWorld(world);
    //        Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
    //        Effects::BasicFX->SetWorldViewProj(worldViewProj);
    //        Effects::BasicFX->SetMaterial(mSkullMat);
    //        Effects::BasicFX->SetShadowTransform(world*shadowTransform);
    //        Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
    //        /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
    //    world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
    //    world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
    //        Effects::BasicFX->SetCubeMap(mDynamicCubeMapSRVSkull);

    //        activeSkullTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
    //        md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
    //    }
    //}
    //else{
    //    for(UINT p = 0; p < techDesc.Passes; ++p)
    //    {
    //        // Draw the Skull.
    //        world = XMLoadFloat4x4(&mSkullWorld);
    //        worldInvTranspose = MathHelper::InverseTranspose(world);
    //        worldViewProj = world*view*proj;

    //        Effects::BasicFX->SetWorld(world);
    //        Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
    //        Effects::BasicFX->SetWorldViewProj(worldViewProj);
    //        Effects::BasicFX->SetMaterial(mSkullMat);
    //        Effects::BasicFX->SetShadowTransform(world*shadowTransform);
    //        Effects::BasicFX->SetShadowTransform2(world*shadowTransform2);
    //        /*Effects::BasicFX->SetShadowTransforms(world*XMLoadFloat4x4(&mShadowTransformOmni[0]),world*XMLoadFloat4x4(&mShadowTransformOmni[1]),
    //    world*XMLoadFloat4x4(&mShadowTransformOmni[2]),world*XMLoadFloat4x4(&mShadowTransformOmni[3]),
    //    world*XMLoadFloat4x4(&mShadowTransformOmni[4]),world*XMLoadFloat4x4(&mShadowTransformOmni[5]));*/
    //        //Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());

    //        activeSkullTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
    //        md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
    //    }
    //}

    mSky->Draw(md3dImmediateContext, camera);

    // restore default states, as the SkyFX changes them in the effect file.
    md3dImmediateContext->RSSetState(0);
    md3dImmediateContext->OMSetDepthStencilState(0, 0);

    // Unbind shadow map as a shader input because we are going to render to it next frame. The shadow might might be at any slot, so clear all slots.
    ID3D11ShaderResourceView* nullSRV[16] = { 0 };
    md3dImmediateContext->PSSetShaderResources(0, 16, nullSRV);

    // Draw particle systems last so it is blended with scene.
    mFire.SetEyePos(camera.GetPosition());
    mFire.Draw(md3dImmediateContext, camera);
    md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default

    //mRain.SetEyePos(camera.GetPosition());
    //mRain.Draw(md3dImmediateContext, camera);

    // restore default states.
    md3dImmediateContext->RSSetState(0);
    md3dImmediateContext->OMSetDepthStencilState(0, 0);
    md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); 
}


void ZeusApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}


void ZeusApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}


void ZeusApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if( (btnState & MK_LBUTTON) != 0 )
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

        mCam.Pitch(dy);
        mCam.RotateY(dx);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}


void ZeusApp::BuildCubeFaceCamera(float x, float y, float z)
{
    // Generate the cube map about the given position.
    XMFLOAT3 center(x, y, z);
    XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);

    // Look along each coordinate axis.
    XMFLOAT3 targets[6] = 
    {
        XMFLOAT3(x+1.0f, y, z), // +X
        XMFLOAT3(x-1.0f, y, z), // -X
        XMFLOAT3(x, y+1.0f, z), // +Y
        XMFLOAT3(x, y-1.0f, z), // -Y
        XMFLOAT3(x, y, z+1.0f), // +Z
        XMFLOAT3(x, y, z-1.0f)  // -Z
    };

    // Use world up vector (0,1,0) for all directions except +Y/-Y.  In these cases, we are looking down +Y or -Y, so we need a different "up" vector.
    XMFLOAT3 ups[6] = 
    {
        XMFLOAT3(0.0f, 1.0f, 0.0f),  // +X
        XMFLOAT3(0.0f, 1.0f, 0.0f),  // -X
        XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
        XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
        XMFLOAT3(0.0f, 1.0f, 0.0f),	 // +Z
        XMFLOAT3(0.0f, 1.0f, 0.0f)	 // -Z
    };

    for(int i = 0; i < 6; ++i)
    {
        mCubeMapCamera[i].LookAt(center, targets[i], ups[i]);
        mCubeMapCamera[i].SetLens(0.5f*XM_PI, 1.0f, 0.1f, 500.0f);
        mCubeMapCamera[i].UpdateViewMatrix();
    }
}

void ZeusApp::BuildCubeFaceShadowTransforms(float x, float y, float z)
{
    // Generate the cube map about the given position.
    XMFLOAT3 center(x, y, z);
    XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);

    // Look along each coordinate axis.
    XMFLOAT3 targets[6] = 
    {
        XMFLOAT3(x+1.0f, y, z), // +X
        XMFLOAT3(x-1.0f, y, z), // -X
        XMFLOAT3(x, y+1.0f, z), // +Y
        XMFLOAT3(x, y-1.0f, z), // -Y
        XMFLOAT3(x, y, z+1.0f), // +Z
        XMFLOAT3(x, y, z-1.0f)  // -Z
    };

    // Use world up vector (0,1,0) for all directions except +Y/-Y.  In these cases, we are looking down +Y or -Y, so we need a different "up" vector.
    XMFLOAT3 ups[6] = 
    {
        XMFLOAT3(0.0f, 1.0f, 0.0f),  // +X
        XMFLOAT3(0.0f, 1.0f, 0.0f),  // -X
        XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
        XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
        XMFLOAT3(0.0f, 1.0f, 0.0f),	 // +Z
        XMFLOAT3(0.0f, 1.0f, 0.0f)	 // -Z
    };

    for(int i = 0; i < 6; ++i)
    {
        XMMATRIX V = XMMatrixLookAtLH( XMLoadFloat3(&center), XMLoadFloat3(&targets[i]), XMLoadFloat3(&ups[i]) );
    
        // Transform bounding sphere to light space.
        XMFLOAT3 sphereCenterLS;
        XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(XMLoadFloat3(&targets[i]), V));
    
        XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PI/2, 1., 1., 20.*mPointLights[0].Range);

        // Transform NDC space [-1,+1]^2 to texture space [0,1]^2
        XMMATRIX T(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f);

        XMMATRIX S = V*P*T;

        XMStoreFloat4x4(&mLightView, V);
        XMStoreFloat4x4(&mLightProj, P);
        XMStoreFloat4x4(&mShadowTransformOmni[i], S);

        mOmniSmaps[i]->BindDsvAndSetNullRenderTarget(md3dImmediateContext);
        DrawSceneToShadowMap();
    }
}


void ZeusApp::BuildDynamicCubeMapViewsSphere()
{
    // Cubemap is a special texture array with 6 elements.
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = CubeMapSizeSphere;
    texDesc.Height = CubeMapSizeSphere;
    texDesc.MipLevels = 0;
    texDesc.ArraySize = 6;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* cubeTex = 0;
    HR(md3dDevice->CreateTexture2D(&texDesc, 0, &cubeTex));

    // Create a render target view to each cube map face (i.e., each element in the texture array).
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvDesc.Texture2DArray.ArraySize = 1;
    rtvDesc.Texture2DArray.MipSlice = 0;

    for(int i = 0; i < 6; ++i)
    {
        rtvDesc.Texture2DArray.FirstArraySlice = i;
        HR(md3dDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &mDynamicCubeMapRTVSphere[i]));
    }

    // Create a shader resource view to the cube map.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = -1;

    HR(md3dDevice->CreateShaderResourceView(cubeTex, &srvDesc, &mDynamicCubeMapSRVSphere));

    ReleaseCOM(cubeTex);

    // We need a depth texture for rendering the scene into the cubemap that has the same resolution as the cubemap faces.  
    D3D11_TEXTURE2D_DESC depthTexDesc;
    depthTexDesc.Width = CubeMapSizeSphere;
    depthTexDesc.Height = CubeMapSizeSphere;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.SampleDesc.Count = 1;
    depthTexDesc.SampleDesc.Quality = 0;
    depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTexDesc.CPUAccessFlags = 0;
    depthTexDesc.MiscFlags = 0;

    ID3D11Texture2D* depthTex = 0;
    HR(md3dDevice->CreateTexture2D(&depthTexDesc, 0, &depthTex));

    // Create the depth stencil view for the entire cube
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = depthTexDesc.Format;
    dsvDesc.Flags  = 0;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    HR(md3dDevice->CreateDepthStencilView(depthTex, &dsvDesc, &mDynamicCubeMapDSVSphere));

    ReleaseCOM(depthTex);

    // Viewport for drawing into cubemap.
    mCubeMapViewport.TopLeftX = 0.0f;
    mCubeMapViewport.TopLeftY = 0.0f;
    mCubeMapViewport.Width    = (float)CubeMapSizeSphere;
    mCubeMapViewport.Height   = (float)CubeMapSizeSphere;
    mCubeMapViewport.MinDepth = 0.0f;
    mCubeMapViewport.MaxDepth = 1.0f;
}


void ZeusApp::BuildDynamicCubeMapViewsSkull()
{
    // Cubemap is a special texture array with 6 elements.
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = CubeMapSizeSkull;
    texDesc.Height = CubeMapSizeSkull;
    texDesc.MipLevels = 0;
    texDesc.ArraySize = 6;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* cubeTex = 0;
    HR(md3dDevice->CreateTexture2D(&texDesc, 0, &cubeTex));

    // Create a render target view to each cube map face (i.e., each element in the texture array).
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvDesc.Texture2DArray.ArraySize = 1;
    rtvDesc.Texture2DArray.MipSlice = 0;

    for(int i = 0; i < 6; ++i)
    {
        rtvDesc.Texture2DArray.FirstArraySlice = i;
        HR(md3dDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &mDynamicCubeMapRTVSkull[i]));
    }

    // Create a shader resource view to the cube map.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = -1;

    HR(md3dDevice->CreateShaderResourceView(cubeTex, &srvDesc, &mDynamicCubeMapSRVSkull));

    ReleaseCOM(cubeTex);

    // We need a depth texture for rendering the scene into the cubemap that has the same resolution as the cubemap faces.  
    D3D11_TEXTURE2D_DESC depthTexDesc;
    depthTexDesc.Width = CubeMapSizeSkull;
    depthTexDesc.Height = CubeMapSizeSkull;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.SampleDesc.Count = 1;
    depthTexDesc.SampleDesc.Quality = 0;
    depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTexDesc.CPUAccessFlags = 0;
    depthTexDesc.MiscFlags = 0;

    ID3D11Texture2D* depthTex = 0;
    HR(md3dDevice->CreateTexture2D(&depthTexDesc, 0, &depthTex));

    // Create the depth stencil view for the entire cube
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = depthTexDesc.Format;
    dsvDesc.Flags  = 0;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    HR(md3dDevice->CreateDepthStencilView(depthTex, &dsvDesc, &mDynamicCubeMapDSVSkull));

    ReleaseCOM(depthTex);

    // Viewport for drawing into cubemap.
    mCubeMapViewport.TopLeftX = 0.0f;
    mCubeMapViewport.TopLeftY = 0.0f;
    mCubeMapViewport.Width    = (float)CubeMapSizeSkull;
    mCubeMapViewport.Height   = (float)CubeMapSizeSkull;
    mCubeMapViewport.MinDepth = 0.0f;
    mCubeMapViewport.MaxDepth = 1.0f;
}


void ZeusApp::BuildDynamicCubeMapViewsMirror()
{
    // Cubemap is a special texture array with 6 elements.
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = CubeMapSizeMirror;
    texDesc.Height = CubeMapSizeMirror;
    texDesc.MipLevels = 0;
    texDesc.ArraySize = 6;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* cubeTex = 0;
    HR(md3dDevice->CreateTexture2D(&texDesc, 0, &cubeTex));

    // Create a render target view to each cube map face  (i.e., each element in the texture array).
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvDesc.Texture2DArray.ArraySize = 1;
    rtvDesc.Texture2DArray.MipSlice = 0;

    for(int i = 0; i < 6; ++i)
    {
        rtvDesc.Texture2DArray.FirstArraySlice = i;
        HR(md3dDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &mDynamicCubeMapRTVMirror[i]));
    }

    // Create a shader resource view to the cube map.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = -1;

    HR(md3dDevice->CreateShaderResourceView(cubeTex, &srvDesc, &mDynamicCubeMapSRVMirror));

    ReleaseCOM(cubeTex);

    // We need a depth texture for rendering the scene into the cubemap that has the same resolution as the cubemap faces.  
    D3D11_TEXTURE2D_DESC depthTexDesc;
    depthTexDesc.Width = CubeMapSizeMirror;
    depthTexDesc.Height = CubeMapSizeMirror;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.SampleDesc.Count = 1;
    depthTexDesc.SampleDesc.Quality = 0;
    depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTexDesc.CPUAccessFlags = 0;
    depthTexDesc.MiscFlags = 0;

    ID3D11Texture2D* depthTex = 0;
    HR(md3dDevice->CreateTexture2D(&depthTexDesc, 0, &depthTex));

    // Create the depth stencil view for the entire cube
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = depthTexDesc.Format;
    dsvDesc.Flags  = 0;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    HR(md3dDevice->CreateDepthStencilView(depthTex, &dsvDesc, &mDynamicCubeMapDSVMirror));

    ReleaseCOM(depthTex);

    // Viewport for drawing into cubemap.
    mCubeMapViewport.TopLeftX = 0.0f;
    mCubeMapViewport.TopLeftY = 0.0f;
    mCubeMapViewport.Width    = (float)CubeMapSizeMirror;
    mCubeMapViewport.Height   = (float)CubeMapSizeMirror;
    mCubeMapViewport.MinDepth = 0.0f;
    mCubeMapViewport.MaxDepth = 1.0f;
}

void ZeusApp::DrawSceneToShadowMap()
{
    if( GetAsyncKeyState('1') & 0x8000 )
        md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

    // TODO:
    //
    // Decide if we want to render shadows from the terrain map.

    // Draw terrain
    //mTerrain.Draw(md3dImmediateContext, mCam, mDirLights);

    XMMATRIX view     = XMLoadFloat4x4(&mLightView);
    XMMATRIX proj     = XMLoadFloat4x4(&mLightProj);
    XMMATRIX viewProj = XMMatrixMultiply(view, proj);

    Effects::BuildShadowMapFX->SetEyePosW(mCam.GetPosition());
    Effects::BuildShadowMapFX->SetViewProj(viewProj);

    // These properties could be set per object if needed.
    Effects::BuildShadowMapFX->SetHeightScale(0.07f);
    Effects::BuildShadowMapFX->SetMaxTessDistance(1.0f);
    Effects::BuildShadowMapFX->SetMinTessDistance(25.0f);
    Effects::BuildShadowMapFX->SetMinTessFactor(1.0f);
    Effects::BuildShadowMapFX->SetMaxTessFactor(5.0f);

    ID3DX11EffectTechnique* tessSmapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
    ID3DX11EffectTechnique* smapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
    switch(mRenderOptions)
    {
    case RenderOptionsBasic:
        md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        smapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
        tessSmapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
        break;
    case RenderOptionsNormalMap:
        md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        smapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
        tessSmapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
        break;
    }

    XMMATRIX world;
    XMMATRIX worldInvTranspose;
    XMMATRIX worldViewProj;

    // Draw Loaded Objects
    UINT stride = sizeof(Vertex::PosNormalTexTan);
    UINT offset = 0;
    D3DX11_TECHNIQUE_DESC techDesc;

    //md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
    //md3dImmediateContext->IASetVertexBuffers(0, 1, &mTreeVB, &stride, &offset);
    //md3dImmediateContext->IASetIndexBuffer(mTreeIB, DXGI_FORMAT_R32_UINT, 0);
    // 
    //tessSmapTech->GetDesc( &techDesc );
    //for(UINT p = 0; p < techDesc.Passes; ++p)
    //{
    //    // Draw the trees.
    //    for(int i = 0; i < mTreecount; i++)
    //    {
    //        world = XMLoadFloat4x4(&mTreeWorld[i]);
    //        worldInvTranspose = MathHelper::InverseTranspose(world);
    //        worldViewProj = world*view*proj;
    //        
    //        Effects::BuildShadowMapFX->SetWorld(world);
    //        Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
    //        Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
    //        Effects::BuildShadowMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

    //        tessSmapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
    //        md3dImmediateContext->DrawIndexed(mTreeIndexCount, 0, 0);
    //    }
    //}

    // Draw the FBX objects
    //md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
    //md3dImmediateContext->IASetVertexBuffers(0, 1, &mFBXVB, &stride, &offset);
    //md3dImmediateContext->IASetIndexBuffer(mFBXIB, DXGI_FORMAT_R32_UINT, 0);

    //tessSmapTech->GetDesc( &techDesc );
    //// Draw the FBX objects
    //for(int i = 0; i < mFBXObjCount; i++)
    //{
    //    for(UINT p = 0; p < techDesc.Passes; ++p)
    //    {
    //        world = XMLoadFloat4x4(&mFBXWorld[i]);
    //        worldInvTranspose = MathHelper::InverseTranspose(world);
    //        worldViewProj = world*view*proj;
    //    
    //        Effects::BuildShadowMapFX->SetWorld(world);
    //        Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
    //        Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
    //        Effects::BuildShadowMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

    //        tessSmapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
    //        md3dImmediateContext->DrawIndexed(mFBXIndexCount[i],mFBXIndexOffset[i], mFBXVertexOffset[i]);
    //    }
    //}

    // Draw the grid, cylinders, and box.
    stride = sizeof(Vertex::PosNormalTexTan);
    offset = 0;

    md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
    md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
    
    tessSmapTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        // Draw the box.
        for(int i = 0; i < mPhysX->GetNumBoxes(); i++)
        {
            world = XMLoadFloat4x4(&mBoxWorld[i]);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            Effects::BuildShadowMapFX->SetWorld(world);
            Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
            Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
            Effects::BuildShadowMapFX->SetTexTransform(XMMatrixScaling(2.0f, 1.0f, 1.0f));

            tessSmapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);
        }

        // Draw the cylinders.
        for(int i = 0; i < 2; ++i)
        {
            world = XMLoadFloat4x4(&mCylWorld[i]);
            worldInvTranspose = MathHelper::InverseTranspose(world);
            worldViewProj = world*view*proj;

            Effects::BuildShadowMapFX->SetWorld(world);
            Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
            Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
            Effects::BuildShadowMapFX->SetTexTransform(XMMatrixScaling(1.0f, 2.0f, 1.0f));

            tessSmapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
            md3dImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
        }
    }

    // FX sets tessellation stages, but it does not disable them.  So do that here to turn off tessellation.
    md3dImmediateContext->HSSetShader(0, 0, 0);
    md3dImmediateContext->DSSetShader(0, 0, 0);

    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw the spheres with cubemap reflection.
    smapTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        // Draw the spheres.
        world = XMLoadFloat4x4(&mSphereWorld);
        worldInvTranspose = MathHelper::InverseTranspose(world);
        worldViewProj = world*view*proj;

        Effects::BuildShadowMapFX->SetWorld(world);
        Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
        Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
        Effects::BuildShadowMapFX->SetTexTransform(XMMatrixIdentity());

        smapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
        md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
    }

    //stride = sizeof(Vertex::Basic32);
    //offset = 0;

    //md3dImmediateContext->RSSetState(0);

    //md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    //md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
    //md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

    //smapTech = Effects::BuildShadowMapFX->BuildShadowMapTech;
    //smapTech->GetDesc( &techDesc );
    //for(UINT p = 0; p < techDesc.Passes; ++p)
    //{
    //    // Draw the Skull.
    //    world = XMLoadFloat4x4(&mSkullWorld);
    //    worldInvTranspose = MathHelper::InverseTranspose(world);
    //    worldViewProj = world*view*proj;

    //    Effects::BuildShadowMapFX->SetWorld(world);
    //    Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
    //    Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
    //    Effects::BuildShadowMapFX->SetTexTransform(XMMatrixIdentity());

    //    smapTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
    //    md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
    //}
}

void ZeusApp::BuildShadowTransform(int source, bool omni)
{
    // Only the first "main" light casts a shadow.
    XMVECTOR lightDir = XMLoadFloat3(&mDirLights[source].Direction);
    XMVECTOR lightPos = -2.0f*mSceneBounds.Radius*lightDir + mCam.GetPositionXM() + mCam.GetLookXM();
    XMVECTOR targetPos = mCam.GetPositionXM() + mCam.GetLookXM();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    float scale = 5.0f;
    if(source !=0)
    {
        lightPos = -2.0f*mSceneBounds.Radius*lightDir + mCam.GetPositionXM() + mCam.GetLookXM();
        targetPos = mCam.GetPositionXM() + mCam.GetLookXM();
        scale = 10.0f;
    }

    XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);
    
    // Transform bounding sphere to light space.
    XMFLOAT3 sphereCenterLS;
    XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));
    
    if(omni)
    {
        V = mCubeMapCamera[source].View();
        XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(-2.0f*mSceneBounds.Radius*XMLoadFloat3(&mCubeMapCamera[source].GetLook()), V)); //??? removed and doesn't appear to change anything
    }
    
    
    // Ortho frustum in light space encloses scene.
    float l = sphereCenterLS.x - scale*mSceneBounds.Radius;
    float b = sphereCenterLS.y - scale*mSceneBounds.Radius;
    float n = sphereCenterLS.z - scale*mSceneBounds.Radius;
    float r = sphereCenterLS.x + scale*mSceneBounds.Radius;
    float t = sphereCenterLS.y + scale*mSceneBounds.Radius;
    float f = sphereCenterLS.z + scale*mSceneBounds.Radius;
    XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

    // Transform NDC space [-1,+1]^2 to texture space [0,1]^2
    XMMATRIX T(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f);

    XMMATRIX S = V*P*T;

    XMStoreFloat4x4(&mLightView, V);
    XMStoreFloat4x4(&mLightProj, P);
    if(!omni)
    {
        if(source == 0)
            XMStoreFloat4x4(&mShadowTransform, S);
        else
            XMStoreFloat4x4(&mShadowTransform2, S);
    }
    else
    {
        XMStoreFloat4x4(&mShadowTransformOmni[source], S); //??? removed and doesn't appear to change anything
    }
}


void ZeusApp::BuildShapeGeometryBuffers()
{
    GeometryGenerator::MeshData box;
    GeometryGenerator::MeshData grid;
    GeometryGenerator::MeshData sphere;
    GeometryGenerator::MeshData cylinder;

    GeometryGenerator geoGen;
    geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
    //geoGen.CreateGrid(20.0f, 30.0f, 50, 40, grid);
    geoGen.CreateSphere(1.0f, 20, 20, sphere);
    geoGen.CreateCylinder(0.5f, 0.5f, 3.0f, 15, 15, cylinder);

    // Cache the vertex offsets to each object in the concatenated vertex buffer.
    mBoxVertexOffset      = 0;
    mGridVertexOffset     = box.Vertices.size();
    mSphereVertexOffset   = mGridVertexOffset + grid.Vertices.size();
    mCylinderVertexOffset = mSphereVertexOffset + sphere.Vertices.size();

    // Cache the index count of each object.
    mBoxIndexCount      = box.Indices.size();
    mGridIndexCount     = grid.Indices.size();
    mSphereIndexCount   = sphere.Indices.size();
    mCylinderIndexCount = cylinder.Indices.size();

    // Cache the starting index for each object in the concatenated index buffer.
    mBoxIndexOffset      = 0;
    mGridIndexOffset     = mBoxIndexCount;
    mSphereIndexOffset   = mGridIndexOffset + mGridIndexCount;
    mCylinderIndexOffset = mSphereIndexOffset + mSphereIndexCount;
    
    UINT totalVertexCount = 
        box.Vertices.size() + 
        grid.Vertices.size() + 
        sphere.Vertices.size() +
        cylinder.Vertices.size();

    UINT totalIndexCount = 
        mBoxIndexCount + 
        mGridIndexCount + 
        mSphereIndexCount +
        mCylinderIndexCount;

    // Extract the vertex elements we are interested in and pack the vertices of all the meshes into one vertex buffer.
    std::vector<Vertex::PosNormalTexTan> vertices(totalVertexCount);

    UINT k = 0;
    for(size_t i = 0; i < box.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos      = box.Vertices[i].Position;
        vertices[k].Normal   = box.Vertices[i].Normal;
        vertices[k].Tex      = box.Vertices[i].TexC;
        vertices[k].TangentU = box.Vertices[i].TangentU;
    }

    for(size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos      = grid.Vertices[i].Position;
        vertices[k].Normal   = grid.Vertices[i].Normal;
        vertices[k].Tex      = grid.Vertices[i].TexC;
        vertices[k].TangentU = grid.Vertices[i].TangentU;
    }

    for(size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos      = sphere.Vertices[i].Position;
        vertices[k].Normal   = sphere.Vertices[i].Normal;
        vertices[k].Tex      = sphere.Vertices[i].TexC;
        vertices[k].TangentU = sphere.Vertices[i].TangentU;
    }

    for(size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos      = cylinder.Vertices[i].Position;
        vertices[k].Normal   = cylinder.Vertices[i].Normal;
        vertices[k].Tex      = cylinder.Vertices[i].TexC;
        vertices[k].TangentU = cylinder.Vertices[i].TangentU;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * totalVertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mShapesVB));

    // Pack the indices of all the meshes into one index buffer.
    std::vector<UINT> indices;
    indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
    indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
    indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
    indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));
}
 
int normal = 0;
int tex = 0;
void ZeusApp::BuildSkullGeometryBuffers()
{
    std::ifstream fin("Models/cow.obj");
    
    if(!fin)
    {
        MessageBox(0, L"Models/cow.obj not found.", 0, 0);
        return;
    }

    char line[100];
    float x = 0, y = 0, z = 0;
    //int normal = 1;
    normal = 0;
    std::vector<Vertex::Basic32> vertices;
    std::vector<int> indices;
    std::vector<Vertex::Basic32> verts;
    //std::vector<Vertex::Basic32> norms;
    normal = 0;
    while( !fin.eof() ) {
        fin >> line;

        if( strcmp( line, "#" ) == 0 ) {
            // Read in the comment and do nothing
            fin.getline( line, 100);
        } else {
            if( strcmp( line, "f" ) == 0 ) {
                for(int i = 0; i < 3; i++) {
                    fin >> line;
                    indices.push_back( atoi( strtok( line, "/" ) ) - 1 );
                }
            } else if (strcmp( line, "v" ) == 0 ) {
                Vertex::Basic32 vertex;
                fin >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
                    
                verts.push_back( vertex );
            } else if (strcmp( line, "vn" ) == 0 ) {
                fin >> verts[normal].Normal.x >> verts[normal].Normal.y >> verts[normal].Normal.z;
                normal++;
            }
        }
    }

    for(int i = 0; i < verts.size(); i++) {
        vertices.push_back(verts[i]);
    }
    mSkullIndexCount = indices.size();
    fin.close();

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::Basic32) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

    //
    // Pack the indices of all the meshes into one index buffer.
    //

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));
}


void ZeusApp::BuildScreenQuadGeometryBuffers()
{
    GeometryGenerator::MeshData quad;

    GeometryGenerator geoGen;
    geoGen.CreateFullscreenQuad(quad);

    // Extract the vertex elements we are interested in and pack the vertices of all the meshes into one vertex buffer.
    std::vector<Vertex::Basic32> vertices(quad.Vertices.size());

    for(UINT i = 0; i < quad.Vertices.size(); ++i)
    {
        vertices[i].Pos    = quad.Vertices[i].Position;
        vertices[i].Normal = quad.Vertices[i].Normal;
        vertices[i].Tex    = quad.Vertices[i].TexC;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::Basic32) * quad.Vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mScreenQuadVB));

    // Pack the indices of all the meshes into one index buffer.
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * quad.Indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &quad.Indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mScreenQuadIB));
}

void ZeusApp::LoadTreeBuffer()
{
    std::ifstream fin("Models/tree1.obj");
    if(!fin)
    {
        MessageBox(0, L"Models/tree1.obj not found.", 0, 0);
        return;
    }

    char line[100];
    float x = 0, y = 0, z = 0;
    //int normal;
    std::vector<Vertex::PosNormalTexTan> vertices;
    std::vector<int> indices;
    std::vector<Vertex::PosNormalTexTan> verts;
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> norms;
	std::vector<XMFLOAT2> texVec;
	std::vector<int> texNum;
    //std::vector<Vertex::Basic32> norms;
    normal = 0;
    tex = 0;
 //   while( !fin.eof() ) {
 //       fin >> line;

 //       if( strcmp( line, "#" ) == 0 ) {
 //           // Read in the comment and do nothing
 //           fin.getline( line, 100);
 //       } else {
 //           if( strcmp( line, "f" ) == 0 ) {
 //               for(int i = 0; i < 3; i++) {
 //                   fin >> line;
 //                   indices.push_back( atoi( strtok( line, "/" ) ) - 1 );
 //               }
 //           } else if (strcmp( line, "v" ) == 0 ) {
 //               Vertex::Basic32 vertex;
 //               fin >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
 //                   
 //               verts.push_back( vertex );
 //           } else if (strcmp( line, "vn" ) == 0 ) {
 //               fin >> verts[normal].Normal.x >> verts[normal].Normal.y >> verts[normal].Normal.z;
 //               normal++;
 //           } else if (strcmp( line, "vt" ) == 0 ) {
 //               fin >> verts[tex].Tex.x >> verts[tex].Tex.y;
 //               tex++;
 //           }
 //       }
 //   }
	//fin.close();
 //   
	//for(int i = 0; i < verts.size(); i++) {
 //       vertices.push_back(verts[i]);
	//	positions.push_back(verts[i].Pos);
 //   }
    FBXImporter importer;
    importer.Import("Models/bigbadman.fbx", &positions, &indices, &norms, &texVec, &texNum);

	Vertex::PosNormalTexTan tempVert;

	XMVECTOR p;
	XMVECTOR n;
	for(int i = 0; i < positions.size(); i++)
	{
		p = XMVectorSet(positions[i].x, positions[i].y,positions[i].z, 1.0 );
		n = XMVectorSet(norms[i].x, norms[i].y, norms[i].z, 1.0 );
		XMVECTOR cross = XMVector3Cross(p, n);
		XMStoreFloat3(&tempVert.TangentU, cross);

		tempVert.Pos = positions[i];
		tempVert.Normal = norms[i];
		tempVert.Tex = texVec[i];
		tempVert.TexNum = texNum[i];

		vertices.push_back(tempVert);
	}

    mTreeIndexCount = indices.size();
    mTreeVertCount = positions.size();
	mTreepositions = positions;
	mTreeIndices = indices;

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mTreeVB));

    //
    // Pack the indices of all the meshes into one index buffer.
    //

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * mTreeIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mTreeIB));
}

void ZeusApp::CreateTreeMatrixes()
{
    XMVECTOR treepositions[] = {
        {	10.0f,		0.0f,		50.0f	},
        {	0.0f,		0.0f,		30.0f	},
        {	-10.0f,		0.0f,		75.0f	},
        {	50.0f,		0.0f,		10.0f	},
        {	70.0f,		0.0f,		50.0f	},
        {	80.0f,		0.0f,		-30.0f	},
        {	30.0f,		0.0f,		-10.0f	},
        {	55.0f,		0.0f,		60.0f	},
        {	85.0f,		0.0f,		40.0f	},
        {	20.0f,		0.0f,		20.0f	},
        {	82.0f,		0.0f,		-3.0f	}, 
        {	46.0f,		0.0f,		-48.0f	}, 
        {	-35.0f,		0.0f,		-39.0f	}, 
        {	49.0f,		0.0f,		-27.0f	},
        {	56.0f,		0.0f,		-77.0f	},
        {	25.0f,		0.0f,		-72.0f	},
        {	14.0f,		0.0f,		-53.0f	},
        {	-9.0f,		0.0f,		-38.0f	},
        {	-58.0f,		0.0f,		-45.0f	},
        {	-81.0f,		0.0f,		-23.0f	},
        {	-99.0f,		0.0f,		-45.0f	},
        {	68.0f,		0.0f,		-89.0f	},
        {	100.0f,		0.0f,		-72.0f	},
        {	138.0f,		0.0f,		-36.0f	},
        {	120.0f,		0.0f,		4.0f	},
        {	79.0f,		0.0f,		84.0f	},
        {	32.0f,		0.0f,		95.0f	},
        {	-3.0f,		0.0f,		104.0f	},
        {	-28.0f,		0.0f,		144.0f	}
    };

    mTreecount = sizeof( treepositions ) / sizeof( XMVECTOR );
    mTreeWorld = new XMFLOAT4X4[mTreecount];
    float scale = 8.0f;
    XMMATRIX treeScale = XMMatrixScaling(scale, scale, scale);
    XMMATRIX treeOffset;
    XMFLOAT3 treeposition;

    for(int i = 0; i < mTreecount; i++)
    {
        XMStoreFloat3(&treeposition, treepositions[i]);
        treeOffset = XMMatrixTranslation(treeposition.x,treeposition.y, treeposition.z);
        XMStoreFloat4x4(&mTreeWorld[i], XMMatrixMultiply(treeScale, treeOffset));
        CreatePhysXTriangleMesh(ObjectNumbers::tree, mTreeVertCount, mTreepositions,
            mTreeIndexCount, mTreeIndices, treeposition.x, treeposition.y, treeposition.z, scale);
    }
}

void ZeusApp::LoadFBXBuffers()
{
    char line[100];

    char *filenames[] = {
        "Models/bigbadman.fbx"
    };
    
    int numFiles = sizeof(filenames) / sizeof(char*);
    for(int i = 0; i < numFiles; i++)
    {
        FBXObj object;
        object.Import(filenames[i], md3dDevice);
		object.LoadTexture(md3dDevice, L"Textures/CommandoArmor_DM.dds");
		object.LoadTexture(md3dDevice, L"Textures/Commando_DM.dds");
		object.LoadNormal(md3dDevice, L"Textures/CommandoArmor_NM.dds");
		object.LoadNormal(md3dDevice, L"Textures/Commando_NM.dds");
        mFBXObjects.push_back(&object);
    }
}

void ZeusApp::CreateFBXMatrixes()
{
    XMVECTOR fbxpositions[] = {
        {	0.0f,		0.0f,		0.0f	},
		{	100.0f,		0.0f,		-100.0f	},
		{	80.0f,	    0.0f,	    -110.0f	},
		{	110.0f,		0.0f,		-80.0f	},
		{	75.0f,		0.0f,		80.0f	}
    };

    XMVECTOR fbxscales[] = {
        {	8.0f,		8.0f,		8.0f    },
		{	8.0f,		8.0f,		8.0f    },
		{	8.0f,		8.0f,		8.0f    },
		{	8.0f,		8.0f,		8.0f    },
		{	8.0f,		8.0f,		8.0f    }
    };

    XMVECTOR fbxrotations[] = {
        {	0.0f,		0.0f,		0.0f	},
		{	0.0f,		0.0f,		0.0f	},
		{	0.0f,		0.0f,		0.0f	},
		{	0.0f,		0.0f,		0.0f	},
		{	0.0f,		0.0f,		0.0f	}
    };

    mFBXWorldCount = sizeof( fbxpositions ) / sizeof( XMVECTOR );
    mFBXWorld = new XMFLOAT4X4[mFBXWorldCount];
    mFBXMats = new Material[mFBXWorldCount];
    mFBXMats[0].Ambient  = XMFLOAT4(0.99f, 0.99f, 0.99f, 1.0f);
    mFBXMats[0].Diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mFBXMats[0].Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 0.8f);
    mFBXMats[0].Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    XMMATRIX fbxScale;
    XMMATRIX fbxOffset;
    XMMATRIX fbxRotation;
    XMFLOAT3 fbxposition;
    XMFLOAT3 fbxrotation;
    XMFLOAT3 fbxscale;

    for(int i = 0; i < mFBXWorldCount; i++)
    {
        XMStoreFloat3(&fbxposition, fbxpositions[i]);
        XMStoreFloat3(&fbxrotation, fbxrotations[i]);
        XMStoreFloat3(&fbxscale, fbxscales[i]);

        fbxOffset = XMMatrixTranslation(fbxposition.x, fbxposition.y, fbxposition.z);
        fbxScale = XMMatrixScaling(fbxscale.x, fbxscale.y, fbxscale.z);
        fbxRotation = XMMatrixRotationX(fbxrotation.x) * XMMatrixRotationY(fbxrotation.y) * XMMatrixRotationZ(fbxrotation.z);

        XMStoreFloat4x4(&mFBXWorld[i], XMMatrixMultiply(fbxScale, XMMatrixMultiply(fbxOffset, fbxRotation) ) );
        //Put into physx
        CreatePhysXTriangleMesh(ObjectNumbers::tree, mTreeVertCount, mTreepositions,
            mTreeIndexCount, mTreeIndices, fbxposition.x, fbxposition.y, fbxposition.z, fbxscale.x);
    }
}

void ZeusApp::BuildInstancedBuffer()
{
    const int n = 5;
    mInstancedData.resize(n*n*n);
    
    float width = 200.0f;
    float height = 200.0f;
    float depth = 200.0f;
    
    float x = -0.5f*width;
    float y = -0.5f*height;
    float z = -0.5f*depth;
    float dx = width / (n-1);
    float dy = height / (n-1);
    float dz = depth / (n-1);
    for(int k = 0; k < n; ++k)
    {
        for(int i = 0; i < n; ++i)
        {
            for(int j = 0; j < n; ++j)
            {
                // Position instanced along a 3D grid.
                mInstancedData[k*n*n + i*n + j].World = XMFLOAT4X4(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    x+j*dx, y+i*dy, z+k*dz, 1.0f);
                
                // Random color.
                mInstancedData[k*n*n + i*n + j].Color.x = MathHelper::RandF(0.0f, 1.0f);
                mInstancedData[k*n*n + i*n + j].Color.y = MathHelper::RandF(0.0f, 1.0f);
                mInstancedData[k*n*n + i*n + j].Color.z = MathHelper::RandF(0.0f, 1.0f);
                mInstancedData[k*n*n + i*n + j].Color.w = 1.0f;
            }
        }
    }
    
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(InstancedData) * mInstancedData.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    
    HR(md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer));
}


void ZeusApp::PxtoXMMatrix(PxTransform input, XMMATRIX* start)
{
    
    PxMat33 quat = PxMat33(input.q);

    start->_11 = quat.column0[0];
   start->_12 = quat.column0[1];
   start->_13 = quat.column0[2];


   start->_21 = quat.column1[0];
   start->_22 = quat.column1[1];
   start->_23 = quat.column1[2];


   start->_31 = quat.column2[0];
   start->_32 = quat.column2[1];
   start->_33 = quat.column2[2];


    start->_41 = input.p.x;
    start->_42 = input.p.y;
    start->_43 = input.p.z;
    
}

void ZeusApp::CreatePhysXTriangleMesh(	ObjectNumbers objnum, int numVerts, std::vector<XMFLOAT3> verts,
                                        int numInds, std::vector<int> inds, float x, float y, float z, float scale)

{
    PxVec3* vertices = new PxVec3[numVerts];
    for(int i = 0; i < numVerts; i++)
    {
        vertices[i].x = verts[i].x;
        vertices[i].y = verts[i].y;
        vertices[i].z = verts[i].z;
    }

    int* indices = new int[numInds];
    for(int i = 0; i < numInds; i++)
    {
        indices[i] = inds[i];
    }

    mPhysX->SetupTriangleMesh(objnum, numVerts, vertices, numInds, indices, x, y, z, scale);
    delete [] indices;
    delete [] vertices;
}

void ZeusApp::CreatePhysXTriangleMeshTerrain( int numVerts, std::vector<XMFLOAT3> verts, int numInds, std::vector<int> inds)
{
    PxVec3* vertices = new PxVec3[numVerts];
    for(int i = 0; i < numVerts; i++)
    {
        vertices[i].x = verts[i].x;
        vertices[i].y = verts[i].y;
        vertices[i].z = verts[i].z;
    }

    int* indices = new int[numInds];
    for(int i = 0; i < numInds; i++)
    {
        indices[i] = inds[i];
    }

    mPhysX->CreateTerrain(numVerts,vertices,numInds,indices);
    delete [] indices;
    delete [] vertices;
}