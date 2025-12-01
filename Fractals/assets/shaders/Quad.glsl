//type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

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
#version 450 core

layout(location = 0) out vec4 color;

//in vec4 v_Color;
//in vec2 v_TexCoord;
//in float v_TexIndex;
//in float v_TilingFactor;

uniform sampler2D u_Textures[32];

uniform vec2 u_resolution;
uniform vec2 u_c;
uniform float u_zoom;
uniform vec2 u_offset;
uniform int u_maxIter;
uniform int u_SPP;



struct DS { float hi; float lo; };
// Convert float to DS
DS floatToDS(float f) { return DS(f, 0.0); }
// DS addition
DS dsAdd(DS a, DS b)
{
    float s = a.hi + b.hi;
    float v = s - a.hi;
    float t = ((b.hi - v) + (a.hi - (s - v))) + a.lo + b.lo;
    return DS(s, t);
}
// DS subtraction
DS dsSub(DS a, DS b)
{
    float s = a.hi - b.hi;
    float v = s - a.hi;
    float t = ((-b.hi - v) + (a.hi - (s - v))) + a.lo - b.lo;
    return DS(s, t);
}
// DS multiplication
DS dsMul(DS a, DS b)
{
    float c11 = a.hi * b.hi;
    float c21 = a.hi * b.lo + a.lo * b.hi;
    float s = c11 + c21;
    float t = c21 - (s - c11) + a.lo * b.lo;
    return DS(s, t);
}
// DS squared
DS dsSqr(DS a) { return dsMul(a, a); }
// DS to float (approximate for coloring)
float dsToFloat(DS a) { return a.hi + a.lo; }




float sampleFractal(vec2 z)
{
	int i = 0;
	float nu = 0.0;
	float escapeRadius = 4.0;
	for (; i < u_maxIter; ++i)
	{
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
			break;
		}
	}

	float it = float(i) + (1.0 - nu);
	return it / float(u_maxIter);
}

vec2 sampleOffsets[9] = vec2[](
    vec2(1.0/6.0, 1.0/6.0),
    vec2(3.0/6.0, 1.0/6.0),
    vec2(5.0/6.0, 1.0/6.0),
    vec2(1.0/6.0, 3.0/6.0),
    vec2(3.0/6.0, 3.0/6.0),
    vec2(5.0/6.0, 3.0/6.0),
    vec2(1.0/6.0, 5.0/6.0),
    vec2(3.0/6.0, 5.0/6.0),
    vec2(5.0/6.0, 5.0/6.0)
);

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution;
	uv = uv * 2.0 - 1.0;
	uv.x *= u_resolution.x / u_resolution.y;
	uv += u_offset / u_zoom;
	uv *= u_zoom;

	float s = 0.0;
	for (int i = 0; i < u_SPP; i++)
	{
		s += sampleFractal((uv + sampleOffsets[i % 9] * u_zoom * 0.001));
	}
	s /= float(u_SPP);
	
	color = vec4(s, s, s, 1.0);
}