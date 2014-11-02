float4x4 view;
float4x4 projection;
float4x4 world;
float4 color;

struct Output
{
    float4 Position : SV_Position;
    float4 Color    : COLOR0;
    float2 TexCoord : TEXCOORD0;
	float4 Normal	: NORMAL0;
};

void main(in float4 Position : POSITION, 
		  in float4 Color : COLOR0, 
		  in float2 TexCoord : TEXCOORD0,
          in float3 Normal : NORMAL,
          out Output ov)
{
    ov.Position = mul(projection, mul(view, mul(world, Position)));
    ov.TexCoord = TexCoord;    
    ov.Color = color;
};