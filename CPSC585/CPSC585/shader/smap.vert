// Used for shadow lookup
varying vec4 ShadowCoord;



varying vec3 normal, lightDir, eyeVec;

void main()
{
     	
		
		
		ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;
		gl_Position = ftransform();
		//gl_Position = gl_ModelViewMatrix * gl_Vertex;
		gl_FrontColor = gl_Color;
		
		vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
		lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
		eyeVec = -vVertex;
		normal = gl_NormalMatrix * gl_Normal;
}
