sampler2D tex;

varying vec3 normal;
varying vec4 ecPosition;

void main()
{
	float att;//tmp
	vec3 aux;
	float dist;//tmp
	
	aux = vec3(gl_LightSource[0].position - ecPosition);//tmp
	dist = length(aux);//tmp
	
	vec4 Idiff = gl_FrontLightProduct[0].diffuse/*vec4(0.0, 0.0, 1.0, 1.0)*/;
	att = 1.0 / (gl_LightSource[0].constantAttenuation +
			gl_LightSource[0].linearAttenuation * dist +
			gl_LightSource[0].quadraticAttenuation * dist * dist);
	
	
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	vec4 pixelFinalColor = /*Idiff **/ att /** gl_FrontMaterial.ambient*/;
	pixelFinalColor[3] = 1.0;
	
	vec4 texcolor = texture2D(tex,gl_TexCoord[0].st);
	
	gl_FragColor = pixelFinalColor * texcolor;
}
