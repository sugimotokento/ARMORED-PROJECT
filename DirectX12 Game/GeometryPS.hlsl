Texture2D<float4> shadowTexture :register(t6);
SamplerState sampler0 : register(s0);

cbuffer CBuffer : register(b0)
{
	float4x4 WVP;
	float4x4 WVPLight;
	float4x4 VP;
	float4x4 World;
	float4 BloomColor;
	float4 ReflectRate;
}


struct PS_INPUT
{
	float4 Position      : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal        : NORMAL;
	float4 Diffuse       : COLOR;
	float4 Depth         : DEPTH;
	float2 TexCoord      : TEXCOORD;

	float4 LightPosition : POSITION1;
};

struct PS_OUTPUT
{
	float4 Normal     : SV_TARGET0;
	float4 Diffuse    : SV_TARGET1;
	float4 Position   : SV_TARGET2;
	float4 Depth      : SV_TARGET3;
	float4 BloomColor : SV_TARGET4;
	float4 ReflectRate: SV_TARGET5;

};


float Shadow(PS_INPUT input) {
	float4 pos = input.LightPosition;
	pos.xy /= pos.w;
	pos.x = pos.x * 0.5 + 0.5;
	pos.y = -pos.y * 0.5 + 0.5;

	float shadow = 1;
	float loop = 10;
	for (float angle = 0; angle < 360; angle += 360 / loop) {

		float2 uv = 0;
		uv.x = cos(angle * 3.14 / 180);
		uv.y = sin(angle * 3.14 / 180);
		uv *= 0.002;

		float depth = shadowTexture.Sample(sampler0, pos.xy+ uv).r;

		if (depth < input.LightPosition.z - 0.2) {
			shadow -= 1 / loop * 0.4;
		}
	}
	return shadow;
}

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output;

	output.Normal = input.Normal;
	output.Diffuse = input.Diffuse * Shadow(input);
	output.Position = input.WorldPosition;
	output.Depth = input.Depth;
	output.BloomColor = BloomColor;
	output.ReflectRate = ReflectRate;
	return output;

}
