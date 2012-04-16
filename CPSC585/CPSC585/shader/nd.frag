varying vec3 normal;
varying float depth;

void main()
{
	//float depth = gl_FragCoord.z;
	//float depth2 = depth*depth;
	
	gl_FragColor = vec4(normal, depth);
}
