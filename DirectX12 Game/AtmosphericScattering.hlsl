float3 AtmosphericScattering(float4 worldPos, float3 lightDir, float3 cameraPos)
{
    float3 diffuse = 0.0;
    
    //視線ベクトル
    float3 eye = worldPos.xyz - cameraPos;
    eye = normalize(eye);
    
    //大気距離
    float dist = distance(worldPos.xyz, cameraPos);
   
    //ミー散乱
    float m;
    m = saturate(-dot(lightDir, eye));
    m = pow(m, 50);
    
    float a = 0.005;
    diffuse += m * dist * a;
    
    //レイリー散乱
    float3 vy = float3(0.0, 1.0, 0.0);
    float atm = saturate(1.0 - dot(-lightDir, vy)); //ライト方向の大気密度
    float3 rcolor = 1.0 - float3(0.5, 0.8, 1.0) * atm * 1.0; //レイリー散乱減衰
    
    float ld = 0.5 - dot(lightDir, eye) * 0.5; //レイリー散乱
    diffuse += rcolor * dist * ld * float3(0.5, 0.8, 1.0) * a;
    
    return diffuse;

}