#include"Common.hlsl"
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 position = float4(input.Position, 1.0f);
    output.Position = mul(position, WVP);
    output.WorldPosition = mul(position, World);
    output.Depth = output.Position.z;

    float4 normal = float4(input.Normal, 0.0f);
    output.Normal = mul(normal, World);
    output.Normal = normalize(output.Normal);

    output.TexCoord = input.TexCoord;

    output.Diffuse.rgb = input.Diffuse.rgb;
    output.Diffuse.a = 1.0;
    
    output.Tangent = normalize(mul(World, input.Tangent)).xyz;
    output.Binormal = normalize(mul(World, input.Binormal)).xyz;

    output.LightPosition = mul(position, WVPLight);

    return output;
}

