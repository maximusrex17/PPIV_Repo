//Rule of Three
//Three things must match
//1. C++ Vetex Struct
//2. Input Layout
//3. HLSL Vertex Struct

cbuffer ConstantBuffer : register(b0) {
	matrix vsWorld;
	matrix vsView;
	matrix vsProjection;
	float4 vsLightDir[2];
	float4 vsLightColor[2];
	float4 vsOutputColor;
}

struct InputVertex {
	float4 pos : POSITION;
	float2 uv : UV;
	float3 norm : NORMAL;
};

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
};



OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.pos = mul(input.pos, vsWorld);
	output.pos = mul(output.pos, vsView);
	output.pos = mul(output.pos, vsProjection);
	
	return output;
}