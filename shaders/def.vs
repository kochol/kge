float4x4 matWorld;
float4x4 matViewProjection;

struct VS_INPUT 
{
   float3 Pos:     POSITION;
   float3 Normal:  NORMAL;
   float2 Tex:	   TEXCOORD;
};

struct VS_OUTPUT 
{
   float4 Pos:            POSITION;
   float3 PosInWorld:     TEXCOORD0; 
   float3 NormalInWorld:  TEXCOORD1;
   float2 Tex:			  TEXCOORD2;
};

VS_OUTPUT main( VS_INPUT In )
{
   VS_OUTPUT Out;

   Out.Pos           = mul(float4(In.Pos,1.0), matViewProjection);
   Out.PosInWorld    = mul(matWorld,float4(In.Pos,1.0)).xyz;
   Out.NormalInWorld = mul(matWorld,float4(In.Normal,1.0)).xyz;
   Out.Tex			 = In.Tex;

   return Out;
}
