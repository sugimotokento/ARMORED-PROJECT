#include"Common.hlsl"
Texture2D<float4> modelAlbedTexture : register(t7);
Texture2D<float4> modelNormalTexture : register(t8);
Texture2D<float4> modelOcclusionTexture : register(t9);
Texture2D<float4> modelMetalTexture : register(t10);


SamplerState sampler0 : register(s0);

struct PS_OUTPUT
{
    float4 Normal : SV_TARGET0;
    float4 Diffuse : SV_TARGET1;
    float4 Position : SV_TARGET2;
    float4 Depth : SV_TARGET3;
    float4 ReflectRate : SV_TARGET4;

};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
    
    float3 normal = modelNormalTexture.Sample(sampler0, input.TexCoord).rgb;
    normal = normalize(normal);
    normal = (normal - 0.5) * 2;
    
    float3 finalNormal =
            input.Tangent * normal.x +
            input.Binormal * normal.y +
            input.Normal.xyz * normal.z;
    finalNormal = normalize(finalNormal);
    
    output.Normal = float4(finalNormal, 1);
    float3 albed = modelAlbedTexture.Sample(sampler0, input.TexCoord).rgb;
    float3 occlusion = modelOcclusionTexture.Sample(sampler0, input.TexCoord).rgb;
    output.Diffuse.rgb = albed * occlusion * input.Diffuse.rgb;
    output.Diffuse.a = 1;
	output.Position = input.WorldPosition;
	output.Depth = input.Depth;
    
    float4 metalMap = modelMetalTexture.Sample(sampler0, input.TexCoord);
    output.ReflectRate = GetMetalRatio(metalMap)*0.5;
	

	return output;

}
