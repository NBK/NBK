-- a function that receives particle ID and number of particles.
function onParticleInit(particleID,particleCount)	

	PARTICLE_SET_COLOR(0.0,0.8,0.0);
	life = 1500;
	PARTICLE_SET_LIFE_PROPERTIES(life,1.0,math.random(life));
	PARTICLE_SET_REBORN(1);
	-- third parameter tells interpeter to connect alpha with particles life.  if it's < 0.0f then it's connected else a valid second param should be set
	PARTICLE_SET_ALPHA_PROPERTIES(1.0,0.0,-1.0);
	PARTICLE_SET_RESET_ALPHA(0);
	PARTICLE_SET_SIZE_PROPERTIES(1.0,0.0,1.0);
	--PARTICLE_SET_SPAWN_PROPERTIES(particleID/10.0);
end;