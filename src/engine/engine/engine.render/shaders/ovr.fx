
Texture2D Texture : register(t0);
SamplerState Linear : register(s0);

cbuffer Parameters : register(b0)
{
	float2 LensCenter		: packoffset(c0);
	float2 ScreenCenter		: packoffset(c1);
	float2 Scale			: packoffset(c2);
	float2 ScaleIn			: packoffset(c3);
	float4 DistortionAmount : packoffset(c20);

	float4x4 view : packoffset(c4);
	float4x4 world: packoffset(c8);
	float4x4 projection : packoffset(c12);
	float4x4 Texm : packoffset(c16);


}


void OVR_VertexShader(in float3 Position : POSITION,
					  in float4 Color : COLOR0,
					  in float2 TexCoord : TEXCOORD0,
					  out float4 oPosition : SV_Position,
					  out float4 oColor : COLOR,
					  out float2 oTexCoord : TEXCOORD0)
{

	oPosition = mul(projection, mul(view, mul(world, float4(Position,1))));
    oTexCoord = TexCoord;    
    oColor = Color;
}

float2 DistortionModel(float2 inTexCoord)
{
	float2 theta = (inTexCoord - LensCenter) * ScaleIn; // scales [-1,1]
	float rSq = theta.x * theta.x + theta.y;
	float2 theta1 = theta * (DistortionAmount.x + DistortionAmount.y * rSq + DistortionAmount.z * rSq * rSq + DistortionAmount.w * rSq * rSq * rSq);
	return LensCenter + Scale * theta1;
}

float4 DistortionCorrectionPixelShader(in float4 oPosition : SV_Position,
										   in float4 oColor : COLOR,
										   in float2 oTexCoord : TEXCOORD0) : SV_Target0
{
	float2 tc = DistortionModel(oTexCoord);
	if ( any(clamp(tc, ScreenCenter - float2(0.25, 0.5), ScreenCenter + float2(0.25, 0.5)) - tc));
		return 0;
	
	return Texture.Sample(Linear, tc);
}
