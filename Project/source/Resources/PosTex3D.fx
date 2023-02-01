//------------------------------------------------
// Globals
//------------------------------------------------
float gPI = 3.14159265359f;
float gLightIntensity = 4.0f;
float3 gLightDirection = normalize(float3(-0.40825f, -0.40825f, 0.8165));
float4 gAmbientColor = float4(0.025f, 0.025f, 0.025f, 1.0f);

float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorld : WorldMatrix;

Texture2D gDiffuseMap : DiffuseMap;

SamplerState gSamState : SampleState
{
	Filter = ANISOTROPIC;
	AddressU = Wrap; // or Mirror, Clamp, Border
	AddressV = Wrap; // or Mirror, Clamp, Border
};

//------------------------------------------------
// Input/Output Struct
//------------------------------------------------
struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: W_POSITION;
	float3 Normal			: NORMAL;
	float2 UV				: TEXCOORD;
};

//------------------------------------------------
// Vertex Shader
//------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
	output.Normal = mul(normalize(input.Normal), (float3x3)gWorld);
	output.UV = input.UV;
	return output;
}

//------------------------------------------------
// BRDF Calculation
//------------------------------------------------
float4 CalculateLambert(float4 cd)
{
	return cd / gPI;
}

//------------------------------------------------
// Pixel Shader
//------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float observedArea = dot(input.Normal, -gLightDirection) / 2.0f + 0.5f;
	float4 lambert = CalculateLambert(gDiffuseMap.Sample(gSamState, input.UV));

	return gLightIntensity * lambert * observedArea + gAmbientColor;
}

//------------------------------------------------
// Technique
//------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}