#include "importer.h"

/*
Make sure to include the fbx sdk and to link
fbxsdk-2013.3d.lib
or 
fbxsdk-2013.3.lib
*/

using namespace std;

const char* lFilename = "Models/traingles.fbx";

int size;
int cornerIndex;
int *index;

FbxVector4 fbxVertex;
FbxVector4 fbxNormal;
FbxVector2 fbxUV;

vector<XMFLOAT3> *vertices;
vector<XMFLOAT3> *normals;
vector<XMFLOAT2> texcoords;
vector<WORD>	 *indices;

void loadVectors(KFbxNode* pNode, int polygonCount, FbxMesh *mesh, FbxVector4* meshVertices)
{
	for(int iPoly = 0; iPoly < polygonCount; iPoly++)
	{
		for(unsigned iPolyVert = 0; iPolyVert<3; iPolyVert++)
		{
			cornerIndex = mesh->GetPolygonVertex(iPoly, iPolyVert);

			fbxVertex = meshVertices[cornerIndex];
			cout << vertices->size() << ": " << fbxVertex[0] << " " << fbxVertex[1] << " " << fbxVertex[2] <<endl;
			vertices->push_back(XMFLOAT3((float)fbxVertex[0],(float)fbxVertex[1],(float)fbxVertex[2]));

			mesh->GetPolygonVertexNormal(iPoly, iPolyVert, fbxNormal);
			fbxNormal.Normalize();
			normals->push_back(XMFLOAT3((float)fbxNormal[0],(float)fbxNormal[1],(float)fbxNormal[2]));

			fbxUV = FbxVector2(0,0);
			FbxLayerElementUV* fbxLayerUV = mesh -> GetLayer(0) -> GetUVs();
			if(fbxLayerUV)
			{
				int iUVIndex = 0;
				switch(fbxLayerUV->GetMappingMode())
				{
				case FbxLayerElement::eByControlPoint:
					iUVIndex = cornerIndex;
					break;
				case FbxLayerElement::eByPolygonVertex:
					iUVIndex = mesh->GetTextureUVIndex(iPoly, iPolyVert, FbxLayerElement::eTextureDiffuse);
					break;
				}
				fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);
			}
			//cout << fbxUV[0] << " " << fbxUV[1] << endl;
			texcoords.push_back(XMFLOAT2((float)fbxUV[0],(float)fbxUV[1])); 
		}
	}
	return;
}

// recursive DisplayContent Where the magic happens
void DisplayContent(KFbxNode* pNode)
{
	FbxVector4 v4; // Default translation values
	//cout << "Child name: " << pNode->GetName() << endl;

	FbxMesh *mesh = pNode->GetMesh();

	if(mesh)
	{
		FbxVector4* meshVertices = mesh->GetControlPoints();
		int polygonCount = mesh->GetPolygonCount();
		//cout << "Polygon Count: " << polygonCount << endl;

		int numIndices=mesh->GetPolygonVertexCount();
		index = new int[numIndices];
		index = mesh->GetPolygonVertices();
		//cout << "Num Indices: " << numIndices << endl;

		//Because our engine doesnt support multiple meshes
		size = vertices->size();

		for( int i = 0; i< polygonCount; i++)
		{
			int TriSize = mesh->GetPolygonSize(i);//Should be 3

			for( int j =0; j<TriSize; j++)
			{			
				int index = mesh->GetPolygonVertex(i,j);
				//cout << index << " ";
				indices->push_back(index+size);

			}
			//cout << endl;
		}

		//loadVectors( pNode, polygonCount, mesh, meshVertices );
		int i, lControlPointsCount = mesh->GetControlPointsCount();
		FbxVector4* lControlPoints = mesh->GetControlPoints();

		//float point = lControlPoints[0];

		//cout << "    Control Points" << endl;

		for (i = 0; i < lControlPointsCount; i++)
		{
			//cout << "        Control Point " << i << endl;
			//cout << "            Coordinates: ";
			//for( int j = 0; j < 3; j++ )
				 //cout << lControlPoints[i][j] << " ";
			//cout << endl;

			vertices->push_back( XMFLOAT3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]));

			for (int j = 0; j < mesh->GetElementNormalCount(); j++)
			{
				FbxGeometryElementNormal* leNormals = mesh->GetElementNormal( j);

				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					char header[100];
					//cout << "            Normal Vector: "; 
					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect){
						//for( int k = 0; k < 3; k++ )
						//	cout << leNormals->GetDirectArray().GetAt(i)[k] << " ";
						normals->push_back( XMFLOAT3( 
							leNormals->GetDirectArray().GetAt(i)[0], 
							leNormals->GetDirectArray().GetAt(i)[1], 
							leNormals->GetDirectArray().GetAt(i)[2] ));

						//cout << endl;
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
void DisplayContent(FbxScene* pScene)
{
	//cout << "Starting Recursion" << endl;

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

	//cout << "Done" << endl;
}


void Import(vector<XMFLOAT3> *vec, vector<WORD> *ind, vector<XMFLOAT3> *norm)
{

	// point to the vectors for changes to be made
	vertices = vec;
	indices = ind;
	normals = norm;

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

	//	cout << "success" << endl;
	//	cout << "Creating scene......" << endl;

	// Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "MyScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file has been imported; we can get rid of the importer.
	lImporter->Destroy();

	//	cout << "Scene created" << endl;

	FbxNode* rootNode = lScene->GetRootNode();
	if(!rootNode)
		cout << "failed. no rootNode" << endl;

	// Traverse through the trees
	DisplayContent( lScene );

//	cout << "Vertices: " << vertices->size() << "\n";
//	cout << "Normals:  " << normals->size() << "\n";
//	cout << "Textures: " << texcoords.size() << "\n";
//	cout << "Indices:  " << indices->size() << endl;

//	for( int i = 0; i < indices->size(); i++ )
//	{
//		if( i%3 == 0 && i != 0 )
//			cout << endl;
//		cout << indices[i] << " ";

//	}

	/*int c;
	cin >> c;*/
	return;
}