varying vec2 uv;

void main()
{
    uv = gl_MultiTexCoord1.xy;
	
	gl_Position = ftransform();
}