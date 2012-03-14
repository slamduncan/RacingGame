// Sobel edge detection filter
// texture of the scene
uniform sampler2D scene;

// texture size ( normally the size of the screen)
uniform float width;
uniform float height;

// Sobel in X
// [ -1 0 1 ]
// [ -2 0 2 ]
// [ -1 0 1 ]
//const mat3 Sx = mat3(vec3(-1, -2, -1), vec3(0, 0, 0), vec3(1, 2, 1));
const float kernal[9] = { -1, 0, 1,
						  -2, 0, 2,
						  -1, 0, 1};

// Sobel in Y
// [ -1 -2 -1 ]
// [  0  0  0 ]
// [  1  2  1 ]
//const mat3 Sy = mat3(vec3(-1, 0, 1), vec3(-2, 0, 2), vec3(-1, 0, 1));

const float step_w = 1.0/width;
const float step_h = 1.0/height;

const vec2 offset[9] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h), 
						 vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0), 
						 vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) };

void main()
{
	int i = 0;
	vec4 sum = vec4(0.0);

	if(gl_TexCoord[0].s < 0.495)
	{
		for( i=0; i<9; i++ )
		{
			vec4 tmp = texture2D(scene, gl_TexCoord[0].st + offset[i]);
			sum += tmp * kernel[i];
		}
	}
	else if( gl_TexCoord[0].s > 0.505 )
	{
		sum = texture2D(scene, gl_TexCoord[0].xy);
	}
	else
	{
		sum = vec4(1.0, 0.0, 0.0, 1.0);
	}

	gl_FragColor = sum;
}