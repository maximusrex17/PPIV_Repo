struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 UV : COLOR;
};

cbuffer ConstantBuffer : register(b0) {
	matrix vsWorld;
	matrix vsView;
	matrix vsProjection;
	float4 vsColor;
}

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	return (1,0,0,1);
}