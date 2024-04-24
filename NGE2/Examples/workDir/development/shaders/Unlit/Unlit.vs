$input a_position, a_normal, a_texcoord0, a_tangent, a_bitangent
$output v_texcoord0
/*=========================================
    Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

void main()
{
	v_texcoord0 = a_texcoord0;
	vec4 aPos = vec4(a_position, 1.0);
	gl_Position = mul(u_modelViewProj, aPos);
}