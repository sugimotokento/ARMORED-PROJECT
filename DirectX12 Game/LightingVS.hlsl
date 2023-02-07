cbuffer cbTansMatrix : register(b0)
{
    float4x4 WVP;
    float4x4 WVPLight;
    float4x4 VP;
    float4x4 World;
    float4 BloomColor;
    float4 ReflectRate;
    float3 cameraPosition;
    float dammy;
};



struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float4 Diffuse : COLOR;
    float2 TexCoord : TEXCOORD;

};
struct PS_INPUT
{
    float4 Position      : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal        : NORMAL;
    float4 Diffuse       : COLOR;
    float4 Depth         : DEPTH;
    float2 TexCoord      : TEXCOORD;
};


PS_INPUT Main(VS_INPUT input)
{
    PS_INPUT output;

    float4 position = float4(input.Position, 1.0f);
    output.Position = mul(position, WVP);

    float4 normal = float4(input.Normal, 0.0f);
    output.Normal = mul(normal, World);

    output.TexCoord = input.TexCoord;

    output.Diffuse.rgb = 1.0;
    output.Diffuse.a = 1.0;

    return output;
}

