/**
	Firework Effects
	Contains trail and effect functions. These have to be called from object context.

	@author Flinti
*/


/* -- Effect Functions -- */

protected func DoEffect_Emit(proplist fweffect)
{
	/*
	amount
	emitted []
	angle
	radius
	rotation
	rotationSpeed
	rotationPrecision
	speed
	speedPrecision
	*/
	var length = GetLength(fweffect.emitted);
	var amount = fweffect.amount ?? length;
	for(var i = 0; i < amount; ++i)
	{
		var curEmitted = fweffect.emitted[i % length];
		
		if(curEmitted.fireworkData)
		{
			var angle = curEmitted.angleSet ?? [0, 359];
			angle = RandomX(angle[0], angle[1]) + GetR();
			
			var radius = curEmitted.radiusSet ?? [0, 0];
			radius = RandomX(radius[0], radius[1]);
			
			var obj = CreateObject(FireworkBomb, Cos(angle, radius), -Sin(angle, radius));
			obj->SetController(GetController());
			
			var rotation = curEmitted.rotationSet ?? [0, 359];
			obj->SetR(RandomX(rotation[0], rotation[1]));
			
			var rotationSpeed = curEmitted.rotationSpeedSet ?? [0, 0];
			obj->SetRDir(RandomX(rotationSpeed[0], rotationSpeed[1]), curEmitted.rotationPrecision);
			
			var speed = curEmitted.speedSet ?? [0, 0];
			speed = RandomX(speed[0], speed[1]);
			obj->SetXDir(Cos(angle, speed, curEmitted.speedPrecision) + GetXDir(curEmitted.speedPrecision), curEmitted.speedPrecision);
			obj->SetYDir(-Sin(angle, speed, curEmitted.speedPrecision) + GetYDir(curEmitted.speedPrecision), curEmitted.speedPrecision);
			
			obj.fireworkData = curEmitted.fireworkData;
			obj->SetFused(this);
		}
	}
}

/* -- Trail Functions -- */

protected func DoTrail_Glow(effect fx)
{
	CreateParticle(fx.trailParticleType, 0, RandomX(-1, 1), RandomX(-1, 1), 0, PV_Random(40, 50), fx.trailParticle);
	CreateParticle(fx.trailParticleType, 0, RandomX(-2, 2), RandomX(-1, 1), 0, PV_Random(40, 50), fx.trailParticle);
	CreateParticle(fx.trailParticleType, 0, RandomX(-2, 2), RandomX(-1, 1), 0, PV_Random(40, 50), fx.trailParticle);
}




