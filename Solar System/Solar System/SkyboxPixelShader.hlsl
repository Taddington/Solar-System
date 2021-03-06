textureCUBE tex : register(t0);
SamplerState samp : register(s0);

cbuffer Shader_Vars : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4 time;
	float4 pointLightPos;
	float4 pointLightColor;
	float4 pointLightRadius;
};

struct OutputVertex
{
	float4 xyzw : SV_POSITION; // system value
	float4 uvws : OTEXTURE;
	float4 nrms : ONORMAL;
	float4 rgba : OCOLOR;
	float4 worldpos : WORLD;
};

float4 main(OutputVertex inputPixel) : SV_TARGET
{
	return tex.Sample(samp, inputPixel.worldpos);
}