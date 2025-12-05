//type vertex
#version 100
precision mediump float;

attribute vec3 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TexCoord;
attribute float a_TexIndex;
attribute float a_TilingFactor;

uniform mat4 u_VP;

//out vec4 v_Color;
//out vec2 v_TexCoord;
//out float v_TexIndex;
//out float v_TilingFactor;

void main()
{
//	v_Color = a_Color;
//	v_TexCoord = a_TexCoord;
//	v_TexIndex = a_TexIndex;
//	v_TilingFactor = a_TilingFactor;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}//*/

//type fragment
#version 100
precision mediump float;


uniform sampler2D u_Textures[32];

uniform vec2 u_resolution;
uniform vec2 u_c;
uniform float u_zoom;
uniform vec2 u_offset;
uniform int u_maxIter;
uniform int u_SPP;

const int MAX_ITER = 1000;

float sampleFractal(vec2 z)
{
	int i = u_maxIter;
	float nu = 0.0;
	float escapeRadius = 4.0;
	for (int it = 0; it < MAX_ITER; it++)
	{
		if (it >= u_maxIter) break;
		// z = z^2 + c
		float x = z.x;
		float y = z.y;
		float x2 = x*x - y*y + u_c.x;
		float y2 = 2.0*x*y + u_c.y;
		z = vec2(x2, y2);

		if(dot(z,z) > escapeRadius)
		{
			// smooth iteration count
			float log_zn = 0.5 * log(float(dot(z,z)));
			nu = log(log_zn / log(escapeRadius)) / log(2.0);
			i = it;
			break;
		}
	}

	float t = float(i) + (1.0 - nu);
	return t / float(u_maxIter);
}

const int MAX_SPP = 4;
vec2 sampleOffsets[MAX_SPP];

void main()
{
	sampleOffsets[0] = vec2(1.0/4.0, 1.0/4.0) * u_zoom * 0.001;
    sampleOffsets[1] = vec2(3.0/4.0, 1.0/4.0) * u_zoom * 0.001;
    sampleOffsets[2] = vec2(1.0/4.0, 3.0/4.0) * u_zoom * 0.001;
    sampleOffsets[3] = vec2(3.0/4.0, 3.0/4.0) * u_zoom * 0.001;

	vec2 uv = gl_FragCoord.xy / u_resolution;
	uv = uv * 2.0 - 1.0;
	uv.x *= u_resolution.x / u_resolution.y;
	uv += u_offset / u_zoom;
	uv *= u_zoom;

	float s = 0.0;
	for (int i = 0; i < MAX_SPP; i++)
	{
		if (i >= u_SPP) break;
		s += sampleFractal((uv + sampleOffsets[i]));
	}
	s /= float(u_SPP);

	gl_FragColor = vec4(s, s, s, 1.0);
}