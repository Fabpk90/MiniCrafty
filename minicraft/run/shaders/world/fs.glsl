#version 400

uniform sampler2D colorTex1;

//Variables en entree
in vec3 normal;
in vec4 color;
in vec2 uv;

out vec4 color_out;

//Globales
const float ambientLevel = 0.4;

void main()
{
	vec4 colorTex = texture(colorTex1, uv) * color;
	vec3 toLight = normalize(vec3(0.2,0.5,1));
	color_out = vec4(sqrt(colorTex.xyz * max(0.1,dot(toLight,normal)) * 0.97 + 0.03 * vec3(0.8,0.9,1)),colorTex.a);
}