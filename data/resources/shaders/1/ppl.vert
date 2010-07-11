varying vec4 ambientGlobal,vertex;
varying vec3 normal,tangent,binormal;
attribute vec3 vTangent;

void main()
{	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	tangent = normalize(gl_NormalMatrix * vTangent);
	binormal = cross(normal,tangent);
	
	vertex = gl_ModelViewMatrix * gl_Vertex;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_TexCoord[0] = gl_MultiTexCoord0;		
	gl_Position = ftransform();
} 