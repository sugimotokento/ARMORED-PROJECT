#include"Common.hlsl"

struct PS_OUTPUT
{
    float4 Depth      : SV_TARGET0;


};


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    output.Depth = input.Depth;
    return output;

}
