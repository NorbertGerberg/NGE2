$input a_position, a_normal, a_texcoord0, a_tangent, a_bitangent, i_data0, i_data1, i_data2, i_data3
$output v_texcoord0
/*=========================================
    Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

void main()
{
	mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);
	vec4 worldPos = mul(model, vec4(a_position, 1.0) );
	gl_Position = mul(u_viewProj, worldPos);

	v_texcoord0 = a_texcoord0;
	
}