struct InputVertex
{
	float3 xyz : POSITION;
	float3 nrm : NORMAL;
	float3 uvw : TEXCOORD;
};

struct OutputVertex
{
	float4 xyzw : SV_POSITION; // system value
	float4 uvws : OTEXTURE;
	float4 nrms : ONORMAL;
	float4 rgba : OCOLOR;
	float4 worldpos : WORLD;
};

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

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.xyzw = float4(input.xyz, 1);
	// Do math here (shader intrinsics)

	output.worldpos = output.xyzw;
	output.xyzw = mul(worldMatrix, output.xyzw);
	output.xyzw = mul(viewMatrix, output.xyzw);
	output.xyzw = mul(projectionMatrix, output.xyzw);
	// don't do perspective divide

	return output;
}