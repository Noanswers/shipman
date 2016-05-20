
TextureCube     cubeMap;
SamplerState    samLinear;

cbuffer ConstantBuffer
{
	float4x4    wvp;
	float4x4	world;

	float4		lightDir;
	float4		lightColor;
}

struct VertexIn
{
	float3	posLocal	: POSITION;
	float4	color		: COLOR;

	float3	normal		: NORMAL;
	float2	tex			: TEXCOORD;
};

struct   VertexOut
{
	float4  posH		: SV_POSITION;       // Projection ��ǥ(Homogeneous Coordinates)
	float3  posLocal	: POSITION;       // ���� ��ǥ

	float4	color		: COLOR;
};

VertexOut   VS(VertexIn vIn)
{
	VertexOut   vout;
	vout.color = vIn.color;
	float4   projectionPos = mul(float4 (vIn.posLocal, 1.0f), wvp);
	vout.posH = projectionPos.xyww;   // Sky�� ���� ���� �׻� 1�̾�� �Ѵ�. 
	   
	vout.posLocal = vIn.posLocal;     // local ��ǥ�� lookup��ǥ�� ����. ( view ��ǥ )

	return vout;
}

float4   PS(VertexOut vOut) : SV_Target
{
//	return vOut.color;
	return cubeMap.Sample(samLinear, vOut.posLocal);
}
