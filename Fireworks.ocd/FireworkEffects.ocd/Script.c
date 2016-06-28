/**
 * Firework Effects
 * 
 * Contains effect prototypes
 * 
 * Contains effect functions. These have to be called from object context.
 * These are internals, for public interfaces see /Basic.ocd/FireworkBomb.ocd and /System.ocg/Firework.c
 * 
 * @author Flinti
*/

/* -- Effect prototypes -- */

static const FW_Effect_Die = { 
								Name = "Effect_Die",
								execfn = FireworkEffects.None, 
								die = true,
							};
static const FW_Effect_SimpleFireworks = {
								Name = "Effect_SimpleFireworks", 
								execfn = FireworkEffects.SimpleFireworks, 
								sound = "Firework::MediumBlast?",
							};
static const FW_Effect_Emit = { 
								Name = "Effect_Emit", 
								execfn = FireworkEffects.Emit, 
								sound = "Firework::MediumBlast?", 
								emitted = [], 
								amount = nil,
								flash = true,
							};
static const FW_Effect_EmitStars = { 
								Name = "Effect_EmitStars", 
								execfn = FireworkEffects.EmitStars, 
								sound = "Firework::MediumBlast?", 
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

public func None(proplist fweffect)
{
}

/* -- Simple Fireworks -- */

public func SimpleFireworks(proplist fweffect)
{
	Fireworks();
}


/* -- Emit -- */

public func Emit(proplist fweffect)
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
			
				obj.fireworkData = curEmitted.fireworkData;
				obj->SetFused(this);
			}
		}
	}
}

/* -- Emit Stars -- */

local EmitStars_Types = {};


public func EmitStars(proplist fweffect)
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
			
			var type = FireworkEffects.EmitStars_Types[curEmitted.type ?? "standard"];
			var particle = type.particle;
			
			var speed = curEmitted.speed, speedx = 0, speedy = 0;
			if(speed)
			{
				speed = FW_Distribution->GetValue(speed, i, amount, emittedIndex);

				speedx = Sin(angle, speed) + GetXDir() * curEmitted.relativeVelocity;
				speedy = -Cos(angle, speed) + GetYDir() * curEmitted.relativeVelocity;
			}
			
			var lifetime = curEmitted.lifetime ?? 1;
			lifetime = FW_Distribution->GetValue(lifetime);
			
			if(curEmitted.color)
			{
				particle.R = GetRGBaValue(curEmitted.color, 1);
				particle.G = GetRGBaValue(curEmitted.color, 2);
				particle.B = GetRGBaValue(curEmitted.color, 3);
			}
			particle.Size = curEmitted.size ?? particle.Size;
			
			CreateParticle(type.name, Sin(angle, radius), -Cos(angle, radius), speedx, speedy, lifetime, particle, type.amount);
		}
	}
}

/* -- Glow -- */

public func Glow(proplist fweffect)
{
	var dummy = CreateObject(Dummy);
	dummy->SetLightRange(fweffect.lightRange, fweffect.lightFadeoutRange);
	dummy->SetLightColor(fweffect.color);
	ScheduleCall(dummy, Global.RemoveObject, fweffect.duration ?? 1);
}






func Definition(id def)
{
	EmitStars_Types = {
		standard = {
			name = "StarSpark",
			amount = 1,
			particle = {
				R = 255,
				G = 255,
				B = 255,
				Alpha = 255,
				Size = 3,
				Rotation = PV_Direction(),
				ForceX = PV_Wind(40),
				ForceY = PV_Gravity(20),
				DampingX = 950, 
				DampingY = 950,
				BlitMode = GFX_BLIT_Additive,
				CollisionVertex = 500,
				OnCollision = PC_Stop(),
			},
		},
	};
}



