-- global variables
i=0;
k=1;

-- MUST(if we use globals): this way you tell the program how many global variables you wanna store/restore
EMITTER_SET_GLOBAL_PARAMS_COUNT(2);

-- MUST: function for restoring global variables
function onBegin(...)
	i=arg[1];
	k=arg[2];
end;

-- MUST: here we send out our variables so program can store them for next frame
function onEnd()
	return i,k;
end;

function onUpdate()	
	i=i+1;
	-- EMITTER_UPDATE_POSITION(0.0,0.005*k,0.0);	
	
	if i>=1000 then
		i=0;
		k=k*-1;
	end;
end;