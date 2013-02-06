#include <fbxsdk.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <direct.h>
#include "Vertex.h"
using namespace std;

void DisplayContent(KFbxNode* pNode);
void DisplayContent(FbxScene* pScene);
void Import(vector<XMFLOAT3> *vec, vector<WORD> *ind, vector<XMFLOAT3> *norm);

#define FBXSDK_NEW_API