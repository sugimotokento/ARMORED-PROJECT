cbuffer CBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float3 RadialDir;
    float radialRatio;
    float radialStartAngle;
    bool isRadialInvers;
    float2 dammy;
}


struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 Diffuse : COLOR;
    float2 TexCoord : TEXCOORD;

};
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal : NORMAL;
    float4 Diffuse : COLOR;
    float4 Depth : DEPTH;
    float2 TexCoord : TEXCOORD;
    
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;

    float4 LightPosition : POSITION1;
    float4 LightDepth : DEPTH2;
};


Texture2D<float4> mainTexture : register(t0);

SamplerState sampler0 : register(s0);

void Radial(PS_INPUT input)
{
    float PI = 3.14;
    float ratio = 1 - radialRatio;
    float startAngle = radialStartAngle;
    bool isInvers = isRadialInvers;


    //現在のテクスチャ座標の角度を計算する
    float2 texDir = (float2(input.TexCoord.y, input.TexCoord.x) - float2(0.5, 0.5));
    float polygonAngle = (atan2(texDir.y, texDir.x)) + PI;
    
    //向きを反転する
    if (isInvers)
    {
        //現在のテクスチャ座標の角度を計算する
        texDir = (float2(input.TexCoord.y, -input.TexCoord.x) - float2(0.5, -0.5));
        polygonAngle = (atan2(texDir.y, texDir.x)) + PI;

    }
        
    //開始地点の角度を加算して角度を再計算する
    polygonAngle += startAngle + PI;
    texDir = float2(cos(polygonAngle), sin(polygonAngle)) * length(texDir);
    polygonAngle = (atan2(texDir.y, texDir.x)) + PI;

    //消す基準値
    float baseAngle = (radialRatio * PI);

    
    if (baseAngle * 2 < polygonAngle)
        discard;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    Radial(input);

	float4 color = mainTexture.Sample(sampler0, input.TexCoord);

	color = color * input.Diffuse;
	
	return color;
}
