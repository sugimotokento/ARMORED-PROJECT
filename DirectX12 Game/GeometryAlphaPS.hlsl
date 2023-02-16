#include"Common.hlsl"
#include"Noise.hlsl"
#include"AtmosphericScattering.hlsl"
Texture2D<float4> unlitColorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
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

float4 Water(PS_INPUT input)
{
    float4 outColor = 1;
    float speed = 1+WaterParam.w*0.05;
    float dx = fbm2((input.TexCoord + float2(0.001, 0.0)) * 0.05, 8, speed) * 10.0
             - fbm2((input.TexCoord - float2(0.001, 0.0)) * 0.05, 8, speed) * 10.0;
    float dz = fbm2((input.TexCoord + float2(0.0, 0.001)) * 0.05, 8, speed) * 10.0
             - fbm2((input.TexCoord - float2(0.0, 0.001)) * 0.05, 8, speed) * 10.0;
    
    float dx2 = fbm2((input.TexCoord + float2(0.001, 0.0)) * 0.02, 8, -speed) * 10.0
             - fbm2((input.TexCoord - float2(0.001, 0.0)) * 0.02, 8, -speed) * 10.0;
    float dz2 = fbm2((input.TexCoord + float2(0.0, 0.001)) * 0.02, 8, -speed) * 10.0
             - fbm2((input.TexCoord - float2(0.0, 0.001)) * 0.02, 8, -speed) * 10.0;
    
    float3 normal = float3(-dx, 0.001, -dz);
    float3 normal2 = float3(-dx2, 0.001, -dz2);
    normal = normalize(normal + normal2);
    
    float3 lightDir = float3(1, -0.5, 1);
    lightDir = normalize(lightDir);
    
    float3 eyev = input.WorldPosition.xyz - cameraPosition.xyz;
    eyev = normalize(eyev);
    
    float light = saturate(-dot(normal, lightDir));
    outColor.rgb = light;
    
    //フォン
    float3 refv = reflect(lightDir.xyz, normal.xyz);
    refv = normalize(refv);

    float specular = -dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 50);
   // outDiffuse.rgb = spec * 1.0;
    
    //フレネル近似式
    float fresnel = saturate(1.0 + dot(eyev, normal));
    fresnel = 0.05 + (1.0 - 0.05) * pow(fresnel, 5);
    fresnel *= light;
    
    
    float4 uv = mul(input.WorldPosition, VP);
    uv.xy /= uv.w;
    uv.x = uv.x * 0.5 + 0.5;
    uv.y = -uv.y * 0.5 + 0.5;
    float4 depth = positionTexture.Sample(sampler0, uv.xy);
    float alpha = (length(depth - input.WorldPosition)) * 0.5;
    alpha = pow(alpha, 0.26f);
    alpha *= 0.6f;
    
    float3 blendColor = float3(0.03, 0, 0.4);
    float3 atmosphericScattering = AtmosphericScattering(input.WorldPosition, lightDir.xyz, cameraPosition.xyz);
    float3 mainColor = lerp(atmosphericScattering, blendColor, 0.3); 
    
    float3 upColor = mainColor ;
    float3 underColor = float3(0.05, 0.1, 0.15);
    outColor.rgb = lerp(upColor, underColor, alpha+0.1);
    outColor.rgb = outColor.rgb * (1.0 - fresnel) + outColor.rgb * fresnel*1.25 + specular;
    
    alpha =  alpha;
    alpha = max(0, alpha);
    alpha = min(1, alpha);
    outColor.a = alpha;
    
    return outColor;
}


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    if (IsWater == true)
    {
        float4 color = Water(input);
        output.Diffuse.rgb = color.rgb;
        output.Diffuse.a = color.a;
    }
    else
    {
        output.Diffuse.rgb = input.Diffuse.rgb ;
        output.Diffuse.a = input.Diffuse.a;
    }

    return output;

}
