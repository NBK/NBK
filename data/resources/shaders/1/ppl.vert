varying vec4 ambientGlobal,vertex;
varying vec3 normal,tangent,binormal;
attribute vec3 vTangent;

void main()
{	
	vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0)); 
	
	if(length(c1)>length(c2))
	{
		tangent = c1;	
	}
	else
	{
		tangent = c2;	
	}
	
	tangent = normalize(tangent);
	
	binormal = cross(gl_Normal, tangent); 
	binormal = normalize(binormal);

	normal = normalize(gl_NormalMatrix * gl_Normal);
	tangent = normalize(gl_NormalMatrix * tangent);
	binormal = normalize(gl_NormalMatrix * binormal);

	
	vertex = gl_ModelViewMatrix * gl_Vertex;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_TexCoord[0] = gl_MultiTexCoord0;		
	gl_Position = ftransform();
} 