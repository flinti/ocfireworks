

global func CreateFireworkEffect(proplist fweffect)
{
	var type = fweffect.type;
	
	if(fweffect.sound)
	{
		var dummy = CreateObject(Dummy);
		dummy->Sound(fweffect.sound);
		dummy->RemoveObject();
	}
	
	if(type == FWE_Emit)
		this->Call(FireworkEffects.DoEffect_Emit, fweffect);
	else if(type == FWE_SimpleFireworks)
		Fireworks();
	else if(type == FWE_Die)
		{}
	else
		DebugLog("DoFireworkEffect: undefined effect type %d", type);
		
	if(fweffect.die)
		RemoveObject();
}


global func AddFireworkTrail(proplist trail)
{
	var fx = CreateEffect(FireworkFx_FireworkTrail, 300, 1, trail);
	
	var type = trail.type;
	if(type == FWT_Glow)
	{
		fx.trailFunc = FireworkEffects.DoTrail_Glow;
		fx.trailParticleType = "Magic";
		fx.trailParticle = Particles_Colored(Particles_Glimmer(), trail.color, trail.color2);
		fx.trailParticle.Size = PV_Linear(trail.size, 0);
		fx.trailParticle.DampingY = 600;
	}
	else
		return DebugLog("AddFireworkTrail: undefined trail type %d", type);
}

global func RemoveFireworkTrail(proplist trail)
{
	var ret = 0;
	while(RemoveEffect("FireworkTrail", this))
		++ret;
	return !!ret;
}

static const FireworkFx_FireworkTrail = new Effect
{
	Name = "FireworkTrail",
	
	Construction = func(proplist trail)
	{
		this.trail = trail;
	},
	
	//Play the trail's sound looped and set the lights
	Start = func(int temp)
	{
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, 1);
			
		this.oldLightColor = Target->GetLightColor();
		this.oldLightRange = Target->GetObjectVal("LightRange");
		this.oldLightFadeoutRange = Target->GetObjectVal("LightFadeoutRange");
		
		Target->SetLightRange(this.trail.lightRange, this.trail.lightFadeoutRange);
		Target->SetLightColor(this.trail.lightColor ?? this.trail.color);
	},

	// Stop the trails sound loop and reset the lights as this firework has vanished
	Stop = func(int temp)
	{
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, -1);
		Target->SetLightRange(this.oldLightRange, this.oldLightFadeoutRange);
		Target->SetLightColor(this.oldLightColor);
	},

	// Do the Trail tick every frame
	Timer = func(int time)
	{
		Target->Call(this.trailFunc, this);
		
		if(this.trail.float)
			Target->SetYDir(Target->GetYDir(10000) - (100 - this.trail.gravityDamping) * GetGravity(), 10000);
		
		if(this.trail.accel)
		{
			var newSpeed = Target->GetSpeed(1000) + this.trail.accel /* * dt (= 1)*/;
			if(newSpeed < 0)
				Target->SetVelocity(nil, 0);
			else
				Target->SetVelocity(nil, newSpeed, nil, 1000);
		}
		
		return FX_OK;
	},
};




