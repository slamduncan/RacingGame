varying vec4 vert_pos;
varying float depth;

void main()
{
	//float depth = vert_pos.z / vert_pos.w ;
	//depth = depth * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth;
	float moment2 = depth * depth;

	// Adjusting moments (this is sort of bias per pixel) using derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy) ;

	//gl_FragColor = vec4( moment1,moment2, 0.0, 0.0 );
	gl_FragColor = vec4( depth, depth, depth, 1.0);
}