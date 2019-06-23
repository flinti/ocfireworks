/**
 * Firework Effects
 * 
 * Contains effect prototypes
 * 
 * Contains effect functions. These have to be called from object context.
 * These are internals, for public interfaces see /Basic.ocd/FireworkBase.ocd and /System.ocg/Firework.c
 * 
 * @author Flinti
*/

/* -- Effect prototypes -- */

static const FW_Effect_Die = { 
								Name = "Effect_Die",
								execfn = FireworkEffects.None, 
								die = true,
							};
static const FW_Effect_Whizz = { 
								Name = "Effect_Whizz",
								execfn = FireworkEffects.Whizz, 
								sound = "Firework::SmallBang",
								pitch = [-5, 20],
								angle = 90,
								speed = 10,
							};
static const FW_Effect_SimpleFireworks = {
								Name = "Effect_SimpleFireworks", 
								execfn = Global.Fireworks, 
								sound = "Firework::MediumBlast?",
								pitch = [-5, 20],
							};
static const FW_Effect_Emit = { 
								Name = "Effect_Emit", 
								execfn = FireworkEffects.Emit, 
								sound = "Firework::MediumBlast?", 
								pitch = [-5, 20],
								emitted = [], 
								amount = nil,
								flash = true,
							};
static const FW_Effect_EmitSingle = { 
								Name = "Effect_EmitSingle", 
								execfn = FireworkEffects.EmitSingle, 
								sound = "Firework::MediumBlast?", 
								pitch = [-5, 20],
								flash = true,
							};
static const FW_Effect_EmitStars = { 
								Name = "Effect_EmitStars", 
								execfn = FireworkEffects.EmitStars, 
								sound = "Firework::MediumBlast?", 
								pitch = [-5, 20],
								emitted = [], 
								amount = nil,
								flash = true,
							};
static const FW_Effect_EmitStarsRandom = { 
								Name = "Effect_EmitStarsRandom", 
								execfn = FireworkEffects.EmitStarsRandom, 
								sound = "Firework::MediumBlast?", 
								pitch = [-5, 20],
								emitted = [], 
								amount = nil,
								flash = true,
							};
static const FW_Effect_Glow = {
								Name = "Effect_Glow", 
								execfn = FireworkEffects.Glow,
								flash = false,
								color = -1,
								lightRange = 20,
								lightFadeoutRange = 200,
								duration = 20,
							};	





/* -- None -- */

private func None(proplist fweffect)
{
}

private func Whizz(proplist fweffect)
{
	var angle = FW_Distribution->GetValue(fweffect.angle);
	var speed = FW_Distribution->GetValue(fweffect.speed);
	var rotationSpeed = FW_Distribution->GetValue(fweffect.rotationSpeed);
	if(Random(2))
		angle = -angle;
	if(Random(2))
		rotationSpeed = -rotationSpeed;
	if(speed)
	{
		SetXDir(GetXDir(fweffect.speedPrecision) + Sin(angle, speed), fweffect.speedPrecision);
		SetYDir(GetYDir(fweffect.speedPrecision) - Cos(angle, speed), fweffect.speedPrecision);
	}
	if(rotationSpeed)
	{
		SetRDir(GetRDir(fweffect.rotationPrecision) + rotationSpeed, fweffect.rotationPrecision);
	}
}

/* -- Simple Fireworks -- */

// na


/* -- Emit -- */

private func Emit(proplist fweffect)
{
	var length = GetLength(fweffect.emitted);
	var amount = fweffect.amount ?? length;
	
	for(var i = 0; i < amount; ++i)
	{
		var emittedIndex = i % length;
		var curEmitted = fweffect.emitted[emittedIndex];
		
		if(curEmitted.fireworkData)
		{
			var angle = curEmitted.angle ?? [0, 359];
			angle = FW_Distribution->GetValue(angle, i, amount, emittedIndex, true) + GetR();
			
			var radius = curEmitted.radius;
			if(radius)
				radius = FW_Distribution->GetValue(radius, i, amount, emittedIndex);
			
			var obj = CreateObject(FireworkBomb, Sin(angle, radius), -Cos(angle, radius));
			
			var rotation = curEmitted.rotation ?? [0, 359];
			obj->SetR(FW_Distribution->GetValue(rotation, i, amount, emittedIndex, true));
			
			var rotationSpeed = curEmitted.rotationSpeed;
			if(rotationSpeed)
				obj->SetRDir(FW_Distribution->GetValue(rotationSpeed, i, amount, emittedIndex), curEmitted.rotationPrecision);
			
			var speed = curEmitted.speed;
			if(speed)
			{
				speed = FW_Distribution->GetValue(speed, i, amount, emittedIndex);
				//var rnd3d = RandomX(-90, 90);
				obj->SetXDir(Sin(angle, speed)/**Cos(rnd3d, 100) /100*/ + GetXDir(curEmitted.speedPrecision) * curEmitted.relativeVelocity, curEmitted.speedPrecision);
				obj->SetYDir(-Cos(angle, speed)/**Cos(rnd3d, 100) /100*/ + GetYDir(curEmitted.speedPrecision) * curEmitted.relativeVelocity, curEmitted.speedPrecision);
			
				obj.fireworkData = new curEmitted.fireworkData {};
				obj.fireworkData.distributionData = fweffect.distributionData;
				obj->SetFused(this);
			}
		}
	}
}

/* -- Emit Single -- */

private func EmitSingle(proplist fweffect)
{
	var emitted = fweffect.emitted;
	if(!emitted)
		return;
	
	var angle = fweffect.angle ?? [0, 359];
	angle = FW_Distribution->GetValue(angle, 0, 1, 0, true) + GetR();
	
	var radius = fweffect.radius;
	if(radius)
		radius = FW_Distribution->GetValue(radius, 0, 1, 0);
	
	var obj = CreateObject(FireworkBomb, Sin(angle, radius), -Cos(angle, radius));
	
	var rotation = fweffect.rotation ?? [0, 359];
	obj->SetR(FW_Distribution->GetValue(rotation, 0, 1, 0, true));
			
	var rotationSpeed = fweffect.rotationSpeed;
	if(rotationSpeed)
		obj->SetRDir(FW_Distribution->GetValue(rotationSpeed, 0, 1, 0), fweffect.rotationPrecision);
	
	var speed = fweffect.speed;
	if(speed)
	{
		speed = FW_Distribution->GetValue(speed, 0, 1, 0);
		obj->SetXDir(Sin(angle, speed) + GetXDir(fweffect.speedPrecision) * fweffect.relativeVelocity, fweffect.speedPrecision);
		obj->SetYDir(-Cos(angle, speed) + GetYDir(fweffect.speedPrecision) * fweffect.relativeVelocity, fweffect.speedPrecision);
	
		obj.fireworkData = new fweffect.fireworkData {};
		obj.fireworkData.distributionData = fweffect.distributionData;
		obj->SetFused(this);
	}
}

/* -- Emit Stars -- */


private func EmitStars(proplist fweffect)
{
	var length = GetLength(fweffect.emitted);
	var amount = fweffect.amount ?? length;
	
	for(var i = 0; i < amount; ++i)
	{
		var emittedIndex = i % length;
		var curEmitted = fweffect.emitted[emittedIndex];
		
		if(curEmitted.type)
		{
			var angle = curEmitted.angle ?? [0, 359];
			angle = FW_Distribution->GetValue(angle, i, amount, emittedIndex, true) + GetR();
			
			var radius = curEmitted.radius;
			if(radius)
				radius = FW_Distribution->GetValue(radius, i, amount, emittedIndex);
			
			//var type = FireworkEffects.EmitStars_Types[curEmitted.type ?? "standard"];
			//var particle = new type.particle {};
			var particle = FireworkParticles->GetParticle(curEmitted.type ?? "standard", curEmitted);
			
			var speed = curEmitted.speed, speedx = 0, speedy = 0;
			if(speed)
			{
				speed = FW_Distribution->GetValue(speed, i, amount, emittedIndex);

				speedx = Sin(angle, speed) + GetXDir() * curEmitted.relativeVelocity;
				speedy = -Cos(angle, speed) + GetYDir() * curEmitted.relativeVelocity;
			}
			
			var lifetime = curEmitted.lifetime ?? 1;
			lifetime = FW_Distribution->GetValue(lifetime);
			
			var color = curEmitted.color;
			if(fweffect.distributionData)
				color = FW_Distribution->GetValue(color, fweffect.distributionData.shotIndex, fweffect.distributionData.shotAmount, fweffect.distributionData.shotTypeIndex);
			if(color)
			{
				particle.R = GetRGBaValue(color, 1);
				particle.G = GetRGBaValue(color, 2);
				particle.B = GetRGBaValue(color, 3);
			}
			//particle->~init(curEmitted);
			
			CreateParticle(particle.name, Sin(angle, radius), -Cos(angle, radius), speedx, speedy, lifetime, particle, particle.stdAmount);
		}
	}
}

private func EmitStarsRandom(proplist fweffect)
{
	var length = GetLength(fweffect.emitted);
	var amount = fweffect.amount / length;
	
	for(var i = 0; i < length; ++i)
	{
		var curEmitted = fweffect.emitted[i];
		//var type = FireworkEffects.EmitStars_Types[curEmitted.type ?? "standard"];
		//var particle = new type.particle {};
		var particle = FireworkParticles->GetParticle(curEmitted.type ?? "standard", curEmitted);
		
		var color = curEmitted.color;
		if(fweffect.distributionData)
				color = FW_Distribution->GetValue(color, fweffect.distributionData.shotIndex, fweffect.distributionData.shotAmount, fweffect.distributionData.shotTypeIndex);
		if(color)
		{
			particle.R = GetRGBaValue(color, 1);
			particle.G = GetRGBaValue(color, 2);
			particle.B = GetRGBaValue(color, 3);
		}
		
		var radius = FW_Distribution->GetParticleValueProvider(curEmitted.radius);
		var speed = FW_Distribution->GetParticleValueProvider(curEmitted.speed);
		var sin = PV_Sin(PV_Random(0, 359, 0, 1), radius);
		var cos = PV_Cos(PV_Random(0, 359, 0, 1), radius);
		var vsin = PV_Sin(PV_Random(0, 359, 0, 1), speed);
		var vcos = PV_Cos(PV_Random(0, 359, 0, 1), speed);
		
		//particle.Size = FW_Distribution->GetParticleValueProvider(curEmitted.size);
		//particle->~init(curEmitted);
		
		CreateParticle(particle.name, sin, cos, vsin, vcos, FW_Distribution->GetParticleValueProvider(curEmitted.lifetime ?? 1), particle, particle.stdAmount * amount);
	}
}

/* -- Glow -- */

private func Glow(proplist fweffect)
{
	var dummy = CreateObject(Dummy);
	dummy->SetLightRange(fweffect.lightRange, fweffect.lightFadeoutRange);
	dummy->SetLightColor(fweffect.color);
	ScheduleCall(dummy, Global.RemoveObject, fweffect.duration ?? 1);
}








