$input v_texcoord0
/*=========================================
    Copyright 2023-2024 Norbert Gerberg
=========================================*/

#include "../Shared/common.sh"

SAMPLER2D(s_texColor, 0);

void main()
{
	vec4 texColor = texture2D(s_texColor, v_texcoord0);
	if(texColor.a <= 0.1)
		discard;

	gl_FragColor = texColor;
}