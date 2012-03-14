uniform sampler2D ranNMap;
uniform sampler2D test;

uniform sampler2D normalMap;

varying vec2 uv;	// uv coord from vertex shader

float totalStr = 2.0;	// how strong we want the SSAO to appear
float str = 1.0;	// strength
float offset = 50.0;
float fallOff = 0.000002;
float radius = 0.006;

// we want 10 samples per pixel
#define SAMPLES 10

// inverse of num samples as float
float invSamples = 1.0/10.0;

void main()
{
	// precomputed vectors of a unit sphere
	vec3 pSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),
							  vec3(-0.06533369, 0.3647007, -0.13746321),
							  vec3(-0.6539235, -0.016726388, -0.53000957),
							  vec3(0.40958285, 0.0052428036, -0.5591124),
							  vec3(-0.1465366, 0.09899267, 0.15571679),
							  vec3(-0.44122112, -0.5458797, 0.04912532),
							  vec3(0.03755566, -0.10961345, -0.33040273),
							  vec3(0.019100213, 0.29652783, 0.066237666),
							  vec3(0.8765323, 0.011236004, 0.28265962),
							  vec3(0.29264435, -0.40794238, 0.15964167));
	
	vec2 screenCoord = gl_FragCoord.xy;
	screenCoord.x = screenCoord.x/1280.0;
	screenCoord.y = screenCoord.y/720.0;
	
	// grab a random normal from our random normal map
	// used to sample the space
	vec3 rNormal = normalize((texture2D(ranNMap, screenCoord*offset).xyz*2.0) - vec3(1.0));
	
	// get the color of the current pixel
	vec4 currentPixelSample = texture2D(normalMap, screenCoord);
	
	// get the depth of the current pixel
	float currentPixelDepth = currentPixelSample.a;
	
	// current pixels coordinates in screen space
	vec3 ep = vec3(screenCoord.xy,currentPixelDepth);
	
	// current normal of the pixel in screen space
	vec3 norm = currentPixelSample.xyz;
	
	float bl = 0.0;
	float radD = radius/currentPixelDepth;
	
	vec3 ray, se, occNorm;
	float occluderDepth, depthDifference, normDiff;
	
	for(int i=0; i < SAMPLES;i++)
	{
		// get a ray from in the sphere and reflect it off the normal of the current pixel
		ray = radD*reflect(pSphere[i], rNormal);
		
		//if the ray is outside the hemisphere, we need to change the direction
		se = ep + sign(dot(ray, norm))*ray;
	
		// get the depth of an occluder
		vec4 occluderFragment = texture2D(normalMap, se.xy);
		
		// get the normal of the occluder fragment
		occNorm = occluderFragment.xyz;
		
		// if depthDifference is negative = occluder is behind current fragment
		depthDifference = currentPixelDepth-occluderFragment.a;
	
		normDiff = (1.0-dot(occNorm,norm));
		// the falloff equation, starts at falloff and is kind of 1/x^2 falling
		bl += step(fallOff,depthDifference)*normDiff*(1.0-smoothstep(fallOff,str,depthDifference));
	}
	
	float ao = 1.0-totalStr*bl*invSamples;
	//gl_FragColor = currentPixelSample;
	gl_FragColor = vec4(vec3(ao), 1.0);
}