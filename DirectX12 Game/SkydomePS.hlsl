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

SamplerState sampler0 : register(s0);

struct PS_OUTPUT
{
    float4 Diffuse : SV_TARGET0;
};

float Scale(float fcos)
{
    const float fScaleDepth = 0.15;
    
    float x = 1.0 - fcos;
    return fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.8 + x * 5.25))));
}



PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    const float PI = 3.141592;
    float _InnerRadius = 100000;
    float _OuterRadius = 110000;
    const float rat = _OuterRadius / 100000;
 
    float _Kr = 0.0025;
    float _Km = 0.0000;
 
    const float fSamples = 3.0;
 
    const float3 three_primary_colors = float3(0.68, 0.55, 0.44);
    const float3 v3InvWaveLength = 1.0 / pow(three_primary_colors, 4.0);
 
    const float fOuterRadius = _OuterRadius;
    const float fInnerRadius = _InnerRadius;
 
    const float fESun = 20.0;
    const float fKrESun = _Kr * fESun;
    const float fKmESun = _Km * fESun;
 
    const float fKr4PI = _Kr * 4.0 * PI;
    const float fKm4PI = _Km * 4.0 * PI;
 
    const float fScale = 1.0 / (_OuterRadius - _InnerRadius);
    const float fScaleDepth = 0.25;
    const float fScaleOverScaleDepth = fScale / fScaleDepth;
 
    const float g = -0.999f;
    const float g2 = g * g;
    
    float3 worldPos = input.WorldPosition.xyz;
    //float3 worldPos = In.WorldPosition.xyz + (camera.viewDirection*0.03 + float3(0, 0.325, 0)*2) * _OuterRadius;
    worldPos.y += _InnerRadius;
    
    float3 v3CameraPos = rat;
    v3CameraPos.y = fInnerRadius;
    float3 lightPos = -GetLightDir();
    float3 v3LightDir = normalize(lightPos);
 
    float3 v3Ray = worldPos - v3CameraPos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;
 
    float3 v3Start = v3CameraPos;
    float fCameraHeight = length(v3CameraPos);
    float fStartAngle = dot(v3Ray, v3Start) / fCameraHeight;
    float fStartDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartOffset = fStartDepth * Scale(fStartAngle);
 
    float fSampleLength = fFar / fSamples;
    float fScaledLength = fSampleLength * fScale;
    float3 v3SampleRay = v3Ray * fSampleLength;
    float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
 
    float3 v3FrontColor = 0.2;
    for (int n = 0; n < int(fSamples); ++n)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth * (Scale(fLightAngle) - Scale(fCameraAngle)));
        float3 v3Attenuate = exp(-fScatter * (v3InvWaveLength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }
 
    float3 c0 = v3FrontColor * (v3InvWaveLength * fKrESun);
    float3 c1 = v3FrontColor * fKmESun;
    float3 v3Direction = v3CameraPos - worldPos;
 
    float fcos = dot(v3LightDir, v3Direction) / length(v3Direction);
    float fcos2 = fcos * fcos;
 
    float rayleighPhase = 0.75 * (1.0 + fcos2);
    float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fcos2) / pow(1.0 + g2 - 2.0 * g * fcos, 5);
 
    float4 col = 0;
    col.rgb = rayleighPhase * c0 + miePhase * c1;
    output.Diffuse = col;
    return output;

}

