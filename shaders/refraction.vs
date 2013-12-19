float3 fvLightPosition = float3(100,100,100);
float3 fvEyePosition;
float4x4 matView;
float4x4 matViewProjection;
float fTime0_X;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent :  TANGENT0;
   
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection:   TEXCOORD2;
   float3 eyeLinear: TEXCOORD3;
};

VS_OUTPUT main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   float4 pPos = mul( Input.Position, matViewProjection );

   Output.Position         = pPos;
   Output.Texcoord         = Input.Texcoord * 2.5 + fTime0_X;
   
   float3 fvObjectPosition = mul( Input.Position, matView ).xyz;
   
   float3 fvViewDirection  = fvEyePosition - fvObjectPosition;
   float3 fvLightDirection = fvLightPosition - fvObjectPosition;
     
   float3 fvNormal         = mul( float4(Input.Normal, 1.0f), matView ).xyz;
   float3 fvBinormal       = mul( float4(Input.Binormal, 1.0f), matView ).xyz;
   float3 fvTangent        = mul( float4(Input.Tangent, 1.0f), matView ).xyz;
      
   Output.ViewDirection.x  = dot( fvTangent, fvViewDirection );
   Output.ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   Output.ViewDirection.z  = dot( fvNormal, fvViewDirection );
   
   Output.LightDirection.x  = dot( fvTangent, fvLightDirection );
   Output.LightDirection.y  = dot( fvBinormal, fvLightDirection );
   Output.LightDirection.z  = dot( fvNormal, fvLightDirection );
   
   // Eye-linear texgen
   Output.eyeLinear = pPos.xyw;
   
   return( Output );
   
}

