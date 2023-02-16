float3 AtmosphericScattering(float4 worldPos, float3 lightDir, float3 cameraPos)
{
    float3 diffuse = 0.0;
    
    //�����x�N�g��
    float3 eye = worldPos.xyz - cameraPos;
    eye = normalize(eye);
    
    //��C����
    float dist = distance(worldPos.xyz, cameraPos);
   
    //�~�[�U��
    float m;
    m = saturate(-dot(lightDir, eye));
    m = pow(m, 50);
    
    float a = 0.005;
    diffuse += m * dist * a;
    
    //���C���[�U��
    float3 vy = float3(0.0, 1.0, 0.0);
    float atm = saturate(1.0 - dot(-lightDir, vy)); //���C�g�����̑�C���x
    float3 rcolor = 1.0 - float3(0.5, 0.8, 1.0) * atm * 1.0; //���C���[�U������
    
    float ld = 0.5 - dot(lightDir, eye) * 0.5; //���C���[�U��
    diffuse += rcolor * dist * ld * float3(0.5, 0.8, 1.0) * a;
    
    return diffuse;

}