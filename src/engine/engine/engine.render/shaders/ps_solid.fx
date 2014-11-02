Texture2D Texture : register(t0);
SamplerState Linear : register(s0);

struct Input
{
    float4 Position : SV_Position;
    float4 Color    : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

float4 main(in Input i) : SV_Target
{
	return Texture.Sample(Linear, i.TexCoord) * i.Color;
};