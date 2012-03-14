varying vec3 normal;
varying vec3 view;
varying vec3 light;

void main()
{	
    normal = normalize(gl_NormalMatrix * gl_Normal);	// compute the normal
	
    vec3 vVertex = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);	// compute the current vertex position

    light = normalize(vec3(gl_LightSource[0].position.xyz - vVertex));	// compute the light vector
    view = normalize(-vVertex);	// compute the view vector to the vertex

    gl_Position = ftransform();
}