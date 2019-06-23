/**
 * Firework Fountain Effects
 * 
 * Contains fountain effect prototypes
 * 
 * Contains fountain effect functions. These have to be called from object context.
 * These are internals, for public interfaces see /Basic.ocd/FireworkBase.ocd, /Basic.ocd/Fountain.ocd and /System.ocg/Firework.c
 * 
 * @author Flinti
*/

/* -- Effect prototypes -- */

static const FW_FountainEffect_None = { 
								Name = "FountainEffect_None", 
								initfn = nil,
								execfn = FireworkFountainEffects.None, 
							};
static const FW_FountainEffect_Sparkle = {
								Name = "FountainEffect_Sparkle", 
								initfn = FireworkFountainEffects.SparkleInit,
								execfn = FireworkFountainEffects.Sparkle, 
								color = -1, 
								lightRange = 20, 
								lightFadeoutRange = 100,
							};
static const FW_FountainEffect_LongSparks = {
								Name = "FW_FountainEffect_LongSparks",
								initfn = FireworkFountainEffects.LongSparksInit,
								execfn = FireworkFountainEffects.LongSparks, 
								color = -1, 
								lightRange = 10, 
								lightFadeoutRange = 60,
								//timer = 5,
							};
static const FW_FountainEffect_Emit = {
								Name = "FW_FountainEffect_Emit",
								initfn = FireworkFountainEffects.EmitInit,
								execfn = FireworkFountainEffects.Emit, 
								color = -1,
								timer = 5,
								emitSound = "Firework::Shot?",
								emitPitch = [-10, 20],
							};



/* -- None -- */

public func None(effect fx)
{
}


/* -- Sparkle -- */

func SparkleParticle() 
{
	return {
		Rotation = PV_Direction(),
		OnCollision = PC_Stop(),
		CollisionVertex = 500,
		BlitMode = GFX_BLIT_Additive,
		Alpha = PV_Random(1, 254, 3),
		ForceX = PV_Wind(40, PV_Random(-1,1)),
		ForceY = PV_Gravity(20, PV_Random(-1,1)),
		DampingX = 950, 
		DampingY = 950,
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(3, 10), 1000, PV_Random(8, 11)),
	};
}


public func SparkleInit(effect fx, proplist fweffect)
{
	fx.particleType = "StarSpark";
	fx.particle = Particles_Colored(SparkleParticle(), fweffect.color, fweffect.color2);
	
	fx.deviation = fweffect.deviation ?? 10;
	fx.speed = fweffect.speed ?? [0, 0];
	if(GetType(fx.speed) != C4V_Array)
		fx.speed = [fx.speed, fx.speed];
		
	var size = fweffect.size;
	if(GetType(size) == C4V_Array)
		fx.particle.Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(size[0] / 2, size[1] / 2), 500, PV_Random(size[0], size[1]), 1000, 0);
	else
		fx.particle.Size = PV_KeyFrames(0, 0, 0, 200, size / 2, 500, size, 1000, 0);
	
	fx.lifetime = FW_Distribution->GetParticleValueProvider(fweffect.lifetime ?? [20, 30]);
	
	fx.posradius = PV_Random(1, 5, nil, 1);
	fx.speedradius = PV_Random(fx.speed[0], fx.speed[1], 0, 1);
}

public func Sparkle(effect fx)
{
	var angle = GetR();
	var angle2 = -90 - angle;
	CreateParticle(fx.particleType, 
				PV_Cos(angle2, fx.posradius), //x
				PV_Sin(angle2, fx.posradius), //y
				
				PV_Cos(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //xdir
				PV_Sin(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //ydir
		fx.lifetime, fx.particle, fx.amount);
}



/* -- Long Sparks -- */

public func LongSparksInit(effect fx, proplist fweffect)
{
	var p = FireworkParticles->GetParticle("trailing", fweffect);
	fx.particleType = p.name;
	fx.particle = Particles_Colored(p, fweffect.color, fweffect.color2);
	
	fx.particle.Stretch = PV_Speed(PV_KeyFrames(0, 0, 0, 200, 2000, 1000, 4000), 500);
	
	fx.deviation = fweffect.deviation ?? 10;
	fx.speed = fweffect.speed ?? [0, 0];
	if(GetType(fx.speed) != C4V_Array)
		fx.speed = [fx.speed, fx.speed];
	
	fx.lifetime = FW_Distribution->GetParticleValueProvider(fweffect.lifetime ?? [15, 20]);
	
	fx.posradius = PV_Random(1, 5, nil, 1);
	fx.speedradius = PV_Random(fx.speed[0], fx.speed[1], 0, 1);
}

public func LongSparks(effect fx)
{
	var angle = GetR();
	var angle2 = -90 - angle;
	CreateParticle(fx.particleType, 
				PV_Cos(angle2, fx.posradius), //x
				PV_Sin(angle2, fx.posradius), //y
				
				PV_Cos(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //xdir
				PV_Sin(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //ydir
		fx.lifetime, fx.particle, fx.amount);
}

/* -- Emit -- */

public func EmitInit(effect fx, proplist fweffect)
{
	fx.chance = fweffect.chance ?? 100;
	fx.radius = fweffect.radius;
	fx.deviation = fweffect.deviation;
	fx.speed = fweffect.speed;
	fx.rotation = fweffect.rotation;
	fx.rotationSpeed = fweffect.rotationSpeed;
	fx.fireworkData = fweffect.fireworkData;
	fx.sound = fweffect.emitSound;
	fx.pitch = fweffect.emitPitch;
}

public func Emit(effect fx)
{
	if(Random(100)+1 <= fx.chance)
	{
		var angle = -90 - GetR();
		var x = Cos(angle, fx.radius), y = Sin(angle, fx.radius);
		var obj = CreateObject(FireworkBomb, x, y);
		obj->SetController(GetController());
		
		angle += RandomX(-fx.deviation, fx.deviation);
		var speed = FW_Distribution->GetValue(fx.speed);
		var xdir = Cos(angle, speed);
		var ydir = Sin(angle, speed);
		obj->SetXDir(xdir);
		obj->SetYDir(ydir);
		var rotation = FW_Distribution->GetValue(fx.rotation ?? [0, 359]);
		if(rotation)
			obj->SetR(rotation);
		var rotationSpeed = FW_Distribution->GetValue(fx.rotationSpeed ?? 0);
		if(rotationSpeed)
			obj->SetRDir(rotationSpeed);
		
		obj.fireworkData = new fx.fireworkData {};
		obj->SetFused(this);
		
		if(fx.sound)
		{
			var dummy = CreateObject(Dummy);
			var pitch = fx.pitch;
			if(pitch)
				pitch = RandomX(pitch[0], pitch[1]);
			dummy->Sound(fx.sound, nil, nil, nil, nil, nil, pitch);
			dummy->RemoveObject();
		}
	}
}




