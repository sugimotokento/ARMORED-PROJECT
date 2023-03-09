cbuffer CBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 WVPLight;
    float4x4 VP;
    float4x4 World;
    float4 ReflectRate;
    float3 cameraPosition;
    bool Dammy;
    float4 WaterParam;
}


struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
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
    float3 Bitangent : BITANGENT;

    float4 LightPosition : POSITION1;
    float4 LightDepth : DEPTH2;
};

float3 GetLightDir()
{
    float3 dir = float3(0.3, -1, 0.3);
    dir = normalize(dir);
    
    return dir;
}


float GetMetalRatio(float4 metalMap)
{
    float specularRatio = 1;
    
    metalMap.r = min(1, metalMap.r);
    metalMap.g = min(1, metalMap.g);
    metalMap.b = min(1, metalMap.b);
    metalMap.r = max(0, metalMap.r);
    metalMap.g = max(0, metalMap.g);
    metalMap.b = max(0, metalMap.b);
    specularRatio = (metalMap.r - (metalMap.g + metalMap.b) * 0.5);
    
    return specularRatio  * specularRatio*0.8;

}
