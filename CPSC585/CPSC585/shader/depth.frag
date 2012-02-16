varying float depth;

void main()
{
	//float depth = gl_FragCoord.z;
	float depth2 = depth*depth;
	
	gl_FragColor = vec4(depth, depth, depth, 1.0);
}
