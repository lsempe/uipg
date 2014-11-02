float4x4	view;
float4x4	projection;
float4x4	world;
float4		solidColor = float4(1,1,1,1);

struct SInputData
{
	float4 position : POSITION;
    float4 color    : COLOR0;
    float2 texCoord : TEXCOORD0;   
};

struct SVertexData
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float2 texCoord : TEXCOORD0;    
};

SVertexData vs_default(	SInputData input )
{
	SVertexData output;
    //output.position = mul(projection, mul(view, mul(world, input.position)));

	float4x4 wvp = mul(projection, mul(view, world));
	output.position = mul(wvp, input.position);
    output.texCoord = input.texCoord;    
    output.color = min(solidColor, input.color);
	return output;
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 ps_solidcolor(SVertexData input) : SV_Target0
{
	return solidColor;
};

float4 ps_vertexcolor(SVertexData input) : SV_Target0
{
	return input.color;
};

float4 ps_texture(SVertexData input) : SV_Target0
{
	return texture0.Sample(sampler0, input.texCoord);
};