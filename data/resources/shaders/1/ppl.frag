varying vec4 ambientGlobal,vertex;
varying vec3 normal;
uniform sampler2D tex;
uniform int numberOfLights;

void main()
{
	vec3 ct,cf,n,aux;
	float NdotL,at,af,att,dist;
	vec4 texel,diffuse,ambient,ecPos;
	vec4 color = ambientGlobal;	
	
	n = normalize(normal);
	
	ecPos = vertex;
	
	if (numberOfLights>=1)
	{
		aux = vec3(gl_LightSource[0].position-ecPos);							
		dist = length(aux);
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;	
		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;	
		
		NdotL = max(dot(n,normalize(aux)),0.1);
	
		if (NdotL > 0.0) 
		{
			att = 1.0/(gl_LightSource[0].constantAttenuation+gl_LightSource[0].linearAttenuation*dist+gl_LightSource[0].quadraticAttenuation*dist*dist);
			
			if (dist>0.3){att*=att*att*att;}
			
			color += att * (diffuse * NdotL + ambient);	
		}	
	}		
	
	if (numberOfLights>=2)
	{
		aux = vec3(gl_LightSource[1].position-ecPos);		
		dist = length(aux);
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;	
		ambient = gl_FrontMaterial.ambient * gl_LightSource[1].ambient;	
		
		NdotL = max(dot(n,normalize(aux)),0.1);
	
		if (NdotL > 0.0) 
		{
			att = 1.0/(gl_LightSource[0].constantAttenuation+gl_LightSource[0].linearAttenuation*dist+gl_LightSource[0].quadraticAttenuation*dist*dist);
			
			if (dist>0.3){att*=att*att*att;}
			color += att * (diffuse * NdotL + ambient);	
		}	
	}	
	
	if (numberOfLights>=3)
	{
		aux = vec3(gl_LightSource[2].position-ecPos);		
		dist = length(aux);
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[2].diffuse;	
		ambient = gl_FrontMaterial.ambient * gl_LightSource[2].ambient;	
		
		NdotL = max(dot(n,normalize(aux)),0.1);
	
		if (NdotL > 0.0) 
		{
			att = 1.0/(gl_LightSource[0].constantAttenuation+gl_LightSource[0].linearAttenuation*dist+gl_LightSource[0].quadraticAttenuation*dist*dist);
			
			if (dist>0.3)
			{
				att*=att*att;
			}
			color += att * (diffuse * NdotL + ambient);	
		}	
	}	
	
	if (numberOfLights>=4)
	{
		aux = vec3(gl_LightSource[3].position-ecPos);		
		dist = length(aux);
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[3].diffuse;	
		ambient = gl_FrontMaterial.ambient * gl_LightSource[3].ambient;	
		
		NdotL = max(dot(n,normalize(aux)),0.1);
	
		if (NdotL > 0.0) 
		{
			att = 1.0/(gl_LightSource[0].constantAttenuation+gl_LightSource[0].linearAttenuation*dist+gl_LightSource[0].quadraticAttenuation*dist*dist);
			
			if (dist>0.3){att*=att*att*att;}
			color += att * (diffuse * NdotL + ambient);	
		}	
	}	
	
	cf = color.rgb;
	af = color.a;
	
	texel = texture2D(tex,gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af);	
}

