// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include"VertexShader_PPIV.csh"
#include"PixelShader_PPIV.csh"
#include"LightVertexShader_PPIV.csh"
#include"LightPixelShader_PPIV.csh"
#include "VertexShader.h"
#include <directxmath.h>
#include <vector>
// FBX includes
#include "DDSTextureLoader.h"
#include "fbxsdk.h"
#include <iostream>

using namespace std;
using namespace DirectX;

#define RAND_NORMAL XMFLOAT3(rand()/float(RAND_MAX),rand()/float(RAND_MAX),rand()/float(RAND_MAX))

struct Vertex {
	float pos[4];
	float UV[2];
	float norm[3];
};

struct ConstantBuffer
{
	XMMATRIX cWorld;
	XMMATRIX cView;
	XMMATRIX cProjection;
	XMMATRIX cRotateY;
	XMFLOAT4 cLightDir[2];
	XMFLOAT4 cLightColor[2];
	XMFLOAT4 cOutputColor;
};

//Vectors for Objects
vector<Vertex> cube;
vector<unsigned  int> cubeIndicies;

XMMATRIX worldMatrix;
XMMATRIX viewMatrix;
XMMATRIX projectionMatrix;
XMMATRIX ReturnViewMatrix;
XMFLOAT4 LightDir[2];
XMFLOAT4 LightColor[2];
XMFLOAT4 OutputColor;

float timeSpent = 0, curDeg = 0;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	ID3D11InputLayout *myInputLayout = nullptr;
	ID3D11VertexShader *myVertexShader = nullptr;
	ID3D11VertexShader *myVertexLightShader = nullptr;
	ID3D11PixelShader *myPixelShader = nullptr;
	ID3D11PixelShader *myLightPixelShader = nullptr;
	ID3D11Buffer *myVertexBuffer = nullptr;
	ID3D11Buffer *myIndexBuffer = nullptr;
	ID3D11Buffer *myConstantBuffer = nullptr;
	D3D11_VIEWPORT myPort; 
	D3D_DRIVER_TYPE myDriverType = D3D_DRIVER_TYPE_NULL;

	
	//Matricies

	ID3D11ShaderResourceView* myShaderResource = nullptr;
	ID3D11SamplerState* mySampler = nullptr;
	

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

FbxVector2 GetVertexUV(FbxGeometryElementUV* uvPtr, unsigned int VertexID, unsigned int controlPointIndex)
{
	FbxLayerElement::EMappingMode mapMode = uvPtr->GetMappingMode();
	switch (mapMode)
	{
	case FbxGeometryElement::EMappingMode::eByControlPoint:
	{
		FbxLayerElement::EReferenceMode refMode = uvPtr->GetReferenceMode();
		switch (refMode)
		{
		case FbxLayerElement::EReferenceMode::eDirect:
		{
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(controlPointIndex);
			return uv;
		}
		break;

		case FbxLayerElement::EReferenceMode::eIndexToDirect:
		{
			int id = uvPtr->GetIndexArray().GetAt(controlPointIndex);
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(id);
			return uv;
		}
		break;

		default:
			break;
		}
	}
	break;
	case FbxGeometryElement::EMappingMode::eByPolygonVertex:
	{
		FbxLayerElement::EReferenceMode refMode = uvPtr->GetReferenceMode();
		switch (refMode)
		{
		case FbxLayerElement::EReferenceMode::eDirect:
		{
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(VertexID);
			return uv;
		}
		break;

		case FbxLayerElement::EReferenceMode::eIndexToDirect:
		{
			int id = uvPtr->GetIndexArray().GetAt(VertexID);
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(id);
			return uv;
		}
		break;

		default:
			break;
		}
	}
	break;
	default:
		break;
	}

	return FbxVector2(0.0f, 0.0f);
}

void ProcessFbxMesh(FbxNode* Node, vector<Vertex>& verts, vector<unsigned int>& indicies)
{
	float scale = 40.0f;
	//FBX Mesh stuff
	int childrenCount = Node->GetChildCount();

	for (int a = 0; a < childrenCount; a++)
	{
		FbxNode *childNode = Node->GetChild(a);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			const char* name = mesh->GetName();

			FbxVector4* lControlPoints = mesh->GetControlPoints();
			const int polyCount = mesh->GetPolygonCount();

			int VertexId = 0;
			for (int i = 0; i < polyCount; i++)
			{
				int polygonSize = mesh->GetPolygonSize(i);
				for (int j = 0; j < polygonSize; j++)
				{
					// create a temporary Vertex
					Vertex temp;

					int controlPointIndex = mesh->GetPolygonVertex(i, j);
					FbxVector4 pos = lControlPoints[controlPointIndex];

					temp.pos[0] = (float)pos.mData[0] / scale;
					temp.pos[1] = (float)pos.mData[1] / scale;
					temp.pos[2] = (float)pos.mData[2] / scale;
					temp.pos[3] = 1.0f;
					
					// Get the Normals array from the mesh
					FbxArray<FbxVector4> normalsVec;
					mesh->GetPolygonVertexNormals(normalsVec);
					cout << "\nNormalVec Count:" << normalsVec.Size();

					temp.norm[0] = normalsVec.GetAt(j).mData[0];
					temp.norm[1] = normalsVec.GetAt(j).mData[1];
					temp.norm[2] = normalsVec.GetAt(j).mData[2];
					
					for (int k = 0; k < mesh->GetElementUVCount(); k++)
					{
						FbxGeometryElementUV *fbxUV = mesh->GetElementUV(k);
						int textIndex = mesh->GetTextureUVIndex(i, j);

						FbxVector2 uv = GetVertexUV(fbxUV, VertexId, controlPointIndex);
						temp.UV[0] = (float)uv.mData[0];
						temp.UV[1] = 1.0f - (float)uv.mData[1];						
					}

					verts.push_back(temp);
					indicies.push_back(VertexId);

					VertexId++;
				}
			}

			//=======================Texture===============================================
			int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();

			for (int index = 0; index < materialCount; index++)
			{
				FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)childNode->GetSrcObject<FbxSurfaceMaterial>(index);

				if (material != NULL)
				{
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

					// Check if it's layeredtextures
					int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

					if (layeredTextureCount > 0)
					{
						for (int j = 0; j < layeredTextureCount; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();

							for (int k = 0; k < lcount; k++)
							{
								FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								const char* textureName = texture->GetFileName();
							}
						}
					}
					else
					{
						// Directly get textures
						int textureCount = prop.GetSrcObjectCount<FbxTexture>();
						for (int j = 0; j < textureCount; j++)
						{
							FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(j));
							// Then, you can get all the properties of the texture, include its name

							FbxProperty p = texture->RootProperty.Find("Crate");

						}
					}
				}
			}

			ProcessFbxMesh(childNode, verts, indicies);
		}
	}
}

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	HRESULT hr = S_OK;
	if (attatchPoint) // valid window?
	{
		//unsigned int width;
		//UINT height;
		//attatchPoint->GetWidth(width);
		//attatchPoint->GetHeight(height);
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			float ratio;
			mySurface->GetAspectRatio(ratio);
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)

			//Create Vertex Shader
			myDevice->CreateVertexShader(VertexShader_PPIV, sizeof(VertexShader_PPIV), nullptr, &myVertexShader);
			myDevice->CreateVertexShader(LightVertexShader_PPIV, sizeof(LightVertexShader_PPIV), nullptr, &myVertexLightShader);

			//Create Pixel Shader
			myDevice->CreatePixelShader(PixelShader_PPIV, sizeof(PixelShader_PPIV), nullptr, &myPixelShader);
			myDevice->CreatePixelShader(LightPixelShader_PPIV, sizeof(LightPixelShader_PPIV), nullptr, &myLightPixelShader);

			// Change the following filename to a suitable filename value.
			const char* lFilename = "Assets/cube.fbx";

			// Initialize the SDK manager. This object handles memory management.
			FbxManager* lSdkManager = FbxManager::Create();

			// Create the IO settings object.
			FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
			lSdkManager->SetIOSettings(ios);

			// Create an importer using the SDK manager.
			FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

			// Use the first argument as the filename for the importer.
			if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
				printf("Call to FbxImporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
				exit(-1);
			}
			// Create a new scene so that it can be populated by the imported file.
			FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

			// Import the contents of the file into the scene.
			lImporter->Import(lScene);

			// The file is imported, so get rid of the importer.
			lImporter->Destroy();

			// Process the scene and build DirectX Arrays
			ProcessFbxMesh(lScene->GetRootNode(), cube, cubeIndicies);

			//Create Input Layout
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = myDevice->CreateInputLayout(ieDesc, ARRAYSIZE(ieDesc), VertexShader_PPIV, sizeof(VertexShader_PPIV), &myInputLayout);

			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;

			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			//Vertex Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(Vertex) * cube.size();
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			subData.pSysMem = cube.data();
			hr = myDevice->CreateBuffer(&bDesc, &subData, &myVertexBuffer);

			//Index Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(unsigned int) * cubeIndicies.size();
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = cubeIndicies.data();
			myDevice->CreateBuffer(&bDesc, &subData, &myIndexBuffer);

			//Constant Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.CPUAccessFlags = 0;
			hr = myDevice->CreateBuffer(&bDesc, nullptr, &myConstantBuffer);

			hr = CreateDDSTextureFromFile(myDevice, L"Assets/cubeTexture.dds", nullptr, &myShaderResource);

			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = myDevice->CreateSamplerState(&sampDesc, &mySampler);

			//World Matrix
			worldMatrix = XMMatrixIdentity();

			//View Matrices
			XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
			XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			ReturnViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
			viewMatrix = ReturnViewMatrix;


			//Projection Matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(50.0f), ratio, 0.1f, 100.0f);


		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();
	myInputLayout->Release();
	myVertexLightShader->Release();
	myVertexShader->Release();
	myPixelShader->Release();
	myLightPixelShader->Release();
	myVertexBuffer->Release();
	myIndexBuffer->Release();
	myConstantBuffer->Release();
	myShaderResource->Release();
	mySampler->Release();

	// TODO: "Release()" more stuff here!
	//delete &WorldMatrix;
	//delete &ProjectionMatrix;

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

float UpdateDeg(float time, float deg) {
	float newDeg = deg + time;
	return newDeg;
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{		
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;

		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below

			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);
			//Clear Screen to Black
			const float black[] = { 0.2f, 0.2f, 0.2f, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, black);
			
			//timeSpent += 0.000001f;
			//curDeg += UpdateDeg(timeSpent, curDeg);
			
			if (myDriverType == D3D_DRIVER_TYPE_REFERENCE)
			{
				curDeg += (float)XM_PI * 0.0125f;
			}
			else
			{
				static ULONGLONG timeStart = 0;
				ULONGLONG timeCur = GetTickCount64();
				if (timeStart == 0)
					timeStart = timeCur;
				curDeg = (timeCur - timeStart) / 1000.0f;
			}

			//Rotate the cube around the origin
			worldMatrix = XMMatrixRotationY(curDeg);


			// Setup our lighting parameters
			XMFLOAT4 myLightDirs[] =
			{
				XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
			};
			XMFLOAT4 myLightColors[2] =
			{
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
			};

			//TODO: Change Second light color

			//Press '1' to make light White 
			if (GetAsyncKeyState(0x31)) {
				myLightColors[1] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//Press '2' to make light Red
			if (GetAsyncKeyState(0x32)) {
				myLightColors[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//Press '3' to make light Green 
			if (GetAsyncKeyState(0x33)) {
				myLightColors[1] = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			}

			//Press '4' to make light Blue
			if (GetAsyncKeyState(0x34)) {
				myLightColors[1] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			}

			//Mix Red and Green
			if (GetAsyncKeyState(0x32) && GetAsyncKeyState(0x33)) {
				myLightColors[1] = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			}

			//Mix Green and Blue
			if (GetAsyncKeyState(0x33) && GetAsyncKeyState(0x34)) {
				myLightColors[1] = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
			}

			//Mix Red and Blue
			if (GetAsyncKeyState(0x32) && GetAsyncKeyState(0x34)) {
				myLightColors[1] = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
			}

			//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'

			//W Key: Forward
			if (GetAsyncKeyState(0x57)) {
				viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
			}

			//A Key: Left
			if (GetAsyncKeyState(0x41)) {
				viewMatrix *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
			}

			//S Key: Backward
			if (GetAsyncKeyState(0x53)) {
				viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
			}

			//D Key: Right
			if (GetAsyncKeyState(0x44)) {
				viewMatrix *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
			}

			//Up Arrow Key: Rise
			if (GetAsyncKeyState(VK_UP)) {
				viewMatrix *= XMMatrixTranslation(0.0f, -0.01f, 0.0f);
			}

			//Down Arrow Key: Fall
			if (GetAsyncKeyState(VK_DOWN)) {
				viewMatrix *= XMMatrixTranslation(0.0f, 0.01f, 0.0f);
			}

			//Left Arrow Key: Rotate Left
			if (GetAsyncKeyState(VK_LEFT)) {
				viewMatrix *= XMMatrixRotationY(0.001f);
			}

			//Right Arrow Key: Rotate Right
			if (GetAsyncKeyState(VK_RIGHT)) {
				viewMatrix *= XMMatrixRotationY(-0.001f);
			}

			//Enter Key: Reset Camera
			if (GetAsyncKeyState(VK_RETURN)) {
				viewMatrix = ReturnViewMatrix;
			}


			// Rotate the second light around the origin
			XMMATRIX mRotate = XMMatrixRotationY(-2 * curDeg);
			XMVECTOR vLightDir = XMLoadFloat4(&myLightDirs[1]);
			vLightDir = XMVector3Transform(vLightDir, mRotate);
			XMStoreFloat4(&myLightDirs[1], vLightDir);
						

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your Vertex & index buffers, Set your InputLayout & Topology & Draw!
						
			//Set Input Layout
			myContext->IASetInputLayout(myInputLayout);

			//Set Primitive Topology
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			UINT strides[] = { sizeof(Vertex) };
			UINT offsets[] = { 0 };
			ID3D11Buffer *tempVB[] = { myVertexBuffer };

			//Set Vertex Buffer
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);

			//Set Index Buffer
			myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			
			//Set Constant Buffer
			ConstantBuffer constBuff;
			constBuff.cWorld = XMMatrixTranspose(worldMatrix);
			constBuff.cView = XMMatrixTranspose(viewMatrix);
			constBuff.cProjection = XMMatrixTranspose(projectionMatrix);
			constBuff.cRotateY = mRotate;
			constBuff.cLightDir[0] = myLightDirs[0];
			constBuff.cLightDir[1] = myLightDirs[1];
			constBuff.cLightColor[0] = myLightColors[0];
			constBuff.cLightColor[1] = myLightColors[1];
			constBuff.cOutputColor = XMFLOAT4(1, 1, 1, 1);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

		//TODO: Render Cube
			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Set Pixel Shader and Pixel Constant Buffer
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);		
			myContext->PSSetShaderResources(0, 1, &myShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);
			
			//Draw Cube
			myContext->DrawIndexed(cubeIndicies.size(), 0, 0);

			XMMATRIX lightMatrix = mRotate;

			myContext->VSSetShader(myVertexLightShader, nullptr, 0);

			for (int m = 0; m < 2; m++)
			{
				XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&myLightDirs[m]));
				XMMATRIX mLightScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
				mLight = mLightScale * mLight;

				// Update the world variable to reflect the current light
				constBuff.cWorld = XMMatrixTranspose(mLight);
				constBuff.cOutputColor = myLightColors[m];
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);
								
				myContext->PSSetShader(myLightPixelShader, nullptr, 0);
				myContext->DrawIndexed(cubeIndicies.size(), 0, 0);
			}
			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			



			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();

		}
	}
}