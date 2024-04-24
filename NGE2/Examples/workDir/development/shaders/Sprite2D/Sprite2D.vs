$input a_position, a_texcoord0
$output v_texcoord0
/*=========================================
    Copyright 2023-2024 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position.xy, 0.0, 1.0));
	v_texcoord0 = a_texcoord0;
}