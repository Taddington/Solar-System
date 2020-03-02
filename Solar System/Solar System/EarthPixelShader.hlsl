Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
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
	bool toNight = false;
	float pointAttenuation = 1.0f - saturate(length(pointLightPos - inputPixel.worldpos) / pointLightRadius.x);
	float4 pointLightDirection = normalize(pointLightPos - inputPixel.worldpos);
	float pointLightRatio = saturate(dot(pointLightDirection, inputPixel.nrms) * pointAttenuation);
	float4 pointResult = pointLightRatio * pointLightColor;

	float4 finalColor = 0;
	finalColor += pointResult;
	if (finalColor.x <= 0 && finalColor.y <= 0 && finalColor.z <= 0 && finalColor.w <= 0)
	{
		toNight = true;
	}

	finalColor = saturate(finalColor + float4(0.5f, 0.5f, 0.5f, 0.0f));

	if (toNight)
	{
		finalColor *= tex2.Sample(samp, inputPixel.uvws);
	}
	else
	{
		finalColor *= tex.Sample(samp, inputPixel.uvws);
	}
	finalColor.a = 1;
	return finalColor;
}