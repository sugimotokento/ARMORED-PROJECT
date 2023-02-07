
cbuffer cbBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 WVPLight;
    float4x4 VP;
    float4x4 World;
    float4 BloomColor;
    float4 ReflectRate;
    float3 cameraPosition;
    float dammy;
}


struct PS_INPUT
{
    float4 Position      : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal        : NORMAL;
    float4 Diffuse       : COLOR;
    float4 Depth         : DEPTH;
    float2 TexCoord      : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 Depth      : SV_TARGET0;


};


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    output.Depth = input.Depth;
    return output;

}
