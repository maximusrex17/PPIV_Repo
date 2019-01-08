// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>

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

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)

			myDevice->CreateVertexShader(myVertexShaderBlob->GetBufferPointer(), myVertexShaderBlob->GetBufferSize(), nullptr, &myVertexShader);

			// Layouts
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			
			UINT numElements = ARRAYSIZE(layout);

			myDevice->CreateInputLayout(layout, numElements, myVertexShaderBlob->GetBufferPointer(), myVertexShaderBlob->GetBufferSize(), &myInputLayout);
			myDevice->CreatePixelShader(myPixelShaderBlob->GetBufferPointer(), myPixelShaderBlob->GetBufferSize(), nullptr, &myPixelShader);
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
	myVertexShaderBlob ->Release();
	myPixelShaderBlob ->Release();
	// TODO: "Release()" more stuff here!


	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

struct Vertex {
	float pos[4];
	float color[4];
};

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;

		Vertex tri[] = {
			{ {0.0f, 0.5f, 0.0f, 1.0f},{1,1,1,1} },
			{ {0.5f, -0.5f, 0.0f, 1.0f},{1,1,1,1} },
			{ {-0.5f, -0.5f, 0.0f, 1.0f},{1,1,1,1} }
		};



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

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);
			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;

			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(bDesc));
			
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(tri) * 3;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			
			subData.pSysMem = tri;

			myDevice->CreateBuffer(&bDesc, &subData, &myVertexBuffer);

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}