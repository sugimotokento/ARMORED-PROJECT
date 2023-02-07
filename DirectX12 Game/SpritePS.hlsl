Texture2D<float4> mainTexture : register(t0);



SamplerState sampler0 : register(s0);

cbuffer CBuffer : register(b0)
{
	float4x4 WVP;
	float4x4 WVPLight;
	float4x4 VP;
	float4x4 World;

}


struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL;
	float4 Diffuse : COLOR;
	float4 Depth : DEPTH;
	float2 TexCoord : TEXCOORD;
};




float4 Main(PS_INPUT input) : SV_TARGET
{

	float4 color = mainTexture.Sample(sampler0, 1-input.TexCoord);
	if (color.r <=0.45 && color.b<= 0.45) {
		color.a = 1 - color.g;//—ÎF‚ð“§‰ß‚·‚é
	}
	color = color * input.Diffuse;

	return color;
}
