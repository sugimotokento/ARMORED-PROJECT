#include"Common.hlsl"
#include"Noise.hlsl"
#include"AtmosphericScattering.hlsl"
Texture2D<float4> unlitColorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
Texture2D<float4> depthTexture : register(t3);
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

float4 Water(PS_INPUT input)
{
    float4 outColor = 1;
    float speed = 1 + WaterParam.w * 0.1;
    float texRatio = 0.5;
    float dx = fbm2((input.WorldPosition.xz * texRatio + float2(0.001, 0.0)) * 0.05, 8, speed) * 10.0
             - fbm2((input.WorldPosition.xz * texRatio - float2(0.001, 0.0)) * 0.05, 8, speed) * 10.0;
    float dz = fbm2((input.WorldPosition.xz * texRatio + float2(0.0, 0.001)) * 0.05, 8, speed) * 10.0
             - fbm2((input.WorldPosition.xz * texRatio - float2(0.0, 0.001)) * 0.05, 8, speed) * 10.0;
    
    float dx2 = fbm2((input.WorldPosition.xz * texRatio + float2(0.001, 0.0)) * 0.02, 8, -speed) * 10.0
             - fbm2((input.WorldPosition.xz * texRatio - float2(0.001, 0.0)) * 0.02, 8, -speed) * 10.0;
    float dz2 = fbm2((input.WorldPosition.xz * texRatio + float2(0.0, 0.001)) * 0.02, 8, -speed) * 10.0
             - fbm2((input.WorldPosition.xz * texRatio - float2(0.0, 0.001)) * 0.02, 8, -speed) * 10.0;
    
    float3 normal = float3(-dx, 0.001, dz);
    float3 normal2 = float3(-dx2, 0.001, dz2);
    normal = normalize(normal + normal2);
    
    float3 lightDir = GetLightDir();
    lightDir = normalize(lightDir);
    
    float3 eyev = input.WorldPosition.xyz - cameraPosition.xyz;
    eyev = normalize(eyev);
    
    float light = saturate(-dot(normal, lightDir));
    outColor.rgb = light;
    
    //フォン
    float3 refv = reflect(lightDir.xyz, normal.xyz);
    refv = normalize(refv);

    float specular = dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 30);
   // outDiffuse.rgb = spec * 1.0;
    
    //フレネル近似式
    float fresnel = saturate(1.0 + dot(eyev, normal));
    fresnel = 0.05 + (1.0 - 0.05) * pow(fresnel, 5);
    fresnel *= light;
    
    float4 waterDepth = input.Position;
    float4 uv = mul(input.WorldPosition, VP);
    uv.xy /= uv.w;
    uv.x = uv.x * 0.5 + 0.5;
    uv.y = -uv.y * 0.5 + 0.5;
    float4 depth = depthTexture.Sample(sampler0, uv.xy);
    float alpha = (length(depth.z - waterDepth.z)) * 49.84;
  
    alpha *= 0.2;
    alpha = pow(alpha, 0.16);

    //alpha *= WaterParam.x;
    //alpha = pow(alpha, WaterParam.y);
   

    float3 underColor = float3(0.046, 0.111, 0.144);
    outColor.rgb = underColor;
    outColor.rgb = outColor.rgb * (1.0 - fresnel) + outColor.rgb * fresnel * 1.3;
    outColor.rgb *= 1.3;
    alpha = alpha;
    alpha = max(0, alpha);
    alpha = min(1, alpha);
    

    float3 texColor = lerp(unlitColorTexture.Sample(sampler0, uv.xy).rgb, outColor.rgb, alpha);
    outColor.rgb = lerp(texColor, outColor.rgb, alpha);

    
    return outColor;
}


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    {
        float4 color = Water(input);
        output.Diffuse.rgb = color.rgb*1.2f;
        output.Diffuse.a = 0;
    }
   

    return output;

}
