#version 400

uniform sampler2D colorTex1;
uniform vec3 lightDir;
uniform vec3 camPos;
uniform vec3 skyColor;
uniform vec3 sunColor;
uniform float elapsed;

//Variables en entree
in vec3 normal;
in vec4 color;
in vec2 uv;
in vec3 wPos;

flat in float type;

out vec4 color_out;

//Globales
const float ambientLevel = 0.4;

#define CUBE_EAU 5.0

float waterNoise(vec3 v)
{
	vec2 dir = vec2(v.x, v.y);

	float d = dot(vec2(1, 1), dir);
	float n = sin(d + elapsed) / 2;

	d = dot(vec2(0, .5), dir);
	n += sin(d + elapsed) / 4;

	d = dot(vec2(.5, 0), dir);
	n += sin(d + elapsed) / 4;

	return n;
}

void main()
{
	//vec4 colorTex = texture(colorTex1, uv) * color;
	vec3 toLight = normalize(lightDir);
	vec3 view = normalize(camPos - wPos);

	vec4 c = color;

	vec3 N = normal;

	if(type == CUBE_EAU)
	{
		vec3 A = wPos;
		A.z += waterNoise(wPos) - 1;

		vec3 B = wPos + vec3(0.1f, 0, 0);
		B.z += waterNoise(B) - 1;

		vec3 C = wPos + vec3(0, 0.1f, 0);
		C.z += waterNoise(C) - 1;

		N = cross( normalize(B-A), normalize(C-A));

		float fresnel = max(0, dot(N, view));
		c.a = pow(1-fresnel, 0.2);

		c.rgb = mix(c.rgb, skyColor.rgb, pow((1 - fresnel), 10));
	}

	float diffuse = max(0.005f, dot(toLight, normal));
	c.rgb *= diffuse;

	//Spec
	vec3 halfVec = normalize(toLight + view);
	float spec = abs(dot(normal, toLight));
	spec = 5f * pow(spec, 300);

	c.rgb += sunColor * spec;

	//Ambient
	c.rgb += 0.01 * skyColor * (1 - diffuse);
	vec4 texColor =texture(colorTex1, uv);
	texColor.x = pow(texColor.x, 2.0);
	texColor.y = pow(texColor.y, 2.0);
	texColor.z = pow(texColor.z, 2.0);

	color_out = vec4(sqrt(c.rgb), c.a); //* texColor;
}