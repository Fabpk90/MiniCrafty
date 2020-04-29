#version 400

in vec2 uv;

uniform sampler2D TexColor;
uniform sampler2D TexDepth;
uniform float screen_width;
uniform float screen_height;
uniform vec2 near_far;

out vec4 color_out;

float LinearizeDepth(float z)
{
	float n = near_far.x; // camera z near
  	float f = near_far.y; // camera z far
  	return (2.0 * n) / (f + n - z * (f - n));
}

void main (void)
{
	float xstep = 1.0/screen_width;
	float ystep = 1.0/screen_height;
	float ratio = screen_width / screen_height;

	vec4 color = texture2D( TexColor , uv );
	float depth = texture2D( TexDepth , uv ).r;	
	
	//Permet de scaler la profondeur
	depth = LinearizeDepth(depth);

	float nbPixels = 0;
	vec3 sumPixel = vec3(0);
	float size = 2;
	float totalIte = 0;

	for(float x= -size; x<= size; x++)
	{
		for(float y = -size; y <= size; y++)
		{
			totalIte++;
			vec2 delta = vec2(x * xstep, y * ystep);
			sumPixel += texture2D(TexColor, uv + delta).rgb;
		}
	}

	//sumPixel /= totalIte;

	//color.rgb = sumPixel;

	vec3 colorSobel = vec3(0);
	float depthSobel = 0;
	size = 1;
    nbPixels = 0;
    float depthSSAO = 0;
    for(float x = -size; x<=size; x++)    
    {
        for(float y = -size; y<=size;y++)
        {
            nbPixels++;
            vec2 delta = vec2(x*xstep, y*ystep)*1;
            depthSSAO += max(-0.01, min(0.001, depth - LinearizeDepth(texture2D(TexDepth, uv +delta).r)));
        }
    }

    depthSobel -= nbPixels * depth;
    colorSobel -= nbPixels * color.rgb;

	color.rgb /= 1+((depthSSAO*20));

    //Gamma correction

	color_out = vec4(color.rgb,1.0);
}