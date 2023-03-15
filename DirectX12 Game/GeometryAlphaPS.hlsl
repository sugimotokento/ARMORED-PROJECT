#include"Common.hlsl"
#include"Noise.hlsl"
#include"AtmosphericScattering.hlsl"
Texture2D<float4> unlitColorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
Texture2D<float4> shadowTexture : register(t5);

Texture2D<float4> modelAlbedTexture : register(t7);
Texture2D<float4> modelNormalTexture : register(t8);
Texture2D<float4> modelOcclusionTexture : register(t9);
Texture2D<float4> modelMetalTexture : register(t10);
Texture2D<float4> modelEmmisionTexture : register(t11);

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
        float3 albed = modelAlbedTexture.Sample(sampler0, input.TexCoord).rgb;
        float3 occlusion = modelOcclusionTexture.Sample(sampler0, input.TexCoord).rgb;
        float4 metal = modelMetalTexture.Sample(sampler0, input.TexCoord);
        float4 emmision = modelEmmisionTexture.Sample(sampler0, input.TexCoord);
        float3 normal = modelNormalTexture.Sample(sampler0, input.TexCoord).rgb;
        normal = normalize(normal);
        normal = (normal - 0.5) * 2;
    
        float3 finalNormal =
            input.Tangent * normal.x +
            input.Binormal * normal.y +
            input.Normal.xyz * normal.z;
        finalNormal = normalize(finalNormal);
        
        
        float3 lightDir = GetLightDir();
        float specular = 0;
        float specularRatio = GetMetalRatio(metal);
        float3 viewDir = input.WorldPosition.xyz - cameraPosition;
        viewDir = normalize(viewDir);
        lightDir = normalize(lightDir);
        


        float3 refv = reflect(finalNormal.xyz, lightDir.xyz);
        refv = normalize(refv);

        specular = -dot(viewDir, refv);
        specular = saturate(specular);
        specular = pow(specular, 50);
        

        specular *= specularRatio;
        
        output.Diffuse.rgb = (albed * occlusion * input.Diffuse.rgb) + (specular * occlusion) + (emmision.rgb);
        output.Diffuse.a = input.Diffuse.a;
    }


    return output;

}

