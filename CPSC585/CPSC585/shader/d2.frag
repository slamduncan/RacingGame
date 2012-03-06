varying vec4 vert_pos;
varying float depth;

void main()
{
	float depth1 = vert_pos.z / vert_pos.w ;
	//depth1 = depth1 * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth1;
	float moment2 = depth1 * depth1;

	// Adjusting moments (this is sort of bias per pixel) using derivative
	float dx = dFdx(depth1);
	float dy = dFdy(depth1);
	moment2 += 0.25*(dx*dx+dy*dy) ;

	gl_FragColor = vec4( moment1,moment2, 0.0, 0.0 );
	//gl_FragColor = vec4( 1.0, 1.0, 0.25, 1.0);
}