function dist(x,y,z)
	return math.sqrt(x*x+y*y+z*z);
end;

function onParticleInit(particleID,particleCount)
	-- circle: x^2+y^2+z¡2 = r¡2
	
	radius = 2.0;
	
	x = 10.0;
	z = 10.0;
	y = 0.0;
		
	while dist(x,y,z)-radius/2.0>0.0 do
		x = math.random()*radius-radius/2.0;
		z = math.random()*radius-radius/2.0;
		y = 0.0;	
	end;
	
	PARTICLE_SET_START_POSITION(x,y,z);	
	dv = 100.0;
	PARTICLE_SET_MOVE_VECTOR(0.0,0.0015,0.0);
	--PARTICLE_SET_MOVE_VECTOR(x/dv,y/dv*3.0,z/dv);
	PARTICLE_SET_COLOR(0.5,0.5,0.5);
	life = 2500.0;
	PARTICLE_SET_LIFE_PROPERTIES(life,1.0,math.random(life));
	--PARTICLE_SET_MOVE_VECTOR(0.0,0.001,0.0);
	--PARTICLE_SET_LIFE_PROPERTIES(1000,0.0);
	PARTICLE_SET_SIZE_PROPERTIES(2.5,0.0,0.0);
	--PARTICLE_SET_GRAVITY(0.0,-0.000015,0.0);
	PARTICLE_SET_REBORN(1);
	PARTICLE_SET_RESET_ALPHA(1);
	-- third parameter tells interpeter to connect alpha with particles life.  if it's < 0.0f then it's connected else a valid second param should be set
	PARTICLE_SET_ALPHA_PROPERTIES(1.0,0.0,-1.0);
end;