uniform sampler2D ShadowMap;

varying vec4 ShadowCoord;

vec4 ShadowCoordPostW;

//
// Input: the current fragments distance from the camera
//
float chebyshevUpperBound(float distance)
{
	// moments.x stores the current depth
	// moments.y stores the variance based on the depth
	vec2 moments = texture2D(ShadowMap,ShadowCoordPostW.xy).rg;
	
	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance < moments.x)
	{
		return 1.0;
	}
	
	float mean = moments.x;
	float variance = moments.y - (moments.x*moments.x);
	
	float p_max = variance/(variance + ((distance - mean)*(distance - mean)));
	
	return p_max;
	
	
/*	
	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.002);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	p_max = clamp(p_max, 0.0, 1.0);
	
	return p_max;
*/
}


void main()
{	
	ShadowCoordPostW = ShadowCoord / ShadowCoord.w;
	ShadowCoordPostW += 0.0005;

	float shadow = chebyshevUpperBound(ShadowCoordPostW.z);

	gl_FragColor = vec4(vec3(shadow), 1.0);
	//gl_FragColor = vec4(vec3(shadow), 1.0);
  
}
