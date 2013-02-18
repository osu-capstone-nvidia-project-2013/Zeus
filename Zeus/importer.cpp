//*********************************************************
// importer.cpp
//********************************************************
#include "importer.h"

/*
Make sure to include the fbx sdk and to link
fbxsdk-2013.3d.lib
or 
fbxsdk-2013.3.lib
*/

using namespace std;

//const char* lFilename = "traingles.fbx";
const char* lFilename;

int size;
int cornerIndex;
int *index;

int texNo = -1;

FbxVector4 fbxVertex;
FbxVector4 fbxNormal;
FbxVector2 fbxUV;

vector<XMFLOAT3> *vertices;
vector<XMFLOAT3> *normals;
vector<XMFLOAT2> *texcoords;
vector<int>	 *indices;
vector<int>    *texnum;

FBXImporter::FBXImporter()
{
}

FBXImporter::~FBXImporter()
{
}

// recursive DisplayContent Where the magic happens
void FBXImporter::DisplayContent(KFbxNode* pNode)
{
	FbxVector4 v4; // Default translation values
	cout << "Child name: " << pNode->GetName() << endl;

	FbxMesh *mesh = pNode->GetMesh();

	if(mesh)
	{
		FbxVector4* meshVertices = mesh->GetControlPoints();
		int polygonCount = mesh->GetPolygonCount();
		cout << "Polygon Count: " << polygonCount << endl;

		int numIndices=mesh->GetPolygonVertexCount();
		index = new int[numIndices];
		index = mesh->GetPolygonVertices();
		cout << "Num Indices: " << numIndices << endl;

		//Because our engine doesnt support multiple meshes
		size = vertices->size();

		for( int i = 0; i< polygonCount; i++)
		{
			int TriSize = mesh->GetPolygonSize(i);//Should be 3

			for( int j =0; j<TriSize; j++)
			{			
				int index = mesh->GetPolygonVertex(i,j);
				indices->push_back(index+size);

			}
			//cout << endl;
		}

		//loadVectors( pNode, polygonCount, mesh, meshVertices );
		int i, lControlPointsCount = mesh->GetControlPointsCount();
		FbxVector4* lControlPoints = mesh->GetControlPoints();

		if( lControlPointsCount > 0 )
			texNo++;

		//		cout << "    Control Points" << endl;

		for (i = 0; i < lControlPointsCount; i++)
		{
			//** VERTICES **//
			vertices->push_back( XMFLOAT3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]));
			texnum->push_back( texNo );

			//** NORMALS **//
			for (int j = 0; j < mesh->GetElementNormalCount(); j++)
			{
				FbxGeometryElementNormal* leNormals = mesh->GetElementNormal( j);

				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					char header[100];
					//					cout << "            normal Vector: "; 
					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						normals->push_back( XMFLOAT3( leNormals->GetDirectArray().GetAt(i)[0], 
						leNormals->GetDirectArray().GetAt(i)[1], 
						leNormals->GetDirectArray().GetAt(i)[2] ));
					}
				}
			}
		}

	}
	texcoords->resize(vertices->size());

	if( mesh )
	{
		FbxStringList lUVSetNameList;
		mesh->GetUVSetNames(lUVSetNameList);

		for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		{
			//get lUVSetIndex-th uv set
			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
			const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);

			if(!lUVElement)
				continue;

			// only support mapping mode eByPolygonVertex and eByControlPoint
			if( lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
				lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint )
				return;

			//index array, where holds the index referenced to the uv data
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
			const int lIndexCount= (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

			//iterating through the data by polygon
			const int lPolyCount = mesh->GetPolygonCount();

			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				int lPolyIndexCounter = 0;
				for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
					for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
					{
						if (lPolyIndexCounter < lIndexCount)
						{
							FbxVector2 lUVValue;

							//the UV index depends on the reference mode
							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
							int lControlPointIndex = mesh->GetPolygonVertex(lPolyIndex, lVertIndex);

							cout << lControlPointIndex+size << ": " << lUVValue[0] << " " << lUVValue[1] << " " << size << endl;

							texcoords[0][lControlPointIndex+size] = XMFLOAT2( (float)lUVValue[0], 1.0f-(float) lUVValue[1] );

							lPolyIndexCounter++;
						}
					}
				}
			}
		}
	}
	for(int i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}

// Non-recursive DisplayContent() is called once
void FBXImporter::DisplayContent(FbxScene* pScene)
{
	cout << "Starting Recursion" << endl;

	int i;
	FbxNode* lNode = pScene->GetRootNode();
	if(lNode)
	{
		for(i = 0; i < lNode->GetChildCount(); i++)
		{
			// Call recursive DisplayContent() once for each child of the root node
			DisplayContent(lNode->GetChild(i));
		}
	}

	cout << "Done" << endl;
}


int FBXImporter::Import(char *name, vector<XMFLOAT3> *vec, vector<int> *ind, vector<XMFLOAT3> *norm, vector<XMFLOAT2> *tex, vector<int> *num )
{

	texNo = -1;
	//	char* lFilename = "traingles.fbx";
	lFilename = name;

	// point to the vectors for changes to be made
	vertices = vec;
	indices = ind;
	normals = norm;
	texcoords = tex;
	texnum = num;

	//const std::string &fileName = "sadirohyea.fbx";
	FbxVector4 fbxVertex;
	FbxVector4 fbxNormal;
	FbxVector2 fbxUV;
	int cornerIndex;
	// Create the FBX SDK manager
	FbxManager* lSdkManager = FbxManager::Create();

	// Create an IOSettings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
	lSdkManager->SetIOSettings(ios);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Initialize the importer.
	bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

	// Check to make sure the file exists
	if(!lImportStatus) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());
		cin >> cornerIndex;
		exit(-1);
	}

	// Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "MyScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file has been imported; we can get rid of the importer.
	lImporter->Destroy();

	FbxNode* rootNode = lScene->GetRootNode();
	if(!rootNode)
		cout << "failed. no rootNode" << endl;

	// Traverse through the trees
	DisplayContent( lScene );

	return texNo;
}