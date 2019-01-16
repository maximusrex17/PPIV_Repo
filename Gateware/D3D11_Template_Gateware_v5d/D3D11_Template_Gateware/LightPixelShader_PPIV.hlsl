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

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	float4 finalColor = 0;
	//do NdotL lighting for 2 lights
	for (int i = 0; i<2; i++)
	{
		finalColor += saturate(dot((float3)vsLightDir[i], InputPixel.norm) * vsLightColor[i]);
	}
	finalColor *= txDiffuse.Sample(samLinear, InputPixel.uv);
	finalColor.a = 1;
	return finalColor;
}