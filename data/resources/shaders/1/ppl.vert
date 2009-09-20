varying vec4 ambientGlobal,vertex;
varying vec3 normal;

void main()
{	
	normal = normalize(gl_NormalMatrix * gl_Normal);		
	vertex = gl_ModelViewMatrix * gl_Vertex;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_TexCoord[0] = gl_MultiTexCoord0;		
	gl_Position = ftransform();
} 