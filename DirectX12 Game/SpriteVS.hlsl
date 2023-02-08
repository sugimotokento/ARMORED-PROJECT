#include"Common.hlsl"


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 position = float4(input.Position, 1.0f);
    output.Position = mul(position, WVP);

    float4 normal = float4(input.Normal, 0.0f);
    output.Normal = mul(normal, World);

    output.TexCoord = input.TexCoord;

    output.Diffuse = input.Diffuse;

    return output;
}

