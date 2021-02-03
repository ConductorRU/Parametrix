// simple pixel shader
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 wposition : TEXCOORD1;
	float2 uv : TEXCOORD2;
};
cbuffer ConstantBufferX : register(b0)
{
	float3 lightDir;
	float3 camPos;
};
float4 main(PSInput ps) : SV_TARGET
{
	float3 viewDirection = normalize(camPos - ps.wposition.xyz);
	float3 lightDirection = -lightDir;
	float lightIntensity = saturate(dot(ps.normal, lightDirection));
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 color = float4(0.2f, 0.2f, 0.2f, 1.0f)*ps.color;
	float3 fr = float3(frac(ps.uv), 1.0f);
	if(lightIntensity > 0.0f)
	{
	 color += (ps.color * lightIntensity);
   color = saturate(color);
   float3 reflection = normalize(2 * lightIntensity * ps.normal - lightDirection);
   specular = pow(saturate(dot(reflection, viewDirection)), 100.0f);
	}

	return float4(color.rgb + specular.rgb, 1.0f); // Red, Green, Blue, Alpha
}