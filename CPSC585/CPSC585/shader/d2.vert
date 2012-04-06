varying vec4 v_position;
varying float linearDepth;

void main()
{
		vec4 viewPos = gl_ModelViewMatrix * gl_Vertex;
		
		linearDepth = (-viewPos.z-1.0)/(5120.0-1.0);

		gl_Position = ftransform();
		v_position = gl_Position;
}