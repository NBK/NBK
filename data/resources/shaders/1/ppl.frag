varying vec4 ambientGlobal,vertex;
varying vec3 normal,tangent,binormal;
uniform sampler2D tex,nrm;
uniform int numberOfLights;

void main()
{
	vec3 ct,cf,n,aux,spec;
	vec3 R;
	float NdotL,at,af,att,dist;
	vec4 texel,diffuse,ambient,ecPos,speccomp;
	vec4 color = ambientGlobal;
	float shininess = 10.0;
	vec3 viewvec;
	
	speccomp = vec4(0.5,0.5,0.5,0);
	
	n = normalize(normal);
	vec3 bump = normalize(texture2D(nrm, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
	n += bump.x*tangent + bump.y*binormal;
	
	ecPos = vertex;
	
	spec = vec3(0,0,0);
	viewvec = -normalize(ecPos.xyz);
	
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
		
		R = reflect(-aux,n);
		spec += gl_LightSource[0].specular.rgb * speccomp.rgb * pow(max(dot(R,viewvec), 0.0), shininess);
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
			att = 1.0/(gl_LightSource[1].constantAttenuation+gl_LightSource[1].linearAttenuation*dist+gl_LightSource[1].quadraticAttenuation*dist*dist);
			
			if (dist>0.3){att*=att*att*att;}
			color += att * (diffuse * NdotL + ambient);	
		}
		
		R = reflect(-aux,n);
		spec += gl_LightSource[1].specular.rgb * speccomp.rgb * pow(max(dot(R,viewvec), 0.0), shininess);
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
			att = 1.0/(gl_LightSource[2].constantAttenuation+gl_LightSource[2].linearAttenuation*dist+gl_LightSource[2].quadraticAttenuation*dist*dist);
			
			if (dist>0.3)
			{
				att*=att*att*att;
			}
			color += att * (diffuse * NdotL + ambient);	
		}
		
		R = reflect(-aux,n);
		spec += gl_LightSource[2].specular.rgb * speccomp.rgb * pow(max(dot(R,viewvec), 0.0), shininess);
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
			att = 1.0/(gl_LightSource[3].constantAttenuation+gl_LightSource[3].linearAttenuation*dist+gl_LightSource[3].quadraticAttenuation*dist*dist);
			
			if (dist>0.3){att*=att*att*att;}
			color += att * (diffuse * NdotL + ambient);	
		}

		R = reflect(-aux,n);
		spec += gl_LightSource[3].specular.rgb * speccomp.rgb * pow(max(dot(R,viewvec), 0.0), shininess);		
	}
	
	
	
	cf = color.rgb;
	af = color.a;
	
	texel = texture2D(tex,gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;
	
	//gl_FragColor =vec4(ct * cf, at * af);	
	
	//distance fog
	float fog = (ecPos.z*ecPos.z)/9;
	gl_FragColor = (1-fog)*vec4(ct * cf + spec, at * af);	
}

