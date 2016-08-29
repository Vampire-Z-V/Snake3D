#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight g_DirLights[3];
	float3 g_EyePosW;
};

cbuffer cbPerObject
{
	float4x4 g_World;
	float4x4 g_WorldInvTranspose;
	float4x4 g_WorldViewProj;
	float4x4 g_TexTransform;

	Material g_Material;
};

Texture2D g_DiffuseMap;
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};

/****** Function ***************************************/

// Vetex Shader
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), g_World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)g_WorldInvTranspose);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_WorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), g_TexTransform).xy;

	return vout;
}

// Pixel Shader
float4 PS(VertexOut pin, 
		  uniform int g_LightCount, uniform bool g_AlphaClip) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = g_EyePosW - pin.PosW;
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
	
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
	// Sample texture.
	texColor = g_DiffuseMap.Sample( samAnisotropic, pin.Tex );

	if(g_AlphaClip)
	{
		// Discard pixel if texture alpha < 0.1.  
		clip(texColor.a - 0.1f);
	}
	
	//
	// Lighting.
	//

	float4 litColor = texColor;
	if( g_LightCount > 0  )
	{  
		// Initialize
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < g_LightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(g_Material, g_DirLights[i], pin.NormalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		litColor = texColor*(ambient + diffuse) + spec;
	}

	// Take alpha from diffuse material and texture.
	litColor.a = g_Material.Diffuse.a * texColor.a;

    return litColor;
}

/****** Technique ***************************************/

technique11 Light3Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
	}
}

technique11 Light3TexAlphaClip
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
	}
}
