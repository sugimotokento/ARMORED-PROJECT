cbuffer CBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float3 RadialDir;
    float radialRatio;
}


struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 Diffuse : COLOR;
    float2 TexCoord : TEXCOORD;

};
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal : NORMAL;
    float4 Diffuse : COLOR;
    float4 Depth : DEPTH;
    float2 TexCoord : TEXCOORD;
    
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;

    float4 LightPosition : POSITION1;
    float4 LightDepth : DEPTH2;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 position = float4(input.Position, 1.0f);
    output.Position = mul(position, WVP);

    float4 normal = float4(input.Normal, 0.0f);
    output.Normal = mul(normal, World);

    output.TexCoord = input.TexCoord;

    output.Diffuse = input.Diffuse;

    return output;
}

