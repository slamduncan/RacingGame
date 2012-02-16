varying float depth;

float near = 1.0;
float far = 1024.0;

void main()
{
    vec4 viewPos = gl_ModelViewMatrix * gl_Vertex; // this will transform the vertex into eyespace
    depth = (-viewPos.z-near)/(far-near); // will map near..far to 0..1
    gl_Position = ftransform();
}