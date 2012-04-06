varying vec3 normal;
varying float depth;
varying vec4 vert_pos;

varying vec2 uv;

float near = 1.0;
float far = 5120.0;

void main()
{
    normal = normalize(gl_NormalMatrix * gl_Normal);	// compute the normal
	vec4 viewPos = gl_ModelViewMatrix * gl_Vertex; // this will transform the vertex into eyespace
    depth = (-viewPos.z-near)/(far-near); // will map near..far to 0..1
    uv = gl_MultiTexCoord1.xy;
	
	gl_Position = ftransform();
	vert_pos = gl_Position;
}