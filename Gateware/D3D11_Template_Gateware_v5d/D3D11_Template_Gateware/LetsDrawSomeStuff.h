// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include"VertexShader_PPIV.csh"
#include"PixelShader_PPIV.csh"
#include "MathFunctions.h"
#include "VertexShader.h"
#include <directxmath.h>
#include <vector>
// FBX includes
#include "DDSTextureLoader.h"
#include <fbxsdk.h>
#include <iostream>

using namespace std;
using namespace DirectX;

struct ConstantBuffer
{
	XMMATRIX cWorld;
	XMMATRIX cView;
	XMMATRIX cProjection;
	XMFLOAT4 cColor;
};

//Vectors for Objects
vector<Vertex> cube;
vector<unsigned int> cubeIndicies;

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
	ID3D11PixelShader *myPixelShader = nullptr;
	ID3DBlob *myVertexShaderBlob = nullptr;
	ID3DBlob *myPixelShaderBlob = nullptr;
	ID3D11Buffer *myVertexBuffer = nullptr;
	ID3D11Buffer *myIndexBuffer = nullptr;
	ID3D11Buffer *myConstantBuffer = nullptr;
	D3D11_VIEWPORT myPort;
	
	//Matricies
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	

public:

	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

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

					temp.pos[0] = (float)pos.mData[0] * scale;
					temp.pos[1] = (float)pos.mData[1] * scale;
					temp.pos[2] = (float)pos.mData[2] * scale;
					temp.pos[3] = 1.0f;

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

			cube = verts;
			cubeIndicies = indicies;

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

							FbxProperty p = texture->RootProperty.Find("Assets/box.fbx");

						}
					}
				}
			}

			ProcessFbxMesh(childNode, verts, indicies);
		}
	}
}

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

			//Create Pixel Shader
			myDevice->CreatePixelShader(PixelShader_PPIV, sizeof(PixelShader_PPIV), nullptr, &myPixelShader);

			//Create Input Layout
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			myDevice->CreateInputLayout(ieDesc, ARRAYSIZE(ieDesc), VertexShader_PPIV, sizeof(VertexShader_PPIV), &myInputLayout);

			//Vertex tri[] = {
			//	{ {0.0f, 0.5f, 0.0f, 1.0f}},
			//	{ {0.5f, -0.5f, 0.0f, 1.0f}, {0.0f,1,0,1} },
			//	{ {-0.5f, -0.5f, 0.0f, 1.0f}, {0.0f,0,1,1} }
			//	//{ (-0.5f, 0.5f, -0.5f, 0.5f),  (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, -0.5f, 0.5f),   (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, 0.5f, 0.5f),    (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (-0.5f, 0.5f, 0.5f, 0.5f),   (1.0f, 1.0f, 0.0f, 1.0f) },
			//	//
			//	//{ (-0.5f, -0.5f, -0.5f, 0.5f), (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, -0.5f, -0.5f, 0.5f),  (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, -0.5f, 0.5f, 0.5f),   (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (-0.5f, -0.5f, 0.5f, 0.5f),  (1.0f, 1.0f, 0.0f, 1.0f) },
			//	//
			//	//{ (-0.5f, -0.5f, 0.5f, 0.5f),  (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (-0.5f, -0.5f, -0.5f, 0.5f), (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (-0.5f, 0.5f, -0.5f, 0.5f),  (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (-0.5f, 0.5f, 0.5f, 0.5f),   (1.0f, 1.0f, 0.0f, 1.0f) },
			//	//
			//	//{ (0.5f, -0.5f, 0.5f, 0.5f),   (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, -0.5f, -0.5f, 0.5f),  (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, -0.5f, 0.5f),   (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, 0.5f, 0.5f),    (1.0f, 1.0f, 0.0f, 1.0f) },
			//	//
			//	//{ (-0.5f, -0.5f, -0.5f, 0.5f), (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, -0.5f, -0.5f, 0.5f),  (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, -0.5f, 0.5f),   (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (-0.5f, 0.5f, -0.5f, 0.5f),  (1.0f, 1.0f, 0.0f, 1.0f) },
			//	//
			//	//{ (-0.5f, -0.5f, 0.5f, 0.5f),  (1.0f, 0.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, -0.5f, 0.5f, 0.5f),   (0.0f, 1.0f, 0.0f, 1.0f) },
			//	//{ (0.5f, 0.5f, 0.5f, 0.5f),    (0.0f, 0.0f, 1.0f, 1.0f) },
			//	//{ (-0.5f, 0.5f, 0.5f, 0.5f),   (1.0f, 1.0f, 0.0f, 1.0f) },
			//};

			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;

			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			//Vertex Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(Vertex) * 3;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			subData.pSysMem = tri;
			hr = myDevice->CreateBuffer(&bDesc, &subData, &myVertexBuffer);


			//WORD indices[] =
			//{
			//	3,1,0,
			//	2,1,3,

			//	6,4,5,
			//	7,4,6,

			//	11,9,8,
			//	10,9,11,

			//	14,12,13,
			//	15,12,14,

			//	19,17,16,
			//	18,17,19,

			//	22,20,21,
			//	23,20,22
			//};

			////Index Buffer
			//bDesc.Usage = D3D11_USAGE_DEFAULT;
			//bDesc.ByteWidth = sizeof(WORD) * 36;
			//bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			//bDesc.CPUAccessFlags = 0;
			//subData.pSysMem = indices;
			//myDevice->CreateBuffer(&bDesc, &subData, &myIndexBuffer);

			//Constant Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.CPUAccessFlags = 0;
			hr = myDevice->CreateBuffer(&bDesc, nullptr, &myConstantBuffer);

			//World Matrix
			worldMatrix = XMMatrixIdentity();

			//View Matrices
			XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
			XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);


			//Projection Matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(15.0f), ratio, 0.1f, 100.0f);


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
	myVertexShader->Release();
	myPixelShader->Release();
	// TODO: "Release()" more stuff here!
	//delete &WorldMatrix;
	//delete &ProjectionMatrix;

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}


// Draw
void LetsDrawSomeStuff::Render()
{
	float timeSpent = 0;
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
			const float black[] = { 0, 0, 0.5f, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, black);
			
			timeSpent += 0.5f;

			curDeg = UpdateDeg(timeSpent, curDeg);

			//Rotate the cube around the origin
			worldMatrix = XMMatrixRotationY(curDeg);
						
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
			//myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

			
			//Set Constant Buffer
			ConstantBuffer constBuff;
			constBuff.cWorld = XMMatrixTranspose(worldMatrix);
			constBuff.cView = XMMatrixTranspose(viewMatrix);
			constBuff.cProjection = XMMatrixTranspose(projectionMatrix);
			constBuff.cColor = XMFLOAT4(1, 1, 1, 1);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

		//TODO: Render Cube
			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Set Pixel Shader and Pixel Constant Buffer
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);			
			
			//Draw Cube
			myContext->Draw(3, 0);

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.




			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();

		}
	}
}