#include"Common.hlsl"

Texture2D<float4> normalTexture : register(t0);
Texture2D<float4> colorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
Texture2D<float4> depthTexture : register(t3);
Texture2D<float4> metallicTexture : register(t4);
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
        uv *= 0.001;

        float depth = shadowTexture.Sample(sampler0, pos.xy + uv).r;

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
    
    float4 pos = input.WorldPosition;
    pos.w = 1;
    
    pos.w = 1;
    float4 uv = mul(pos, VP);
    uv.xy /= uv.w;
    uv.x = uv.x * 0.5 + 0.5;
    uv.y = -uv.y * 0.5 + 0.5;
    
    float4 normal = normalTexture.Sample(sampler0, uv.xy);
    float4 color = colorTexture.Sample(sampler0, uv.xy);
    float4 position = positionTexture.Sample(sampler0, uv.xy);
    float4 depth = depthTexture.Sample(sampler0, uv.xy);
    float4 metallicRate = metallicTexture.Sample(sampler0, uv.xy);


    output.Diffuse.rgb = input.Diffuse * Shadow(input);
    output.Diffuse.a = input.Diffuse.a;
    return output;

}
