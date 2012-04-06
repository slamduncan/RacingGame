varying vec4 v_position;
varying float linearDepth;

void main()
{
	//float depth = v_position.z / v_position.w;

	
	float depth = linearDepth * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth;
	float moment2 = depth * depth;

	// Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy);
	
	float bd = linearDepth + 0.1;
	
	//gl_FragColor = vec4( moment1,moment2, 0.0, 1.0 );
	gl_FragColor = vec4(vec3(bd), 1.0 );
}