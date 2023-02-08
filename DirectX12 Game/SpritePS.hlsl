#include"Common.hlsl"

Texture2D<float4> mainTexture : register(t0);

SamplerState sampler0 : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{

	float4 color = mainTexture.Sample(sampler0, 1-input.TexCoord);
	if (color.r <=0.45 && color.b<= 0.45) {
		color.a = 1 - color.g;//—ÎF‚ð“§‰ß‚·‚é
	}
	color = color * input.Diffuse;

	return color;
}
