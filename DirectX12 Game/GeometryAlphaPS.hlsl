#include"Common.hlsl"


Texture2D<float4> shadowTexture : register(t5);
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
    
    output.Diffuse.rgb = input.Diffuse * Shadow(input);
    output.Diffuse.a = input.Diffuse.a;
    return output;

}
