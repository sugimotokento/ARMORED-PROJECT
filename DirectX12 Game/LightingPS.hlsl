#include"Common.hlsl"

Texture2D<float4> normalTexture : register(t0);
Texture2D<float4> unlitColorTexture : register(t1);
Texture2D<float4> positionTexture : register(t2);
Texture2D<float4> depthTexture : register(t3);
Texture2D<float4> metallicTexture : register(t4);
Texture2D<float4> colorTexture : register(t6);






SamplerState sampler0 : register(s0);

float Random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float ScreenSpaceAnbientOcclusion(PS_INPUT input)
{
    float ao = 1;
    int loop = 20;
    float4 normal = normalTexture.Sample(sampler0, input.TexCoord);
    float depth = depthTexture.Sample(sampler0, input.TexCoord).r;


    for (int i = 0; i < loop; i++)
    {
		//ランダムなベクトルを作成
        float x = Random(float2(i * input.TexCoord.x, i * input.TexCoord.y)) * 2 - 1;
        float y = Random(float2(x, i * input.TexCoord.y)) * 2 - 1;
        float rand = Random(float2(x, y));
        float3 vec = normalize(float3(x, y, rand));
        float dt = dot(normal.xyz, vec);
        float sgn = -sign(dt);
        vec *= sgn;

        float randomDepth = depthTexture.Sample(sampler0, input.TexCoord + (vec.xy * rand * 0.01)).r;
        if (depth > randomDepth + 0.01)
        {
            ao *= 0.99;
        }
    }

    return ao;
}
float4 ScreenSpaceReflection(PS_INPUT input, float4 color)
{
    float4 normal = normalTexture.Sample(sampler0, input.TexCoord);
    float4 position = positionTexture.Sample(sampler0, input.TexCoord);
    float depth = depthTexture.Sample(sampler0, input.TexCoord).r;
    if (length(normal.rgb) <= 0)
        return color;

    float3 dir = position.xyz - cameraPosition;
    dir = normalize(dir);
    float3 refVec = reflect(dir.xyz, normal.xyz);
    refVec = normalize(refVec);


    for (int i = 0; i < 50; i++)
    {
        position.xyz += (refVec.xyz);
        position.w = 1;
        float4 uv = mul(position, VP);
        uv.xy /= uv.w;
        uv.x = uv.x * 0.5 + 0.5;
        uv.y = -uv.y * 0.5 + 0.5;

        //if (uv.x > 1 || uv.x < 0 || uv.y > 1 || uv.y < 0)
        //{
        //    break;
        //}

        depth = depthTexture.Sample(sampler0, uv.xy).r;
        if (uv.z > depth)
        {
            return colorTexture.Sample(sampler0, uv.xy);
        }
    }
    return colorTexture.Sample(sampler0, input.TexCoord);
}
float4 Bloom(PS_INPUT input)
{
	
    float4 bloom = 0;
    int loop = 10;
    for (int y = -loop; y < loop + 1; y++)
    {
        for (int x = -loop; x < loop + 1; x++)
        {
            float2 texCoord = input.TexCoord + float2((float) x * 1.2 / 1920, (float) y * 1.2 / 1080);
            if (texCoord.x > 1 || texCoord.x < 0 || texCoord.y > 1 || texCoord.y < 0)
            {
                break;
            }
            float4 color = colorTexture.Sample(sampler0, texCoord);
            float bright = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
            if (color.r >= 1 && color.g >= 1 && color.b >= 1 && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
            else if (color.r > 1.1 && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
            else if (color.g > 1.1 && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
            else if (color.b > 1.1 && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
        }
    }
    return bloom / (loop * 0.5);

}


float4 main(PS_INPUT input) : SV_TARGET
{

    float4 normal = normalTexture.Sample(sampler0, input.TexCoord);
    float4 baseColor = colorTexture.Sample(sampler0, input.TexCoord);
    float4 position = positionTexture.Sample(sampler0, input.TexCoord);
    float4 depth = depthTexture.Sample(sampler0, input.TexCoord);
    float4 metallicRate = metallicTexture.Sample(sampler0, input.TexCoord);
    normal = normalize(normal);

   
    
    float4 color = baseColor;
    float lambert = 1;
    float specular = 0;
    float ssao = 1;
    float3 lightDir = GetLightDir();
    float3 viewDir = position - cameraPosition;
    viewDir = normalize(viewDir);
    lightDir = normalize(lightDir);

    lambert = dot(lightDir, normal);
    lambert = saturate(lambert) *0.5+0.5;

    float3 refv = reflect(lightDir.xyz, normal.xyz);
    refv = normalize(refv);

    specular = -dot(viewDir, refv);
    specular = saturate(specular);
    specular = pow(specular, 50);

    ssao = ScreenSpaceAnbientOcclusion(input);
	

    float4 bloom = Bloom(input);
    color = (baseColor * lambert) * ssao;
    float4 reflectColor = ScreenSpaceReflection(input, color);
    color = reflectColor * metallicRate + color * (1 - metallicRate);

    return float4((color + bloom).rgb, 1);
    if (position.y > 0)
    {
       
    }

    return float4(((colorTexture.Sample(sampler0, input.TexCoord))).rgb, 1);
}
