$input a_position, a_texcoord0
$output v_texcoord0
/*=========================================
    Copyright 2023-2024 Norbert Gerberg
=========================================*/

#include "../Shared/common.sh"

uniform vec4 scale;

void main()
{
	mat4 mdlView = u_modelView;
	
	mdlView[0][0] = scale.x;
	mdlView[0][1] = 0.0;
	mdlView[0][2] = 0.0;
	
	mdlView[1][0] = 0.0;
	mdlView[1][1] = scale.y;
	mdlView[1][2] = 0.0;
	
	mdlView[2][0] = 0.0;
	mdlView[2][1] = 0.0;
	mdlView[2][2] = 1.0;
	
	vec4 pos = mul(mdlView, vec4(a_position.xy, 0.0, 1.0));
	gl_Position = mul(u_proj, pos);
	v_texcoord0 = a_texcoord0;
}