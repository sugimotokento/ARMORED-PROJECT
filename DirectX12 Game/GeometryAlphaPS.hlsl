#include"Common.hlsl"
#include"Noise.hlsl"
#include"AtmosphericScattering.hlsl"
Texture2D<float4> unlitColorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
Texture2D<float4> shadowTexture : register(t5);

Texture2D<float4> modelAlbedTexture : register(t7);
Texture2D<float4> modelNormalTexture : register(t8);
Texture2D<float4> modelMetalTexture : register(t9);
Texture2D<float4> modelEmmisionTexture : register(t10);

SamplerState sampler0 : register(s0);

struct PS_OUTPUT
{
    float4 Diffuse : SV_TARGET0;
};

float Shadow(PS_INPUT input)
{
    float4 pos = input.LightPosition;
    pos.xy /= pos.w;
    pos.x = pos.x * 0.5 + 0.5;
    pos.y = -pos.y * 0.5 + 0.5;

    float shadow = 1;
    float loop = 10;
    for (float angle = 0; angle < 360; angle += 360 / loop)
    {

        float2 uv = 0;
        uv.x = cos(angle * 3.14 / 180);
        uv.y = sin(angle * 3.14 / 180);
        uv *= 0.0009;

        float depth = shadowTexture.Sample(sampler0, pos.xy + uv).z;

        if (depth < input.LightPosition.z - 0.15)
        {
            shadow -= 1 / loop * 0.4;
        }
    }
    return shadow;
}



PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    {
        output.Diffuse.rgb = modelAlbedTexture.Sample(sampler0, input.TexCoord).rgb * input.Diffuse.rgb;
        output.Diffuse.a = input.Diffuse.a;
    }

    return output;

}
