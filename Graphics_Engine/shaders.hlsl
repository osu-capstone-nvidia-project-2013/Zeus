float4 VShader(float4 position : POSITION) : SV_POSITION
{
    return position;
}

float4 PShader() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}