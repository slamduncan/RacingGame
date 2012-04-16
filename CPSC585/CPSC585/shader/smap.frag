uniform sampler2D ShadowMap;
varying vec4 ShadowCoord;
vec4 ShadowCoordPostW;

//
// Input: the current fragments distance from the camera
//
float chebyshevUpperBound(float distance)
{


	//ShadowCoordPostW.x = 1.0 - ShadowCoordPostW.x;
	//ShadowCoordPostW.y = 1.0 - ShadowCoordPostW.y;
	
	vec2 moments = texture2D(ShadowMap,ShadowCoordPostW.xy).rg;
	
	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance < moments.x)
		return 1.0 ;

	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.2);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
	
}


void main()
{	
	ShadowCoordPostW = ShadowCoord / ShadowCoord.w;
	//ShadowCoordPostW += 0.0075;
	//ShadowCoordPostW += 0.0000005;

	float shadow = chebyshevUpperBound(ShadowCoordPostW.z);

	vec3 lit = vec3(shadow);
	
	
	
	
	
	
	
	
	gl_FragColor = vec4(vec3(shadow), 1.0);
	//gl_FragColor = vec4(vec3(shadow), 1.0);
  
}
