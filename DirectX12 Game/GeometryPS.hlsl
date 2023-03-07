#include"Common.hlsl"
Texture2D<float4> modelAlbedTexture : register(t7);
Texture2D<float4> modelNormalTexture : register(t8);
Texture2D<float4> modelMetalTexture : register(t9);
Texture2D<float4> modelEmmisionTexture : register(t10);
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
    
    output.Normal = input.Normal;
    output.Diffuse.rgb = modelAlbedTexture.Sample(sampler0, input.TexCoord).rgb * input.Diffuse.rgb;
    output.Diffuse.a = 1;
	output.Position = input.WorldPosition;
	output.Depth = input.Depth;
	output.ReflectRate = ReflectRate;
	

	return output;

}
