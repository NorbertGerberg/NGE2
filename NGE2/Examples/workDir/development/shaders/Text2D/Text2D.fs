$input v_texcoord0
/*=========================================
    Copyright 2023-2024 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

SAMPLER2D(s_texColor, 0);

uniform vec4 atlasInfo[2];

void main()
{
	vec2 modifiedUV = v_texcoord0;
	
	if(atlasInfo[1].z == 1.0)
	{
		float resX = (atlasInfo[0].z / atlasInfo[1].x);
		float resY = (atlasInfo[0].w / atlasInfo[1].y);
	
		vec2 offsetUv = vec2(atlasInfo[0].x / resX, atlasInfo[0].y / resY);
		modifiedUV = modifiedUV * vec2(1.0 / resX, 1.0 / resY) + offsetUv;
	}
	
	vec4 tex = texture2D(s_texColor, modifiedUV);
	
	bool isBlack = all(lessThanEqual(abs(tex.rgb - vec3_splat(0.0)), vec3_splat(0.001)));
	if((tex.r == 1.0 && tex.g == 0.0 && tex.b == 0.0) || isBlack)
		tex.a = 0.0;

	gl_FragColor = tex;
}