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
	float n = (1+sin(d + elapsed)) / 4;

	d = dot(vec2(1, .5), dir);
	n += (1+sin(d + elapsed)) / 2;

	d = dot(vec2(-.5, 1), dir);
	n += (1+ sin(d + elapsed)) / 4;

	return n / 2.75;
}

void main()
{
	vec4 texColor = texture(colorTex1, uv);
	texColor.x = pow(texColor.x, 2.2);
	texColor.y = pow(texColor.y, 2.2);
	texColor.z = pow(texColor.z, 2.2);

	

	vec3 toLight = normalize(lightDir);
	vec3 view = normalize(camPos - wPos);
	vec4 c = texColor * color;

	vec3 N = normal;
	N = normalize(N);

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
		c.a = 1-fresnel;

		//c.rgb = mix(c.rgb, skyColor.rgb, pow((1 - fresnel), 500));
	}

	float diffuse = max(0.005f, dot(toLight, N));
	c.rgb *= diffuse;

	//Spec
	if(type == CUBE_EAU){
	vec3 halfVec = normalize(toLight + view);
	float spec = abs(dot(N, halfVec));
	spec = pow(spec, 300);

	c.rgb += sunColor * spec;
	}
	//Ambient
	c.rgb += 0.01 * skyColor * (1 - diffuse);


	color_out = sqrt(c);
}