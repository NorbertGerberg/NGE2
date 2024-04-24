$input v_texcoord0
/*=========================================
    Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

//Material
SAMPLER2D(tex_diffuse, 0);

void main()
{
	gl_FragColor = texture2D(tex_diffuse, v_texcoord0);
}