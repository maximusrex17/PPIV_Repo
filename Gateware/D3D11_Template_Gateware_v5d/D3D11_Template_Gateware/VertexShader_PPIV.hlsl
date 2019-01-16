//Rule of Three
//Three things must match
//1. C++ Vetex Struct
//2. Input Layout
//3. HLSL Vertex Struct
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0) {
	matrix vsWorld;
	matrix vsView;
	matrix vsProjection;
	matrix vsRotateY;
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
	output.norm = mul(float4(input.norm, 1), vsWorld).xyz;
	output.uv = input.uv;
	return output;
}