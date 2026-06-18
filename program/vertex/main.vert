struct Input
{
  float2 corner : TEXCOORD0;
  float2 uv_corner : TEXCOORD1;
  float4 model_column_0 : TEXCOORD2;
  float4 model_column_1 : TEXCOORD3;
  float4 model_column_2 : TEXCOORD4;
  float4 model_column_3 : TEXCOORD5;
  float4 color : TEXCOORD6;
  float4 uv_rect : TEXCOORD7;
  float transparency : TEXCOORD8;
};
struct Output
{
  float4 position : SV_Position;
  float4 color : TEXCOORD0;
  float2 texcoord : TEXCOORD1;
  float transparency : TEXCOORD2;
};

cbuffer Matrices : register(b0, space1)
{
  float4x4 projection_matrix;
  float4x4 view_matrix;
};

Output main(Input input)
{
  float4x4 model_matrix = {input.model_column_0, input.model_column_1, input.model_column_2, input.model_column_3};
  float4 world_position = mul(float4(input.corner, 0.0f, 1.0f), model_matrix);
  Output output = {mul(projection_matrix, mul(view_matrix, world_position)), input.color,
                   lerp(input.uv_rect.xy, input.uv_rect.zw, input.uv_corner), input.transparency};
  return output;
}
