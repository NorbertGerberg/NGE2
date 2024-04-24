$input v_texcoord0
/*=========================================
    Copyright 2023-2024 Norbert Gerberg
=========================================*/
#include "../Shared/common.sh"

SAMPLER2D(s_texColor, 0);
uniform vec4 properties;
#define P_FXAA 	properties.x

//FXAA
void FXAATexCoords(vec2 fragCoord, vec2 resolution, out vec2 v_rgbNW, out vec2 v_rgbNE,
					out vec2 v_rgbSW, out vec2 v_rgbSE, out vec2 v_rgbM);
vec4 ApplyFXAA(sampler2D tex, vec2 fragCoord, vec2 resolution, vec2 v_rgbNW, vec2 v_rgbNE,
					vec2 v_rgbSW, vec2 v_rgbSE, vec2 v_rgbM);

void main()
{
	if(P_FXAA == 1.0)
	{
		vec2 v_rgbNW;
		vec2 v_rgbNE;
		vec2 v_rgbSW;
		vec2 v_rgbSE;
		vec2 v_rgbM;
		
		vec2 texSize = bgfxTextureSize(s_texColor, 0);
		vec2 fc = vec2(gl_FragCoord.x, -gl_FragCoord.y);

		vec2 uv = vec2(fc / texSize);
		uv.y = 1.0 - uv.y;
		vec2 fragCoord = uv * texSize;
		
		FXAATexCoords(fragCoord, texSize, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
		
		gl_FragColor = ApplyFXAA(s_texColor, fragCoord, texSize,
			v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
	}
	else
		gl_FragColor = texture2D(s_texColor, v_texcoord0);
}

void FXAATexCoords(vec2 fragCoord, vec2 resolution, out vec2 v_rgbNW, out vec2 v_rgbNE, out vec2 v_rgbSW, out vec2 v_rgbSE, out vec2 v_rgbM)
{
	vec2 inverseVP = 1.0 / resolution.xy;
	v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
	v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
	v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
	v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
	v_rgbM = vec2(fragCoord * inverseVP);
}

vec4 ApplyFXAA(sampler2D tex, vec2 fragCoord, vec2 resolution, vec2 v_rgbNW, vec2 v_rgbNE, vec2 v_rgbSW, vec2 v_rgbSE, vec2 v_rgbM)
{
	float reduceMin = 1.0 / 128.0;
	float reduceMul = 1.0 / 8.0;
	float spanMax 	= 8.0;
	
	vec4 color;

	mediump vec2 inverseVP = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture2D(tex, v_rgbNW).xyz;
    vec3 rgbNE = texture2D(tex, v_rgbNE).xyz;
    vec3 rgbSW = texture2D(tex, v_rgbSW).xyz;
    vec3 rgbSE = texture2D(tex, v_rgbSE).xyz;
    vec4 texColor = texture2D(tex, v_rgbM);
    vec3 rgbM  = texColor.xyz;
    vec3 luma = vec3(0.299, 0.587, 0.114);
	
	float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	
	mediump vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	
	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * reduceMul), reduceMin);
	
	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(spanMax, spanMax), max(vec2(-spanMax, -spanMax), dir * rcpDirMin)) * inverseVP;

	vec2 texCoordA = fragCoord * inverseVP + dir * (1.0 / 3.0 - 0.5);
	vec2 texCoordB = fragCoord * inverseVP + dir * (2.0 / 3.0 - 0.5);

	vec4 texSampleA = texture2D(tex, texCoordA);
	vec4 texSampleB = texture2D(tex, texCoordB);

	vec3 rgbA = 0.5 * (texSampleA.xyz + texSampleB.xyz);
	vec3 rgbB = rgbA * 0.5 + 0.25 * (texSampleA.xyz + texSampleB.xyz);

	float lumaB = dot(rgbB, luma);
	if (lumaB < lumaMin || lumaB > lumaMax)
		color = vec4(rgbA, texColor.a);
	else
		color = vec4(rgbB, texSampleB.a);
    return color;
}