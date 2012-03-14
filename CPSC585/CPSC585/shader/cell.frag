varying vec3 normal;
varying vec3 view;
varying vec3 light;

uniform sampler2D cel;

void main()
{
	float NdotL = dot(normal, light);
	float NdotV = dot(normal, -view);
	
	vec4 color = texture2D(cel, vec2(NdotL, NdotV));
	gl_FragColor = color;
	//gl_FragColor = vec4(-view, 1.0);
}