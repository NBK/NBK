function onParticleInit(particleID,particleCount)
	PARTICLE_SET_COLOR(1.0,1.0,1.0);
	PARTICLE_SET_LIFE_PROPERTIES(1.0,0.0,1.0);
	PARTICLE_SET_SIZE_PROPERTIES(1.5,0.0,0.0);
	PARTICLE_SET_REBORN(1);
	PARTICLE_SET_RESET_ALPHA(1);
	-- third parameter tells interpeter to connect alpha with particles life.  if it's < 0.0f then it's connected else a valid second param should be set
	PARTICLE_SET_ALPHA_PROPERTIES(1.0,0.0,-1.0);
end;