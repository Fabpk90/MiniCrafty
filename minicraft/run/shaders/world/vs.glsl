#version 400

uniform float elapsed;
uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 nmat;

layout(location=0) in vec3 vs_position_in;
layout(location=1) in vec3 vs_normal_in;
layout(location=2) in vec2 vs_uv_in;
layout(location=3) in float vs_type_in;

//Variables en sortie
out vec3 normal;
out vec4 color;
out vec2 uv;
out vec3 wPos;

flat out float type;

#define CUBE_HERBE 1.0
#define CUBE_TERRE 2.0
#define CUBE_EAU 5.0

float waterNoise(vec4 v)
{
	vec2 dir = vec2(v.x, v.y);

	float d = dot(vec2(1, 1), dir);
	float n = sin(d + elapsed) / 2;

	d = dot(vec2(1, .5), dir);
	n += sin(d + elapsed) / 4;

	d = dot(vec2(.5, 1), dir);
	n += sin(d + elapsed) / 4;

	return n;
}

void main()
{
	vec4 vecIn = vec4(vs_position_in,1.0);
	vec4 vecInW = m * vecIn;
		
	normal = (nmat * vec4(vs_normal_in,1.0)).xyz; 

	uv = vs_uv_in;

	//Couleur par défaut blanc
	color = vec4(1.0,1.0,1.0,1.0);

	wPos = vecInW.xyz;

	//Couleur fonction du type
	if(vs_type_in == CUBE_HERBE)
		color = vec4(154 / 255,255 / 255,87/255,1);
	else if(vs_type_in == CUBE_TERRE)
		color = vec4(0.2,0.1,0,1);
	else if(vs_type_in == CUBE_EAU)
	 {
		color = vec4(0.0,0.0,1.0,0.5);	
		vecInW.z += waterNoise(vecInW);
	 }

	 type = int(vs_type_in);

	 
	 gl_Position = p * v * vecInW;
}