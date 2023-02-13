cbuffer CBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 WVPLight;
    float4x4 VP;
    float4x4 World;
    float4 ReflectRate;
    float3 cameraPosition;
    bool dammy;
}


struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
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

    float4 LightPosition : POSITION1;
    float4 LightDepth : DEPTH2;
};




