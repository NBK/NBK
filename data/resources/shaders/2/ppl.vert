varying vec3 normal;
varying vec4 ecPosition;

void main()
{
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	//eye coordinate position
	ecPosition = gl_ModelViewMatrix * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
