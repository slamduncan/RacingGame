uniform sampler2D ShadowMap;

varying vec4 ShadowCoord;

vec4 ShadowCoordPostW;

float chebyshevUpperBound(float distance)
{
	vec2 moments = texture2D(ShadowMap,ShadowCoordPostW.xy).rg;
	
	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance < moments.x)
		return 1.0;

	return 0.0;
		
	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.02);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}


void main()
{	
	ShadowCoordPostW = ShadowCoord / ShadowCoord.w;
	ShadowCoordPostW += 0.00005;

	float shadow = chebyshevUpperBound(ShadowCoordPostW.z);

	gl_FragColor = vec4(vec3(shadow), 1.0);
	//gl_FragColor = vec4(vec3(shadow), 1.0);
  
}
